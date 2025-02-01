#include "../include/StompClient.h"
#include <thread>
#include <queue>

bool DEBUG_MODE = false;
queue<vector<string>> commandsQueue; // Shared data

int main(int argc, char *argv[]) {
    unique_ptr<StompClient> client;
    
    // Check if the client should run in debug mode
    if (argc > 1) {
        if (string(argv[1]) == "debug" && argc == 2) {
            DEBUG_MODE = true;
            client.reset(new AdminClient());
        } else {
            cout << "Invalid arguments" << endl;
            return 1;
        }
    } else {
        client.reset(new UserClient());
    }

    // TODO: start keyboard listener on a different thread (for the input loop)


    // Start the client on the main thread
    client->run();
    
    // Make sure to logout before exiting. If the client is already logged out, will skip the logout command.
    client->ensureLogout();
}