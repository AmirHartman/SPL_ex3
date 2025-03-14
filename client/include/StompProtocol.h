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
#include <ctime>


using namespace std;
extern mutex events_lock;
extern atomic<bool> should_terminate;
extern mutex screen_access;

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
            void summary(string channel_name, string name, string file_name);
            void logout();
            void addEvent(Event& event);
            
            private:
            bool login(string username, string password);
            bool sendFrame(Frame& frame);
            static string epoch_to_date(int date);
            void update_stats (const Event &event, int& active, int& forces_arrival_at_scene);
            void update_reports (int& counter, string& reports, const Event &event);
            void update_file (string &file_name, string &file_content);
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


