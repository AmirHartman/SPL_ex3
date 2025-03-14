#pragma once

#include "StompProtocol.h"
#include <condition_variable>
#include <thread>

extern mutex screen_access;
extern atomic<bool> should_terminate;

class SocketReader {
public:
    SocketReader(StompProtocol& stomp, bool& connected);
    ~SocketReader();
    void start();
    void stop();

private:
    void read_loop();

    StompProtocol& stomp;
    thread reading_thread;
    bool& connected;
    bool continue_reading = true;
};