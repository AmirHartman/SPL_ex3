#include "SocketReader.h"
#include <iostream>

SocketReader::SocketReader(StompProtocol& _stomp, bool& _connected) : stomp(_stomp), should_terminate(false), reading_thread(), connected(_connected) {}

SocketReader::~SocketReader() {
    stop();
}

void SocketReader::start() {
    should_terminate = false;
    if (reading_thread.joinable()) {
        reading_thread.join();
    }
    reading_thread = thread(&SocketReader::read_loop, this);
}

void SocketReader::stop() {
    should_terminate = true;
    if (reading_thread.joinable()) {
        reading_thread.join();
        cout << "Socket reader thread has been terminated." << endl;
    }
}

void SocketReader::read_loop() {
    if (DEBUG_MODE) {
        screen_access.try_lock();
        cout << "[DEBUG] Starting the socket reader thread." << endl;
        screen_access.unlock();
    }
    
    while (!should_terminate) {
            Frame answer_frame = stomp.in.read_from_socket();
            if (stomp.in.proccess(answer_frame)) {
                should_terminate = true;
                connected = false;
            }
    }
    cout << "Ending read loop" << endl;
}


