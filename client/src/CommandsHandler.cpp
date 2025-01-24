#include "../include/CommandsHandler.h"
#include "../include/MessageEncoderDecoder.h"

CommandsHandler::CommandsHandler() {}

bool CommandsHandler::runCommand(vector<string> &args) {
    // check if command exists
    if (commandsMap.find(args[0]) == commandsMap.end()) {
        cout << "Command not found" << endl;
        return false;
    }

    if (!connectionHandler) {
        cout << "You must login first" << endl;
        return false;
    }
    
    Commands command = commandsMap[args[0]];
    switch (command)
    {
    case LOGIN:
        if (args.size() != 5) {
            cerr << "Usage: login {host} {port} {username} {password}" << endl;
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

// לא גמור!
bool CommandsHandler::login(string& host, short port, string& username, string& password) {
    cout << "Connecting to " << host << ":" << port << endl;
    connectionHandler = make_unique<ConnectionHandler>(host, port);
    if (!connectionHandler->connect()) {
        cerr << "Failed to connect" << endl;
        connectionHandler.reset();
        return false;
    }
    cout << "Connected successfully as " << username << endl;
    return true;
}

bool CommandsHandler::join(string& channelName) {
    cout << "Joining channel " << channelName << endl;
    string frame = clientFrames.generateSubscribeFrame(channelName);
    connectionHandler->sendFrameAscii(frame, '\0');
    string answer;
    if (!connectionHandler->getFrameAscii(answer, '\0')) {
        
        cerr << "Failed to join channel " << channelName << endl;
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