#include "../../include/StompClient.h"

void UserClient::run() {
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
        stomp.proccess(commandVector);
    }
}