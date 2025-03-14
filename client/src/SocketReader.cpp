#include "SocketReader.h"
#include <iostream>

SocketReader::SocketReader(StompProtocol& _stomp, bool& _connected) : stomp(_stomp), reading_thread(), connected(_connected) {}

SocketReader::~SocketReader() {
    stop();
}

void SocketReader::start() {
    continue_reading = true;
    if (reading_thread.joinable()) {
        reading_thread.join();
    }
    reading_thread = thread(&SocketReader::read_loop, this);
}

void SocketReader::stop() {
    continue_reading = false;
    if (reading_thread.joinable()) {
        reading_thread.join();
    }
}

void SocketReader::read_loop() {
    while (continue_reading) {
            Frame answer_frame = stomp.in.read_from_socket();
            if (stomp.in.proccess(answer_frame)) {
                continue_reading = false;
                connected = false;
            }
    }
}


