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

        Frame communicateServer(Frame& frameToSend);

        bool checkIfLoggedIn();
        void handleUNKNOWNFrame(string &serverAnswerAsString);
        void handleERRORFrame(Frame &errorFrame);
        
};


