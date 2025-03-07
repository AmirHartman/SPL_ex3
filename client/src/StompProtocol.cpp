#include "../include/StompProtocol.h"

StompProtocol::StompProtocol(): in(*this), out(*this), connectionHandler(nullptr), encdec(), awaiting_frames_for_receipt(), events() {}
StompProtocol::In::In(StompProtocol& _parent) : p(_parent) {}
StompProtocol::Out::Out(StompProtocol& _parent) : p(_parent) {}

void StompProtocol::closeConnection() {
    if (connectionHandler != nullptr) {
        cout << "Closing connection...\n" << endl;
        connectionHandler->close();
        connectionHandler.reset(nullptr);
    }
}


bool StompProtocol::Out::connect(string& host, short port, string username, string password) {
    p.connectionHandler.reset(new ConnectionHandler(host, port));

    if (p.connectionHandler->connect()) {
        if (DEBUG_MODE) cout << "\n[DEBUG] Connected to " << host << ":" << port << endl;
        return login(username, password);
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
        cout << "Couldn't find the channel \"" << channelName << "\" in the subscribed channels" << endl;
        return;
    }
    Frame unsubscribeFrame = p.encdec.generateUnsubscribeFrame(channelName);
    p.awaiting_frames_for_receipt[stoi(unsubscribeFrame.headers["receipt"])] = unsubscribeFrame;
    sendFrame(unsubscribeFrame);
}

void StompProtocol::Out::report(names_and_events& namesAndEvents) {
    cout << "Report for channel " << namesAndEvents.channel_name << endl;
    
    string message;
    for (Event event : namesAndEvents.events) {
        event.setEventOwnerUser(p.connectionHandler->getUsername());
        cout << "Sending the event \"" << event.get_name() << "\" to the server..." << endl;
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
        sendFrame(reportFrame);
    }
}


void StompProtocol::Out::summary() {
}

void StompProtocol::Out::logout(){
    Frame disconnectFrame = p.encdec.generateDisconnectFrame();
    p.awaiting_frames_for_receipt[stoi(disconnectFrame.headers["receipt"])] = disconnectFrame;
    sendFrame(disconnectFrame);
}

bool StompProtocol::Out::sendFrame(Frame& frameToSend) {
    string message = frameToSend.toString();

    if (DEBUG_MODE) {
        cout << "[DEBUG] Frame sent to the server:" << endl;
        cout << "____________________" << endl;
        cout << message << endl;
        cout << "____________________" << endl;
    }

    // Send the frame to the server
    if (!p.connectionHandler->sendFrameAscii(message, '\0')) {
        cerr << "Failed to send request to the server. Connection error." << endl;
        return false;
    } else {
        if (DEBUG_MODE) cout << "\n[DEBUG] Frame successfully sent to the server" << endl;
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
            cout << "Message received from the server: \n" << server_answer.body << endl;
            try {
                Event event(server_answer.body);  
                p.out.addEvent(event);
            } catch (const std::exception& e) {
                cout << "Error creating event: " << e.what() << endl;
            }
            break;
        case FrameType::RECEIPT:
            should_disconnect = proccessReceipt(server_answer);
            break;
        case FrameType::ERROR:
            cout << "Error message received from the server: " << server_answer.headers["message"] << "." << endl;
            should_disconnect = true;
            break;
        case FrameType::UNKNOWN:
            // Server's answer failed to be decoded -- in this case, the client should disconnect manually (in other cases, the server will disconnect the client)
            if (DEBUG_MODE) {
                cout << "[DEBUG] Server's answer failed to be decoded." << endl;
            }
            should_disconnect = true;
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
        cout << "Received a receipt for an unknown frame." << endl;
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
            cout << "Couldn't find the channel id " << subscriptionId << "in the subscribed channels" << endl;
            return true;
        } else {
            frame_related_to_receipt.headers.erase("id");
            frame_related_to_receipt.headers["destination"] = topic;
            p.encdec.topicSubscriptionMap.erase(topic);
        }
    }

    switch(frame_related_to_receipt.type){
        case FrameType::SUBSCRIBE:
            cout << "Subscribed to channel \"" << frame_related_to_receipt.headers["destination"] << "\"." << endl;
            break;
        case FrameType::UNSUBSCRIBE:
            cout << "Unsubscribed from channel \"" << frame_related_to_receipt.headers["destination"] << "\"." << endl;
            break;
        case FrameType::SEND:
            cout << "Event successfully sent to the server." << endl;
            break;
        case FrameType::DISCONNECT:
            should_disconnect = true;
            cout << "Logged out successfully." << endl;
            break;
        default:
            break;
    }
    return should_disconnect;
}

Frame StompProtocol::In::read_from_socket(){
    Frame answerFrame;
    string answerAsString;
    cout << "reading from socket again" << endl;
    if (!p.connectionHandler->getFrameAscii(answerAsString, '\0')) {
        cout << "!p.connectionHandler->getFrameAscii(answerAsString, '\0')" << endl;
        screen_access.try_lock();
        cerr << "Failed to receive an answer from the server." << endl;
        p.closeConnection();
        screen_access.unlock();
    } else {
        cout << "before decoding" << endl;
        answerFrame = p.encdec.generateFrameFromString(answerAsString);
        cout << "after decoding" << endl;
        if (DEBUG_MODE) {
            screen_access.try_lock();
            cout << "[DEBUG] Answer frame successfully received from the server." << endl;
            cout << "[DEBUG] Frame received from the server:" << endl;
            cout << "____________________" << endl;
            cout << answerFrame.toString() << endl;
            cout << "____________________\n\n" << endl;
            screen_access.unlock();
        } 
    }
    return answerFrame;
}





//   /workspaces/SPL_ex3/events4.json
//    login 127.0.0.1 8888
