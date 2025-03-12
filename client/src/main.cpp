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

    string command;
    while (getline(cin, command)) {
        cout << endl;
        vector<string> commandVector;
        stringstream ss(command);
        string word;
        while (ss >> word) {
            commandVector.push_back(word);
        }
        command_handler.execute(commandVector);
    }
    return 0;
}
