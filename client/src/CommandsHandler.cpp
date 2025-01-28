#include "../include/CommandsHandler.h"
#include "../include/MessageEncoderDecoder.h"

extern bool DEBUG_MODE;

CommandsHandler::CommandsHandler() : connectionHandler(nullptr), encdec() {}

void CommandsHandler::runCommand(vector<string> &args) {
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
    if (command != "login" && !checkIfLoggedIn()) {
        return;
    }

    // Commands execution
    if (command == "login") {
        if (connectionHandler != nullptr) {
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

        if (!connectionHandler->connect()) {
            if (DEBUG_MODE) cout << "\n[DEBUG] Failed executing connect() command to " << host << ":" << port << endl << endl;
            return;
        }
        isConnectedSuccessfully = true;
        if (DEBUG_MODE) cout << "\n[DEBUG] Connected to " << host << ":" << port << endl;

        Frame connectFrame = encdec.generateConnectFrame(host, port, username, password);
        Frame answerFrame = communicateServer(connectFrame);
        if (answerFrame.type == FrameType::CONNECTED) isLoggedSuccessfully = true;

        if (!isConnectedSuccessfully || !isLoggedSuccessfully) {
            connectionHandler.reset();
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
        cout << "Joining channel " << channelName << " ..." << endl << endl;
        
        Frame subscribeFrame = encdec.generateSubscribeFrame(channelName);
        Frame answerFrame = communicateServer(subscribeFrame);
        if (answerFrame.type == FrameType::RECEIPT) {
            cout << "Joined channel " << channelName << endl;
        } else {
            cout << "Failed to join channel " << channelName << endl;
        }

    } else if (command == "exit") {
        if (args.size() != 2) {
            cout << "Usage: exit {channelName}" << endl;
            return;
        }

        string channelName = args[1];
        cout << "Exiting channel " << channelName << endl;
        Frame unsubscribeFrame = encdec.generateUnsubscribeFrame(channelName);
        Frame answerFrame = communicateServer(unsubscribeFrame);
        if (answerFrame.type == FrameType::RECEIPT) {
            cout << "Exited channel " << channelName << endl;
        } else {
            cout << "Failed to exit channel " << channelName << endl;
        }

    } else if (command == "report") {
        if (args.size() != 2) {
            cout << "Usage: report {filePath}" << endl;
            return;
        }

        string filePath = args[1];
        // TODO: Implement this

    } else if (command == "summary") {
        if (args.size() != 4) {
            cout << "Usage: summary {channelName} {user} {filePath}" << endl;
            return;
        }

        string channelName = args[1];
        string user = args[2];
        string filePath = args[3];
        // TODO: Implement this

    } else if (command == "logout") {
        if (args.size() != 1) {
            cout << "Usage: logout" << endl;
            return;
        }

        if (!connectionHandler) {
            if (DEBUG_MODE) cout << "[DEBUG] logout commands was executed but connection is offline" << endl;
            return;
        }

        cout << "Logging out" << endl;
        Frame disconnectFrame = encdec.generateDisconnectFrame();
        Frame answerFrame = communicateServer(disconnectFrame);
        if (answerFrame.type == FrameType::RECEIPT) {
            cout << "Logged out successfully" << endl;
        } else {
            cout << "Failed to log out" << endl;
        }

    }
}


void CommandsHandler::handleUNKNOWNFrame(string &answerAsString) {
    if (DEBUG_MODE) {
        cout << "[DEBUG] Server's answer failed to be decoded." << endl;
        cout << "[DEBUG] Frame received from server (as a string):\n" << answerAsString << endl;
    }
    cout << "\nAn error occured during communicating to server. Closing connection.." << endl;
    vector<string> logout = {"logout"};
    runCommand(logout); // try to logout before closing the connection
    connectionHandler.reset();
}

void CommandsHandler::handleERRORFrame(Frame &errorFrame) {
    cout << "Error message received from server: " << errorFrame.headers["message"] << endl;
    if (errorFrame.body != "") {
        cout << "Additional information received from server:\n" << errorFrame.body << "\n" << endl;
    }
    cout << "Disconnecting from server..." << endl;
    connectionHandler.reset();
}

bool CommandsHandler::checkIfLoggedIn() {
    if (!connectionHandler) {
        cout << "You must login first" << endl;
        return false;
    }
    return true;
}

Frame CommandsHandler::communicateServer(Frame& frameToSend) {
    string message = frameToSend.toString();
    Frame output = Frame();

    // Send the frame to the server
    if (!connectionHandler->sendFrameAscii(message, '\0')) {
        cerr << "Failed to send request to the server. Connection error." << endl;
    } else {
        if (DEBUG_MODE) cout << "\n[DEBUG] Frame successfully sent to the server" << endl;
    }

    // Get the server's answer
    string answerAsString;
    if (!connectionHandler->getFrameAscii(answerAsString, '\0')) {
        cerr << "Failed to receive an answer from the server." << endl;
    } else {
        if (DEBUG_MODE) cout << "[DEBUG] Answer frame successfully received from the server\n" << endl;
        output = encdec.generateFrameFromString(answerAsString);
    }

    // If answer could not be decoded
    if (output.type == FrameType::UNKNOWN) {
        handleUNKNOWNFrame(answerAsString);
    }

    // If answer is an error frame
    if (output.type == FrameType::ERROR) {
        handleERRORFrame(output);
    }

    return output;
}