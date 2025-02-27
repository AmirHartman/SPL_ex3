#pragma once

#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"
#include "event.h"

#include <fstream>
#include <boost/lockfree/spsc_queue.hpp>
using namespace std;

extern bool DEBUG_MODE;
extern atomic<bool> should_terminate;
extern mutex screen_access;

class In;
class Out;

class StompProtocol {
public:
    StompProtocol();
    
    bool isLoggedIn();
    void closeConnection();
    
    class Out {
        public:
        Out(StompProtocol& _parent);
        StompProtocol& p;
        
        void connect(string& host, short port, string& username, string& password);
        void join(string& channelName);
        void exit(string& channelName);
        void report(names_and_events& namesAndEvents);
        void summary();
        void logout();
        
        private:
        bool sendFrame(Frame& frame);
    };
    
    class In {
    public:
        In(StompProtocol& _parent);
        void start_reading();
        
    private:
        StompProtocol& p;
        Frame readFrameFromSocket();
        void proccess(Frame &server_answer);
        void proccessReceipt(Frame &server_answer);
    };

    In in;
    Out out;

private:
    unique_ptr<ConnectionHandler> connectionHandler;
    MessageEncoderDecoder encdec;
    friend class In;
    friend class Out;
    map<int, Frame> awaiting_frames_for_receipt;
    atomic<bool> is_connected;
    mutex mtx;
    condition_variable cv;

};

