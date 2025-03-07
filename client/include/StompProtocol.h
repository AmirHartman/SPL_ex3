#pragma once

#include "ConnectionHandler.h"
#include "MessageEncoderDecoder.h"
#include "event.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <boost/lockfree/spsc_queue.hpp>
#include <set>
#include <mutex>

using namespace std;
extern mutex events_lock;


extern bool DEBUG_MODE;
extern atomic<bool> should_terminate;
extern mutex screen_access;
extern condition_variable cv;

class In;
class Out;

class StompProtocol {
public:
    StompProtocol();
    
    void closeConnection();
    
    class In {
        public:
            In(StompProtocol& _parent);
            Frame read_from_socket();
            bool proccess(Frame &server_answer);
            
        private:
            StompProtocol& p;
            bool proccessReceipt(Frame &server_answer);

        };

    class Out {
        public:
        Out(StompProtocol& _parent);
        StompProtocol& p;
        
        bool connect(string& host, short port, string username, string password);
        void join(string& channelName);
        void exit(string& channelName);
        void report(names_and_events& namesAndEvents);
        void summary();
        void logout();
        void addEvent(Event& event);
        
        private:
        bool login(string username, string password);
        bool sendFrame(Frame& frame);

    };


    In in;
    Out out;

private:
    friend class In;
    friend class Out;
    unique_ptr<ConnectionHandler> connectionHandler;
    MessageEncoderDecoder encdec;
    map<int, Frame> awaiting_frames_for_receipt;
    // key is channel, value is map of users (key) that sent reports (value).
    map<string, map<string, set<Event, EventComparator>>> events;

};

