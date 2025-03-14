#include "../include/StompProtocol.h"

StompProtocol::StompProtocol(): in(*this), out(*this), connectionHandler(nullptr), encdec(), awaiting_frames_for_receipt(), events() {}
StompProtocol::In::In(StompProtocol& _parent) : p(_parent) {}
StompProtocol::Out::Out(StompProtocol& _parent) : p(_parent) {}

void StompProtocol::closeConnection() {
    if (connectionHandler != nullptr) {
        connectionHandler->close();
        connectionHandler.reset(nullptr);
    }
}


bool StompProtocol::Out::connect(string& host, short port, string username, string password) {
    p.connectionHandler.reset(new ConnectionHandler(host, port));

    if (p.connectionHandler->connect()) {
        return login(username, password);
    } else {
        screen_access.try_lock();
        cerr << "Could not connect to server." << endl;
        screen_access.unlock();
    }
    return false;
}

bool StompProtocol::Out::login(string username, string password) {
    Frame connectFrame = p.encdec.generateConnectFrame(username, password);
    if (sendFrame(connectFrame)){
        Frame server_answer = p.in.read_from_socket();
        if (server_answer.type == FrameType::CONNECTED) {
            p.connectionHandler->setUsername(username);
            return true;
        } else {
            p.in.proccess(server_answer);
        }
    }
    return false;
}

void StompProtocol::Out::join(string& channelName) {
    Frame subscribeFrame = p.encdec.generateSubscribeFrame(channelName);
    p.awaiting_frames_for_receipt[stoi(subscribeFrame.headers["receipt"])] = subscribeFrame;
    sendFrame(subscribeFrame);
}

void StompProtocol::Out::exit(string& channelName) {
    if (p.encdec.topicSubscriptionMap.find(channelName) == p.encdec.topicSubscriptionMap.end()) {
        screen_access.try_lock();
        cout << "you are not subscribed to channel " << channelName << endl;
        screen_access.unlock();
        return;
    }
    Frame unsubscribeFrame = p.encdec.generateUnsubscribeFrame(channelName);
    p.awaiting_frames_for_receipt[stoi(unsubscribeFrame.headers["receipt"])] = unsubscribeFrame;
    sendFrame(unsubscribeFrame);
}

void StompProtocol::Out::report(names_and_events& namesAndEvents) {
    if (p.encdec.topicSubscriptionMap.find(namesAndEvents.channel_name) == p.encdec.topicSubscriptionMap.end()) {
        screen_access.try_lock();
        cout << "You are not registered to channel " << namesAndEvents.channel_name << endl;
        screen_access.unlock();
        return;
    }
    
    string message;
    for (Event event : namesAndEvents.events) {
        screen_access.lock();
        event.setEventOwnerUser(p.connectionHandler->getUsername());
        message = "user:" + event.getEventOwnerUser() + "\n";
        message += "channel name:" + event.get_channel_name() + "\n";
        message += "city:" + event.get_city() + "\n";
        message += "event name:" + event.get_name() + "\n";
        message += "date time:" + to_string(event.get_date_time()) + "\n";
        message += "general information:\n";
        for (auto const& pair : event.get_general_information()) {
            message += "\t" + pair.first + ":" + pair.second + "\n";
        }
        message += "description:\n" + event.get_description() + "\n";
        p.out.addEvent(event);
        Frame reportFrame = p.encdec.generateSendFrame(namesAndEvents.channel_name, message);
        p.awaiting_frames_for_receipt[stoi(reportFrame.headers["receipt"])] = reportFrame;
        screen_access.unlock();
        sendFrame(reportFrame);
    }
    screen_access.try_lock();
    cout << "reported" << endl;
    screen_access.unlock();
}
    

void StompProtocol::Out::summary(string channel_name, string name, string file_name){
    if (p.encdec.topicSubscriptionMap.find(channel_name) == p.encdec.topicSubscriptionMap.end()) {
        screen_access.try_lock();
        cout << "you are not subscribed to channel " << channel_name <<  endl;
        screen_access.unlock();
        return;
    }
    events_lock.lock();
    string file_content = "Channel " + channel_name + "\n";
           file_content += "Stats: \n";
    // if the channel doesn't exist or the user didn't send any reports to this channel
    if (p.events.find(channel_name) == p.events.end() || p.events[channel_name].find(name) == p.events[channel_name].end()) {
        file_content += "Total: 0\n\n";
        file_content += "Event Reports:";
    }
    else {
        set<Event, EventComparator> events = p.events[channel_name][name];
        int total = events.size();
        int active = 0, forces_arrival_at_scene = 0, counter = 1;
        string reports = "Event Reports:\n\n";
        // iterating over the events by order
        for(set<Event, EventComparator>::iterator it = events.begin(); it != events.end(); ++it) { 
            update_stats(*it, active, forces_arrival_at_scene);
            update_reports(counter, reports, *it);       
        }
        file_content += "Total: " + to_string(total) + "\n";
        file_content += "active: " + to_string(active) + "\n";
        file_content += "forces arrival at scene: " + to_string(forces_arrival_at_scene) + "\n\n";
        file_content += reports;
    }
    if (file_name.find(".text") == string::npos && file_name.find(".") == string::npos) {
        file_name += ".text";
    }
    update_file(file_name, file_content);
    events_lock.unlock();

}

void StompProtocol::Out::logout(){
    Frame disconnectFrame = p.encdec.generateDisconnectFrame();
    p.awaiting_frames_for_receipt[stoi(disconnectFrame.headers["receipt"])] = disconnectFrame;
    sendFrame(disconnectFrame);
}

bool StompProtocol::Out::sendFrame(Frame& frameToSend) {
    string message = frameToSend.toString();

    // Send the frame to the server
    if (!p.connectionHandler->sendFrameAscii(message, '\0')) {
        screen_access.try_lock();
        cerr << "Failed to send request to the server. Connection error." << endl;
        screen_access.unlock();
        p.closeConnection();
        return false;
    }
    return true;
}

void StompProtocol::Out::addEvent(Event& event) {
    events_lock.lock();
    // if events map doesn't contain the channel, create a new map and insert event
    if (p.events.find(event.get_channel_name()) == p.events.end()) {
        p.events[event.get_channel_name()] = map<string, set<Event, EventComparator>>();
        p.events[event.get_channel_name()].emplace(event.getEventOwnerUser(), set<Event, EventComparator>());
        p.events[event.get_channel_name()][event.getEventOwnerUser()].insert(event);
    } // channel exists but no reports sent by this user so far
    else if (p.events[event.get_channel_name()].find(event.getEventOwnerUser()) == p.events[event.get_channel_name()].end()) {
        p.events[event.get_channel_name()].emplace(event.getEventOwnerUser(), set<Event, EventComparator>());
        p.events[event.get_channel_name()][event.getEventOwnerUser()].insert(event);
    } // channel exists and user sent reports in the past
    else {
        p.events[event.get_channel_name()][event.getEventOwnerUser()].insert(event);
    }
    events_lock.unlock();
}

bool StompProtocol::In::proccess(Frame &server_answer) {
    bool should_disconnect = false;
    switch(server_answer.type){
        case FrameType::MESSAGE:
            try {
                Event event(server_answer.body);  
                p.out.addEvent(event);
            } catch (const std::exception& e) {
                screen_access.try_lock();
                cout << "Error creating event: " << e.what() << endl;
                screen_access.unlock();
            }
            break;
        case FrameType::RECEIPT:
            should_disconnect = proccessReceipt(server_answer);
            break;
        case FrameType::ERROR:
            screen_access.try_lock();
            cout << "ERROR FROM THE SERVER:\n\n" << server_answer.toString() << endl;
            screen_access.unlock();
            should_disconnect = true;
            should_terminate = true;
            break;
        case FrameType::UNKNOWN:
            // Server's answer failed to be decoded -- in this case, the client should disconnect manually (in other cases, the server will disconnect the client)
            screen_access.try_lock();
            cout << "Server's answer failed to be decoded." << endl;
            screen_access.unlock();
            should_disconnect = true;
            should_terminate = true;
            break;
        default:
            break;
    }
    if (should_disconnect) {
        p.closeConnection();
    }
    return should_disconnect;
}

bool StompProtocol::In::proccessReceipt(Frame &server_answer) {
    bool should_disconnect = false;
    int receipt_id = stoi(server_answer.headers["receipt-id"]);
    if (p.awaiting_frames_for_receipt.find(receipt_id) == p.awaiting_frames_for_receipt.end()) {
        screen_access.try_lock();
        cout << "Received a receipt for an unknown frame." << endl;
        screen_access.unlock();
        return true;
    }

    Frame frame_related_to_receipt = p.awaiting_frames_for_receipt[receipt_id];
    p.awaiting_frames_for_receipt.erase(receipt_id);

    if (frame_related_to_receipt.type == FrameType::UNSUBSCRIBE) {
        int subscriptionId = stoi(frame_related_to_receipt.headers["id"]);
        string topic = "";
        for (auto const& pair : p.encdec.topicSubscriptionMap) {
            if (pair.second == subscriptionId) {
                topic = pair.first;
                break;
            }
        }
        if (topic == "") {
            screen_access.try_lock();
            cout << "you are not subscribed to channel " << frame_related_to_receipt.headers["destination"].substr(1) << endl;
            screen_access.unlock();
            return true;
        } else {
            frame_related_to_receipt.headers.erase("id");
            frame_related_to_receipt.headers["destination"] = topic;
            p.encdec.topicSubscriptionMap.erase(topic);
        }
    }

    screen_access.try_lock();
    switch(frame_related_to_receipt.type){
        case FrameType::SUBSCRIBE:
            cout << "Joined channel " << frame_related_to_receipt.headers["destination"].substr(1) << endl;
            break;
        case FrameType::UNSUBSCRIBE:
            cout << "Exited channel " << frame_related_to_receipt.headers["destination"] << endl;
            break;
        case FrameType::SEND:
            break;
        case FrameType::DISCONNECT:
            should_disconnect = true;
            cout << "Logged out" << endl;
            break;
        default:
            break;
    }
    screen_access.unlock();
    return should_disconnect;
}

Frame StompProtocol::In::read_from_socket(){
    Frame answerFrame;
    string answerAsString;
    if (!p.connectionHandler->getFrameAscii(answerAsString, '\0')) {
        screen_access.try_lock();
        cerr << "Failed to receive an answer from the server." << endl;
        screen_access.unlock();
        p.closeConnection();
    } else {
        answerFrame = p.encdec.generateFrameFromString(answerAsString);
    }
    return answerFrame;
}

string StompProtocol::Out::epoch_to_date(int date) {
    time_t time = date;
    struct tm * timeinfo = localtime(&time);
    char buffer[25];
    strftime(buffer, 25, "%d-%m-%Y %H:%M:%S", timeinfo);
    return string(buffer);
}

void StompProtocol::Out::update_stats (const Event &event, int& active, int& forces_arrival_at_scene){
    const map<string, string> & general_information = event.get_general_information();
    if (general_information.find("active") != general_information.end() && general_information.at("active") == "true") {
        active++;
    }
    if (general_information.find("forces_arrival_at_scene") != general_information.end() && general_information.at("forces_arrival_at_scene") == "true") {
        forces_arrival_at_scene++;
    }
}

void StompProtocol::Out::update_reports (int& counter, string& reports, const Event &event){
    reports += "Report_" + to_string(counter) + ":\n";
    reports += "city: " + event.get_city() + "\n";
    reports += "date time: " + epoch_to_date(event.get_date_time()) + "\n";
    reports += "event name: " + event.get_name() + "\n";
    if (event.get_description().length() > 27){
        reports += "summary: " + event.get_description().substr(0, 27) + "...\n";
    }    else {
        reports += "summary: " + event.get_description() + "\n";
    }
    reports += "\n\n";
    counter++;
}

void StompProtocol::Out::update_file (string &file_name, string &file_content){
    ofstream file(file_name, std::ios::trunc); 
    if (!file) {
        cout << "Error: Could not open file " << file_name << " for writing.\n";
        return;
    }
    file << file_content;  
    file.close();
}


