#pragma once

#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"
#include "event.h"
#include <fstream>

extern bool DEBUG_MODE;

class StompProtocol {
public:
    StompProtocol();

    Frame readFrameFromSocket();
    
    bool isLoggedIn();

    void connect(string& host, short port, string& username, string& password);
    void join(string& channelName);
    void exit(string& channelName);
    void report(names_and_events& namesAndEvents);
    void summary();
    void logout();

    void proccess(vector<string> &args);
private:
    unique_ptr<ConnectionHandler> connectionHandler;
    MessageEncoderDecoder encdec;

    bool sendFrame(Frame& frame);

    map<int, FrameType> receipts;
    
    void closeConnection();

};

