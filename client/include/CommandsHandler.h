#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "ConnectionHandler.h"
#include "ClientFrames.h"

using namespace std;

class CommandsHandler{
    public:
        CommandsHandler();
        void runCommand(vector<string> &command);
        void login(string& host, short port, string& username, string& password);
        void join(string& channelName);
        void exit(string& channelName);
        void report(string& filePath);
        void summary(string& channelName, string& user, string& filePath);
        void logout();

    private:
        unique_ptr<ConnectionHandler> connectionHandler;
        ClientFrames clientFrames;
        
        // enum for the commands - could implement without it but it makes the code more readable
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


