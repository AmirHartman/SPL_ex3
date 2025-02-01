#pragma once

#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"
#include "event.h"
#include <fstream>

extern bool DEBUG_MODE;

class StompProtocol {
    public:
        StompProtocol();
        bool isLoggedIn();
        void proccess(vector<string> &args);

    private:
        unique_ptr<ConnectionHandler> connectionHandler;
        MessageEncoderDecoder encdec;
        Frame communicateServer(Frame& frameToSend);
};
