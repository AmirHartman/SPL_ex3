#include "../include/StompProtocol.h"
#include "../include/CommandsHandler.h"
#include "../include/StompClient.h"

#include <thread>
#include <queue>
#include <iostream>
#include <sstream>


bool DEBUG_MODE = false;

atomic<bool> should_terminate(false);
// atomic<bool> is_connected(false);
mutex screen_access;
condition_variable cv;



// int main(int argc, char *argv[]) {
//     StompProtocol stomp;
//     CommandsHandler command_handler(stomp);
//     unique_ptr<StompClient> client;
    
//     // Check if the client should run in debug mode
//     if (argc > 1) {
//         if (string(argv[1]) == "debug" && argc == 2) {
//             DEBUG_MODE = true;
//             client.reset(new AdminClient(command_handler, stomp));
//         } else {
//             cout << "Invalid arguments" << endl;
//             return 1;
//         }
//     } else {
//         client.reset(new UserClient(command_handler, stomp));
//     }

//     thread reading_thread([&stomp](){
//         screen_access.try_lock();
//         if (DEBUG_MODE) cout << "[DEBUG] Starting the socket reader thread." << endl;
//         stomp.in.start_reading();
//         screen_access.unlock();
//     });

//     // Start the client on the main thread
//     client->run();
    
//     // Make sure to logout before exiting. If the client is already logged out, will skip the logout command.

//     reading_thread.join();
//     return 0;
// }

int main(int argc, char *argv[]) {
    StompProtocol stomp;
    CommandsHandler command_handler(stomp);
    string host = "127.0.0.1";
    short port = 7777;
    string username = "admin";
    string password = "123";
    stomp.out.connect(host, port, username, password);

    thread reading_thread([&stomp](){
        DEBUG_MODE = true;
        screen_access.try_lock();
        if (DEBUG_MODE) cout << "[DEBUG] Starting the socket reader thread." << endl;
        stomp.in.start_reading();
        screen_access.unlock();
    });
    cout << "Sleeping..." << endl;
    this_thread::sleep_for(chrono::seconds(3));
    cout << "Waking up..." << endl;
    cout << "Exiting..." << endl;
    stomp.closeConnection();
    reading_thread.join();
    return 0;
}