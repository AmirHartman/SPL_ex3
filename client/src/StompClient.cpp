#include <iostream>
#include "../include/CommandsHandler.h"

int main() {

    // std::string host = "127.0.0.1";
    // short port = 7777;
    cout << "Starting client" << endl;
    CommandsHandler commandsHandler;
    cout << "Client started" << endl;
    cout << "Commands available: login, join, exit, report, summary, logout" << endl;
    cout << "To exit the client, type 'quit'" << endl;
    cout << "Please enter a command" << endl;
    string command;
    while (getline(cin, command)) {
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
        commandsHandler.runCommand(commandVector);
    }
}