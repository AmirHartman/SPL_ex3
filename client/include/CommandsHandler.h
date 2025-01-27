#pragma once
#include <iostream>
#include <sstream>
#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"
#include "event.h"

using namespace std;

class CommandsHandler{
    public:
        CommandsHandler();
        void runCommand(vector<string> &command);

    private:
        unique_ptr<ConnectionHandler> connectionHandler;
        MessageEncoderDecoder encdec;
        void login(string& host, short port, string& username, string& password);
        bool join(string& channelName);
        bool exit(string& channelName);
        bool report(string& filePath);
        bool summary(string& channelName, string& user, string& filePath);
        bool logout();

        bool checkIfLoggedIn();
        void handleUNKNOWNFrame(string &answerAsString);
        
        // enum class and a map for the commands - could implement without it but it makes the code more readable
        enum Commands{
            LOGIN,
            JOIN,
            EXIT,
            REPORT,
            SUMMARY,
            LOGOUT
        };
        map<string, Commands> commandsMap = {
            {"login", LOGIN},
            {"join", JOIN},
            {"exit", EXIT},
            {"report", REPORT},
            {"summary", SUMMARY},
            {"logout", LOGOUT}
        };
};


