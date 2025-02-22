#include "../include/StompProtocol.h"

StompProtocol::StompProtocol() : connectionHandler(nullptr), encdec(), receipts() {}

bool StompProtocol::isLoggedIn() {
    return connectionHandler != nullptr;
}

void StompProtocol::connect(string& host, short port, string& username, string& password) {
    connectionHandler.reset(new ConnectionHandler(host, port));
    bool error = false;

    if (connectionHandler->connect()) {
        if (DEBUG_MODE) cout << "\n[DEBUG] Connected to " << host << ":" << port << endl;
        Frame connectFrame = encdec.generateConnectFrame(host, port, username, password);
        error = !sendFrame(connectFrame);
    } else {
        error = true;
    }

    if (error) {
        connectionHandler.reset(nullptr);
    }
}

void StompProtocol::join(string& channelName) {
    Frame subscribeFrame = encdec.generateSubscribeFrame(channelName);
    receipts[stoi(subscribeFrame.headers["receipt"])] = FrameType::SUBSCRIBE;
    sendFrame(subscribeFrame);
}

void StompProtocol::exit(string& channelName) {
    if (encdec.topicSubscriptionMap.find(channelName) == encdec.topicSubscriptionMap.end()) {
        cout << "Couldn't find the channel \"" << channelName << "\" in the subscribed channels" << endl;
        return;
    }
    Frame unsubscribeFrame = encdec.generateUnsubscribeFrame(channelName);
    sendFrame(unsubscribeFrame);
}

void StompProtocol::report(names_and_events& namesAndEvents) {
    cout << "Report for channel " << namesAndEvents.channel_name << endl;
    
    string message;
    for (Event event : namesAndEvents.events) {
        cout << "Sending the event \"" << event.get_name() << "\" to the server..." << endl;
        message = "user: " + event.getEventOwnerUser() + "\n";
        message += "city: " + event.get_city() + "\n";
        message += "event name: " + event.get_name() + "\n";
        message += "date time: " + to_string(event.get_date_time()) + "\n";
        message += "general information:\n";
        for (auto const& pair : event.get_general_information()) {
            message += "\t" + pair.first + ": " + pair.second + "\n";
        }
        message += "description:\n" + event.get_description() + "\n";
        Frame reportFrame = encdec.generateSendFrame(namesAndEvents.channel_name, message);
        sendFrame(reportFrame);
    }
}

void StompProtocol::summary() {

}

void StompProtocol::logout(){
    Frame disconnectFrame = encdec.generateDisconnectFrame();
    sendFrame(disconnectFrame);
}

void StompProtocol::proccess(vector<string> &args) {
    // ______________________________ NOT FINISHED ______________________________

}

bool StompProtocol::sendFrame(Frame& frameToSend) {
    string message = frameToSend.toString();
    bool error = false;

    if (DEBUG_MODE) {
        cout << "[DEBUG] Frame sent to the server:" << endl;
        cout << "____________________" << endl;
        cout << message << endl;
        cout << "____________________" << endl;
    }

    // Check if connected
    if (!isLoggedIn()) {
        cout << "[DEBUG] Tried to communicate the server without being logged in." << endl;
        error = true;
    }

    // Send the frame to the server
    if (!error && !connectionHandler->sendFrameAscii(message, '\0')) {
        cerr << "Failed to send request to the server. Connection error." << endl;
        error = true;
    } else {
        if (DEBUG_MODE) cout << "\n[DEBUG] Frame successfully sent to the server" << endl;
    }

    return error;
}

Frame StompProtocol::readFrameFromSocket(){
    // Get the server's answer
    Frame answerFrame;
    string answerAsString;
    bool error = false;
    if (!connectionHandler->getFrameAscii(answerAsString, '\0')) {
        cerr << "Failed to receive an answer from the server." << endl;
        error = true;
    } else {
        answerFrame = encdec.generateFrameFromString(answerAsString);
        if (DEBUG_MODE) {
            cout << "[DEBUG] Answer frame successfully received from the server." << endl;
            cout << "[DEBUG] Frame received from the server:" << endl;
            cout << "____________________" << endl;
            cout << answerFrame.toString() << endl;
            cout << "____________________\n\n" << endl;
        } 

    }

    // Error handling:
    if (!error && answerFrame.type == FrameType::UNKNOWN) {
        // Server's answer failed to be decoded -- in this case, the client should disconnect manually (in other cases, the server will disconnect the client)
        if (DEBUG_MODE) {
            cout << "[DEBUG] Server's answer failed to be decoded." << endl;
            cout << "[DEBUG] Frame received from server (the returned string):\n" << answerAsString << endl;
        }
        vector<string> logout = {"logout"};
        proccess(logout); 
        error = true;
    } else if (!error && answerFrame.type == FrameType::ERROR) {
        // Server's answer is an error frame
        cout << "Error message received from server: " << answerFrame.headers["message"] << "." << endl << endl;
        error = true;
    }

    if (error) {
        cout << "Closing connection...\n" << endl;
        connectionHandler.reset(nullptr);
        return Frame();
    }

    return answerFrame;
}