#include "../include/StompProtocol.h"
#include "../include/CommandsHandler.h"
#include "../include/SocketReader.h"
#include "../include/ConnectionHandler.h"

#include <iostream>
#include <sstream>

mutex screen_access;
mutex events_lock;
atomic<bool> should_terminate(false);

int main(int argc, char *argv[]) {

    CommandsHandler command_handler;

    string command;
    while (!should_terminate && getline(cin, command)) {
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
