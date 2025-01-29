#include "../include/StompProtocol.h"

StompProtocol::StompProtocol() : connectionHandler(nullptr), encdec() {}

void StompProtocol::proccess(vector<string> &args) {
    if (args.empty()) {
        cout << "No command was entered" << endl;
        return;
    }

    string command = args[0];
    // check if command exists
    if (!(command == "login" || command == "join" || command == "exit" || command == "report" || command == "summary" || command == "logout")) {
        cout << "Command not found" << endl;
        cout << "Available commands: login, join, exit, report, summary, logout" << endl;
    }
    
    // Stop the execution if the client is not logged in and trying to run other command
    if (command != "login" && !isLoggedIn()) {
        if (DEBUG_MODE) cout << "[DEBUG] Tried to run the command " << command << " without being logged in.\nstopping the command execution\n" << endl;
        if (command == "logout") {
            cout << "You are not logged in" << endl;
        } else {
            cout << "You must login first" << endl;
        }
        return;
    }

    // Commands execution
    if (command == "login") {
        if (isLoggedIn()) {
            cout << "The client is already logged in, log out before trying again" << endl;
            return;
        }
        if (args.size() != 5) {
            cout << "Usage: login {host} {port} {username} {password}" << endl;
            return;
        } 

        // login command execution
        string host = args[1];
        short port = stoi(args[2]);
        string username = args[3];
        string password = args[4];
        connectionHandler.reset(new ConnectionHandler(host, port));
        bool isConnectedSuccessfully = false;
        bool isLoggedSuccessfully = false;

        if (connectionHandler->connect()) {
            isConnectedSuccessfully = true;
            if (DEBUG_MODE) cout << "\n[DEBUG] Connected to " << host << ":" << port << endl;
            Frame connectFrame = encdec.generateConnectFrame(host, port, username, password);
            Frame answerFrame = communicateServer(connectFrame);
            if (answerFrame.type == FrameType::CONNECTED) isLoggedSuccessfully = true;
        } else {
            if (DEBUG_MODE) cout << "\n[DEBUG] Failed executing connect() command to " << host << ":" << port << endl << endl;
        }

        if (!isConnectedSuccessfully || !isLoggedSuccessfully) {
            connectionHandler.reset(nullptr);
            if (!isConnectedSuccessfully) cout << "Failed to connect to " << host << ":" << port << endl;
            if (!isLoggedSuccessfully) cout << "Failed to login" << endl;
        } else {
            cout << "Logged in successfully" << endl;
        }

    } else if (command == "join") {
        if (args.size() != 2) {
            cout << "Usage: join {channelName}" << endl;
            return;
        }

        string channelName = args[1];
        cout << "Joining channel \"" << channelName << "\"..." << endl << endl;
        
        Frame subscribeFrame = encdec.generateSubscribeFrame(channelName);
        Frame answerFrame = communicateServer(subscribeFrame);
        if (answerFrame.type == FrameType::RECEIPT) {
            cout << "Joined channel \"" << channelName << "\"." << endl;
        } else {
            cout << "Failed to join channel \"" << channelName << "\"." << endl;
        }

    } else if (command == "exit") {
        if (args.size() != 2) {
            cout << "Usage: exit {channelName}" << endl;
            return;
        }

        string channelName = args[1];
        if (encdec.topicSubscriptionMap.find(channelName) == encdec.topicSubscriptionMap.end()) {
            cout << "Couldn't find the channel \"" << channelName << "\" in the subscribed channels" << endl;
            return;
        }

        cout << "Exiting channel \"" << channelName << "\"..." << endl;
        Frame unsubscribeFrame = encdec.generateUnsubscribeFrame(channelName);
        Frame answerFrame = communicateServer(unsubscribeFrame);
        if (answerFrame.type == FrameType::RECEIPT) {
            cout << "Exited channel \"" << channelName << "\"" << endl;
        } else {
            cout << "Failed to exit channel \"" << channelName << "\"" << endl;
        }

    } else if (command == "report") {
        if (args.size() != 2) {
            cout << "Usage: report {file_name}\n" << endl;
            return;
        }

        string fileName = args[1];
        if (fileName.find(".json") == string::npos) {
            if (fileName.find(".") != string::npos) {
                cout << "The file must be a .json file" << endl;
                return;
            } else {
                fileName += ".json";
            }
        }
        string path = "../data/" + fileName; // Relative to the bin folder

        fstream file(path);
        if (!file.good()) {
            cout << "Failed to open the file: \"" << fileName << "\"." << endl;
            cout << "Please make sure the file exists in the directory \"{program_folder}/clients/data/\" and try again." << endl;
            return;
        }

        // Parse the events file
        names_and_events namesAndEvents;
        try {
            namesAndEvents = parseEventsFile(path);
        } 
        catch (const std::exception& e) {
            cerr << "Failed to parse the events file." << endl;
            if (DEBUG_MODE) {
                cout << "\n[DEBUG] Exception message: " << e.what() << endl;
                cerr << e.what() << endl << endl;
            }
            return;
        }

        if (namesAndEvents.events.empty()) {
            cout << "The events file is empty." << endl;
            return;
        }

        cout << "Report for channel " << namesAndEvents.channel_name << endl;
        // int eventCounter = 1;
        // for (Event event : namesAndEvents.events) {
        //     cout << "Event no." << eventCounter << ":" << endl;
        //     cout << "____________________" << endl;
        //     cout << "Event name: " << event.get_name() << endl;
        //     cout << "City: " << event.get_city() << endl;
        //     cout << "Date: " << event.get_date_time() << endl;
        //     cout << "Description: " << event.get_description() << endl;
        //     cout << "General information:" << endl;
        //     for (auto const& pair : event.get_general_information()) {
        //         cout << pair.first << ": " << pair.second << endl;
        //     }
        //     cout << "____________________\n" << endl;
        //     eventCounter++;
        // }


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
            Frame sendFrame = encdec.generateSendFrame(namesAndEvents.channel_name, message);
            Frame answerFrame = communicateServer(sendFrame);
            if (answerFrame.type != FrameType::RECEIPT) {
                cout << "Failed to send the event \"" << event.get_name() << "\" to the server.\nStopping execution." << endl;
                return;
            }
        }


    } else if (command == "summary") {
        if (args.size() != 4) {
            cout << "Usage: summary {channelName} {user} {fileName}" << endl;
            return;
        }

        string channelName = args[1];
        string user = args[2];
        string fileName = args[3];
        // TODO: Implement this

    } else if (command == "logout") {
        if (args.size() != 1) {
            cout << "Usage: logout" << endl;
            return;
        }

        if (!isLoggedIn()) {
            if (DEBUG_MODE) cout << "[DEBUG] logout commands was executed but connection is offline" << endl;
            return;
        }

        cout << "Logging out..." << endl;
        Frame disconnectFrame = encdec.generateDisconnectFrame();
        Frame answerFrame = communicateServer(disconnectFrame);
        if (answerFrame.type == FrameType::RECEIPT) {
            cout << "Logged out successfully" << endl;
            connectionHandler.reset(nullptr);
        } else {
            cout << "Failed to log out" << endl;
        }
    }
}

Frame StompProtocol::communicateServer(Frame& frameToSend) {
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

    // Get the server's answer
    Frame answerFrame;
    string answerAsString;
    if (!error && !connectionHandler->getFrameAscii(answerAsString, '\0')) {
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

bool StompProtocol::isLoggedIn() {
    return connectionHandler != nullptr;
}