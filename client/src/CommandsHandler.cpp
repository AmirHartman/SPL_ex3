#include "../include/CommandsHandler.h"
#include "../include/MessageEncoderDecoder.h"

CommandsHandler::CommandsHandler() {}

bool CommandsHandler::runCommand(vector<string> &args) {
    // check if command exists
    if (commandsMap.find(args[0]) == commandsMap.end()) {
        cout << "Command not found" << endl;
        return false;
    }

    // check if user is logged in
    if (!connectionHandler) {
        cout << "You must login first" << endl;
        return false;
    }
    
    Commands command = commandsMap[args[0]];
    switch (command)
    {
    case LOGIN:
        if (args.size() != 5) {
            cout << "Usage: login {host} {port} {username} {password}" << endl;
            return false;
        }
        return login(args[1], stoi(args[2]), args[3], args[4]);
    case JOIN:
        return join(args[1]);
    case EXIT:
        // return exit(args[1]);
    case REPORT:
        // return report(args[1]);
    case SUMMARY:
        // return summary(args[1], args[2], args[3]);
    case LOGOUT:
        // return logout();
    }
}

bool CommandsHandler::login(string& host, short port, string& username, string& password) {
    bool isConnectedSuccessfully = false;

    if (connectionHandler != nullptr) {
        cout << "The client is already logged in, log out before trying again" << endl;
    } else {
        connectionHandler = make_unique<ConnectionHandler>(host, port);
    }

    if (!connectionHandler->connect()) {
        cout << "Could not connect to server" << endl;
    } else {
        string frame = encdec.generateConnectFrame(host, port, username, password);
        if (!connectionHandler->sendFrameAscii(frame, '\0')){
            cerr << "Failed to send the connect frame" << endl;
        }

        string answer;
        if (!connectionHandler->getFrameAscii(answer, '\0')) {
            cerr << "Failed to get the answer from the server" << endl;
        }

        encdec.decodeFrame(answer); // TODO: check with yam's user already logged on error frame and print 

        if (encdec.getServerMessageType(answer) == ServerFrameType::CONNECTED) {
            isConnectedSuccessfully = true;
        }
        
        return isConnectedSuccessfully;
    }

}

bool CommandsHandler::join(string& channelName) {
    cout << "Joining channel " << channelName << endl;
    string frame = encdec.generateSubscribeFrame(channelName);
    if (connectionHandler->sendFrameAscii(frame, '\0')) {
        cerr << "Failed to send the subscribe frame" << endl;
        return false;
    }
    string answer;
    if (!connectionHandler->getFrameAscii(answer, '\0')) {
        cerr << "Failed to receive an answer from the server" << endl;
        return false;
    }
    
    cout << "Joined channel " << channelName << endl;
    return true;
}


// // DELETE THIS (FOR TESTING PURPOSES)
// int main(int argc, char *argv[]) {
//     CommandsHandler commandsHandler;
//     vector<string> argsVector(argv + 1, argv + argc);
//     commandsHandler.runCommand(argsVector);
//     return 0;
// }