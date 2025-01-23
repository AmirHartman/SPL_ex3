#include "../include/CommandsHandler.h"
#include "../include/ClientFrames.h"

CommandsHandler::CommandsHandler() {}

void CommandsHandler::runCommand(vector<string> &args) {
    // check if command exists
    if (commandsMap.find(args[0]) == commandsMap.end()) {
        cout << "Command not found" << endl;
        return;
    }
    
    Commands command = commandsMap[args[0]];
    switch (command)
    {
    case LOGIN:
        if (args.size() != 5) {
            cerr << "Usage: login {host} {port} {username} {password}" << endl;
            return;
        }
        login(args[1], stoi(args[2]), args[3], args[4]);
        break;
    case JOIN:
        // join(args[1]);
        break;
    case EXIT:
        // exit(args[1]);
        break;
    case REPORT:
        // report(args[1]);
        break;
    case SUMMARY:
        // summary(args[1], args[2], args[3]);
        break;
    case LOGOUT:
        // logout();
        break;
    }
}

// לא גמור!
void CommandsHandler::login(string& host, short port, string& username, string& password) {
    cout << "Connecting to " << host << ":" << port << endl;
    connectionHandler = make_unique<ConnectionHandler>(host, port);
    if (!connectionHandler->connect()) {
        cerr << "Failed to connect" << endl;
        connectionHandler.reset();
        return;
    }
    cout << "Connected successfully as " << username << endl;
}



// // DELETE THIS (FOR TESTING PURPOSES)
// int main(int argc, char *argv[]) {
//     CommandsHandler commandsHandler;
//     vector<string> argsVector(argv + 1, argv + argc);
//     commandsHandler.runCommand(argsVector);
//     return 0;
// }