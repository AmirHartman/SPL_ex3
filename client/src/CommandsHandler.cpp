#include "../include/CommandsHandler.h"
#include "../include/MessageEncoderDecoder.h"

CommandsHandler::CommandsHandler() : connectionHandler(nullptr), encdec() {}

void CommandsHandler::runCommand(vector<string> &args) {
    bool error = false;
    
    // check if command exists
    if (commandsMap.find(args[0]) == commandsMap.end()) {
        cout << "Command not found" << endl;
        cout << "Available commands: login, join, exit, report, summary, logout" << endl;
    }
    
    Commands command = commandsMap[args[0]];
    if (command != LOGIN && !checkIfLoggedIn()) {
        return;
    }
    switch (command) {
        case LOGIN:
            if (args.size() != 5) {
                cout << "Usage: login {host} {port} {username} {password}" << endl;
            } else {
                login(args[1], stoi(args[2]), args[3], args[4]);
            }
            break;
        case JOIN:
            if (args.size() != 2) {
                cout << "Usage: join {channelName}" << endl;
            } else {
                error = !join(args[1]);
            }
            break;
        case EXIT:
            if (args.size() != 2) {
                cout << "Usage: exit {channelName}" << endl;
            } else {
                error = !exit(args[1]);
            }
            break;
        case REPORT:
            if (args.size() != 2) {
                cout << "Usage: report {filePath}" << endl;
            } else {
                error = !report(args[1]);
            }
            break;
        case SUMMARY:
            if (args.size() != 4) {
                cout << "Usage: summary {channelName} {user} {filePath}" << endl;
            } else {
                error = !summary(args[1], args[2], args[3]);
            }
            break;
        case LOGOUT:
            if (args.size() != 1) {
                cout << "Usage: logout" << endl;
            } else {
                error = !logout();
            }
            break;
    }

    if (error) {
        cout << "An error occurred while executing the command" << endl;
        cout << "Disconnecting from server..." << endl;
        connectionHandler.reset();
    }
}

void CommandsHandler::login(string& host, short port, string& username, string& password) {
    if (connectionHandler != nullptr) {
        cout << "The client is already logged in, log out before trying again" << endl;
        return;
    } else {
        connectionHandler.reset(new ConnectionHandler(host, port));
    }

    bool isConnectedSuccessfully = false;
    if (!connectionHandler->connect()) {
        cout << "Could not connect to server" << endl;
    } else {
        Frame frame = encdec.generateConnectFrame(host, port, username, password);
        if (!connectionHandler->sendFrameAscii(frame.toString(), '\0')){
            cerr << "Failed to send the connect frame" << endl;
        }

        string answerAsString;
        if (!connectionHandler->getFrameAscii(answerAsString, '\0')) {
            cerr << "Failed to get the answer from the server" << endl;
        }

        Frame answerFrame = encdec.decodeFrame(answerAsString);
        switch(answerFrame.type) {
            case FrameType::CONNECTED:
                cout << "Login successful" << endl;
                isConnectedSuccessfully = true;
                break;
            case FrameType::ERROR:
                cout << "Connection to server succeeded but server returned a login error." << endl;
                cout << "Error message: " << answerFrame.headers["message"] << endl;
                if (answerFrame.body != "") {
                    cout << "Additional information received from server:\n" << answerFrame.body << "\n" << endl;
                }
                break;
            case FrameType::UNKNOWN:
                cout << "Connection to server succeeded." << endl;
                handleUNKNOWNFrame(answerAsString);
                break;

            // Mention other enum values to avoid compilation warnings
            case FrameType::DISCONNECT:
            case FrameType::SEND:
            case FrameType::MESSAGE:
            case FrameType::CONNECT:
            case FrameType::SUBSCRIBE:
            case FrameType::UNSUBSCRIBE:
            case FrameType::RECEIPT:
                break;
        }

    }
    if (!isConnectedSuccessfully) {
        cout << "Failed to login" << endl;
        connectionHandler.reset();
    }
}

bool CommandsHandler::join(string& channelName) {
    cout << "Joining channel " << channelName << endl;
    Frame frame = encdec.generateSubscribeFrame(channelName);
    if (connectionHandler->sendFrameAscii(frame.toString(), '\0')) {
        cerr << "Failed to send the subscribe frame. Connection error." << endl;
        return false;
    }
    string answerAsString;
    if (!connectionHandler->getFrameAscii(answerAsString, '\0')) {
        cerr << "Failed to receive an answer from the server." << endl;
        return false;
    }

    bool joinedSuccessfully = false;
    Frame answerFrame = encdec.decodeFrame(answerAsString);
    switch(answerFrame.type) {
        case FrameType::RECEIPT:
            cout << "Joined channel " << channelName << endl;
            joinedSuccessfully = true;
            break;
        case FrameType::ERROR:
            cout << "Failed to join channel " << channelName << endl;
            cout << "Error message: " << answerFrame.headers["message"] << endl;
            if (answerFrame.body != "") {
                cout << "Additional information received from server:\n" << answerFrame.body << "\n" << endl;
            }
            break;
        case FrameType::UNKNOWN:
            cout << "Failed to join channel " << channelName << endl;
            handleUNKNOWNFrame(answerAsString);
            break;

        // Mention other enum values to avoid compilation warnings
        case CONNECT:
        case CONNECTED:
        case DISCONNECT:
        case SEND:
        case MESSAGE:
        case SUBSCRIBE:
        case UNSUBSCRIBE:
            break;
    }
    return joinedSuccessfully;
}

bool CommandsHandler::exit(string& channelName) {
    cout << "Exiting channel " << channelName << endl;
    Frame frame = encdec.generateUnsubscribeFrame(channelName);
    if (connectionHandler->sendFrameAscii(frame.toString(), '\0')) {
        cerr << "Failed to send the unsubscribe frame. Connection error." << endl;
        return false;
    }
    string answerAsString;
    if (!connectionHandler->getFrameAscii(answerAsString, '\0')) {
        cerr << "Failed to receive an answer from the server." << endl;
        return false;
    }

    bool exitedSuccessfully = false;
    Frame answerFrame = encdec.decodeFrame(answerAsString);
    switch(answerFrame.type) {
        case FrameType::RECEIPT:
            cout << "Exited channel " << channelName << endl;
            exitedSuccessfully = true;
            break;
        case FrameType::ERROR:
            cout << "Failed to exit channel " << channelName << endl;
            cout << "Error message: " << answerFrame.headers["message"] << endl;
            if (answerFrame.body != "") {
                cout << "Additional information received from server:\n" << answerFrame.body << "\n" << endl;
            }
            break;
        case FrameType::UNKNOWN:
            cout << "Failed to exit channel " << channelName << endl;
            handleUNKNOWNFrame(answerAsString);
            break;
        
        // Mention other enum values to avoid compilation warnings
        case FrameType::CONNECT:
        case FrameType::CONNECTED:
        case FrameType::DISCONNECT:
        case FrameType::SEND:
        case FrameType::MESSAGE:
        case FrameType::SUBSCRIBE:
        case FrameType::UNSUBSCRIBE:
            break;
    }
    return exitedSuccessfully;
}

bool CommandsHandler::report(string& filePath) {
    // TODO: Finish implementing this
    names_and_events events = parseEventsFile(filePath);
    for (Event event : events.events) {
        // TODO: for each event, send a message to the server
    }
    return true;
}

bool CommandsHandler::summary(string& channelName, string& user, string& filePath) {
    // TODO: Implement this
    return true;
}

bool CommandsHandler::logout() {
    cout << "Logging out" << endl;
    Frame frame = encdec.generateDisconnectFrame();
    if (connectionHandler->sendFrameAscii(frame.toString(), '\0')) {
        cerr << "Failed to send the disconnect frame. Connection error." << endl;
        return false;
    }
    string answerAsString;
    if (!connectionHandler->getFrameAscii(answerAsString, '\0')) {
        cerr << "Failed to receive an answer from the server." << endl;
        return false;
    }

    bool loggedOutSuccessfully = false;
    Frame answerFrame = encdec.decodeFrame(answerAsString);
    switch(answerFrame.type) {
        case FrameType::RECEIPT:
            cout << "Logged out successfully" << endl;
            loggedOutSuccessfully = true;
            break;
        case FrameType::ERROR:
            cout << "Failed to log out" << endl;
            cout << "Error message: " << answerFrame.headers["message"] << endl;
            if (answerFrame.body != "") {
                cout << "Additional information received from server:\n" << answerFrame.body << "\n" << endl;
            }
            break;
        case FrameType::UNKNOWN:
            cout << "Failed to log out" << endl;
            handleUNKNOWNFrame(answerAsString);
            break;

        // Mention other enum values to avoid compilation warnings
        case FrameType::CONNECT:
        case FrameType::CONNECTED:
        case FrameType::DISCONNECT:
        case FrameType::SEND:
        case FrameType::MESSAGE:
        case FrameType::SUBSCRIBE:
        case FrameType::UNSUBSCRIBE:
            break;
    }
    return loggedOutSuccessfully;
}

void CommandsHandler::handleUNKNOWNFrame(string &answerAsString) {
    cout << "Server's answer failed to be decoded." << endl;
    cout << "Frame received from server (as a string):\n" << answerAsString << endl;
    logout();
}

bool CommandsHandler::checkIfLoggedIn() {
    if (!connectionHandler) {
        cout << "You must login first" << endl;
        return false;
    }
    return true;
}