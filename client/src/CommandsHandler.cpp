#include "../include/CommandsHandler.h"

vector<string> parseStringByDelimeter(const string &frame, char delimiter){
    vector<string> parsedString;
    stringstream stream(frame);
    string token;
    while (getline(stream, token, delimiter)) {
        parsedString.push_back(token);
    }
    return parsedString;
}
    

CommandsHandler::CommandsHandler() : connected(false), stomp(), reader(stomp,connected) {}
CommandsHandler::~CommandsHandler() {
    if (connected) {
        stomp.out.logout();
    }
}

void CommandsHandler::execute(vector<string> &args) {
    // _____________General input check_____________
    if (args.empty()) {
        return;
    }

    string command = args[0];
    // check if command exists
    if (!(command == "login" || command == "join" || command == "exit" || command == "report" || command == "summary" || command == "logout")) {
        screen_access.try_lock();
        cout << "Illegal command, please try a different one" << endl;
        screen_access.unlock();
        return;
    }

    // make sure that user is logged in before executing any command
    if (command != "login" && !connected) {
        // if (DEBUG_MODE) cout << "[DEBUG] Tried to run the command " << command << " without being logged in.\nstopping the command execution\n" << endl;
        screen_access.try_lock();
        cout << "Please login first." << endl;
        screen_access.unlock();
        return;
    }

    //__________________________ LOGIN __________________________
    if (command == "login") {
        if (connected) {
            screen_access.try_lock();
            cout << "The client is already logged in, log out before trying again" << endl;
            screen_access.unlock();
            return;
        }
        if (args.size() != 4) {
            screen_access.try_lock();
            cout << "login command needs 3 args:{host:port} {username} {password}" << endl;
            screen_access.unlock();
            return;
        } 
        vector<string> host_port = parseStringByDelimeter(args[1], ':');
        string host = host_port[0];
        short port = stoi(host_port[1]);
        string username = args[2];
        string password = args[3];
        if (stomp.out.connect(host, port, username, password)) {
            connected = true;
            screen_access.try_lock();
            cout << "Login successful" << endl;
            screen_access.unlock();
            reader.start();
        }

    }

    //__________________________ JOIN __________________________
    if (command == "join") {
        if (args.size() != 2) {
            screen_access.try_lock();
            cout << "join command needs 1 args:{channel_name}" << endl;
            screen_access.unlock();
            return;
        }
        stomp.out.join(args[1]);
    }

    //__________________________ EXIT __________________________
    if (command == "exit") {
        if (args.size() != 2) {
            screen_access.try_lock();
            cout << "exit command needs 1 args:{channel_name}" << endl;
            screen_access.unlock();
            return;
        }
        stomp.out.exit(args[1]);
    }

    //__________________________ REPORT __________________________
    if (command == "report") {
        if (args.size() != 2) {
            screen_access.try_lock();
            cout << "report command needs 1 args:{file}" << endl;
            screen_access.unlock();
            return;
        }

        names_and_events names_and_events;

        try {
            names_and_events = parseEventsFile(args[1]);
            stomp.out.report(names_and_events);

            } catch (const std::exception& e) {
                screen_access.try_lock();
                cerr << "Failed to parse the events file." << endl;
                screen_access.unlock();
            }

    }

    // __________________________ SUMMARY __________________________
    if (command == "summary") {
        if (args.size() != 4) {
            screen_access.try_lock();
            cout << "summary command needs 3 args: {channel_name} {user} {file}" << endl;
            screen_access.unlock();
            return;
        }
        stomp.out.summary(args[1], args[2], args[3]);

    }

    // __________________________ LOGOUT __________________________
    if (command == "logout") {
        if (args.size() != 1) {
            screen_access.try_lock();
            cout << "Illegal command, please try a different one" << endl;
            screen_access.unlock();
            return;
        }
        
        stomp.out.logout();
        connected = false;
    }
    
}

