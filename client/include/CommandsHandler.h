#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"

using namespace std;

class CommandsHandler{
    public:
        CommandsHandler();
        bool runCommand(vector<string> &command);
        bool login(string& host, short port, string& username, string& password);
        bool join(string& channelName);
        bool exit(string& channelName);
        bool report(string& filePath);
        bool summary(string& channelName, string& user, string& filePath);
        bool logout();

    private:
        unique_ptr<ConnectionHandler> connectionHandler;
        MessageEncoderDecoder encdec;
        
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


