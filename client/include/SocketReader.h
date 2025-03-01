#pragma once

#include "StompProtocol.h"
#include <condition_variable>
#include <thread>

extern mutex screen_access;

class SocketReader {
public:
    SocketReader(StompProtocol& stomp, bool& connected);
    ~SocketReader();
    void start();
    void stop();

private:
    void read_loop();

    StompProtocol& stomp;
    bool should_terminate;
    thread reading_thread;
    bool& connected;
};