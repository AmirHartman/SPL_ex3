#pragma once

#include "StompProtocol.h"
#include "SocketReader.h"
#include "event.h"

class CommandsHandler {
public:
    CommandsHandler();
    ~CommandsHandler();
    
    void execute(vector<string> &command);
    
private:
    bool connected;
    StompProtocol stomp;
    SocketReader reader;
};