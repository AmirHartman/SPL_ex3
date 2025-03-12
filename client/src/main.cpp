#include "../include/StompProtocol.h"
#include "../include/CommandsHandler.h"
#include "../include/SocketReader.h"
#include "../include/ConnectionHandler.h"

#include <iostream>
#include <sstream>

bool DEBUG_MODE = false;
mutex screen_access;
mutex events_lock;

int main(int argc, char *argv[]) {
    // DEBUG_MODE = true;

    CommandsHandler command_handler;
    cout << "Client started\n" << endl;
    cout << "Commands available: login, join, exit, report, summary, logout" << endl;
    cout << "To exit the client, type 'quit'\n" << endl;
    cout << "Please enter a command:" << endl;

    string command;
    while (getline(cin, command)) {
        cout << endl;
        vector<string> commandVector;
        stringstream ss(command);
        string word;
        while (ss >> word) {
            commandVector.push_back(word);
        }
        if (commandVector[0] == "quit") {
            cout << "Exiting client" << endl;
            break;
        }
        command_handler.execute(commandVector);
    }
    return 0;
}
