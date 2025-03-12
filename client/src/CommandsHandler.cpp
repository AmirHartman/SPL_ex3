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
        cout << "No command was entered" << endl;
        return;
    }

    string command = args[0];
    // check if command exists
    if (!(command == "login" || command == "join" || command == "exit" || command == "report" || command == "summary" || command == "logout")) {
        cout << "Command not found" << endl;
        cout << "Available commands: login, join, exit, report, summary, logout" << endl;
        return;
    }

    // make sure that user is logged in before executing any command
    if (command != "login" && !connected) {
        // if (DEBUG_MODE) cout << "[DEBUG] Tried to run the command " << command << " without being logged in.\nstopping the command execution\n" << endl;
        if (command == "logout") {
            cout << "You are not logged in." << endl;
        } else {
            cout << "You must login first." << endl;
        }
        return;
    }

    //__________________________ LOGIN __________________________
    if (command == "login") {
        if (connected) {
            cout << "The client is already logged in, log out before trying again" << endl;
            return;
        }
        if (args.size() != 4) {
            cout << "Usage: login {host} {port} {username} {password}" << endl;
            return;
        } 
        vector<string> host_port = parseStringByDelimeter(args[1], ':');
        string host = host_port[0];
        short port = stoi(host_port[1]);
        string username = args[2];
        string password = args[3];
        if (stomp.out.connect(host, port, username, password)) {
            connected = true;
            cout << "Login successful." << endl;
            reader.start();
        }

    }

    //__________________________ JOIN __________________________
    if (command == "join") {
        if (args.size() != 2) {
            cout << "Usage: join {channelName}" << endl;
            return;
        }
        cout << "Joining channel \"" << args[1] << "\"..." << endl << endl;
        stomp.out.join(args[1]);
    }

    //__________________________ EXIT __________________________
    if (command == "exit") {
        if (args.size() != 2) {
            cout << "Usage: exit {channelName}" << endl;
            return;
        }
        cout << "Exiting channel \"" << args[1] << "\"..." << endl << endl;
        stomp.out.exit(args[1]);
    }

    //__________________________ REPORT __________________________
    if (command == "report") {
        if (args.size() != 2) {
            cout << "Usage: report {file_name}\n" << endl;
            return;
        }

        names_and_events names_and_events;

        try {
            names_and_events = parseEventsFile(args[1]);
            stomp.out.report(names_and_events);
            cout << "reported" << endl;
            } catch (const std::exception& e) {
            cerr << "Failed to parse the events file." << endl;
            }

    }

    // __________________________ SUMMARY __________________________
    if (command == "summary") {
        cout << "test out 1" << endl;
        if (args.size() != 4) {
            cout << "Usage: summary {channel_name} {name} {file_name}" << endl;
            return;
        }
        cout << "test out 2" << endl;
        stomp.out.summary(args[1], args[2], args[3]);
        cout << "test out 3" << endl;

    }

    // __________________________ LOGOUT __________________________
    if (command == "logout") {
        if (args.size() != 1) {
            cout << "Usage: logout" << endl;
            return;
        }
        
        cout << "Logging out..." << endl;
        stomp.out.logout();
        cout << "Loggedout" << endl;
        connected = false;
    }
    
}





///////////////////////////////////////////////////////////////////
///////////////////////////////AMIR////////////////////////////////
///////////////////////////////////////////////////////////////////

// vector<string> CommandsHandler::handle_file_path(string &input_string) {
//     vector<string> output = {};

//     // path handling
//     vector<string> fileStringSplitted = parseStringByDelimeter(input_string, '/');
//     while (fileStringSplitted.size() > 3) {
//         fileStringSplitted.erase(fileStringSplitted.begin());
//     }
//     if (fileStringSplitted.size() == 3) {
//         if (fileStringSplitted[0] == "client") {
//             fileStringSplitted.erase(fileStringSplitted.begin());
//         } else {
//             return output;
//         }
//     }
//     if (fileStringSplitted.size() == 2) {
//         if (fileStringSplitted[0] == "data") {
//                 fileStringSplitted.erase(fileStringSplitted.begin());
//         } else {
//             return output;
//         }
//     }
//     output.push_back("../data/"); // Relative to the bin folder

//     // name handling
//     if (fileStringSplitted.size() == 1) {
//         string fileName = fileStringSplitted[0];
//         if (fileName.find(".json") == string::npos && fileName.find(".") == string::npos) {
//             fileName += ".json";
//             output.push_back(fileName);
//         }
//     }

//     return output;
// }



// bool CommandsHandler::parse_json_events_file(string &input_string, names_and_events &namesAndEvents) {
//     string path;
//     string file_name;

//     vector<string> file_args= handle_file_path(input_string);
//     if (file_args.empty()) {
//         cout << "The file must be in the directory \"{program_folder}/clients/data/\"." << endl;
//         return false;
//     } else {
//         path = file_args[0];
//     }
//     if (file_args.size() == 1) {
//         cout << "The file must be a .json file." << endl;
//         return false;
//     } else {
//         path += file_args[1];
//     }

//     fstream file(path);
//     if (!file.good()) {
//         cout << "Failed to open the file: \"" << input_string << "\"." << endl;
//         cout << "Please make sure the file exists in the directory \"{program_folder}/clients/data/\" and try again." << endl;
//         return false;
//     }

//     try {
//         namesAndEvents = parseEventsFile(path);
//         file.close();
//     } 
//     catch (const std::exception& e) {
//         cerr << "Failed to parse the events file." << endl;
//         if (DEBUG_MODE) {
//             cout << "\n[DEBUG] Exception message: " << e.what() << endl;
//             cerr << e.what() << endl << endl;
//         }
//         return false;
//     }

//     if (namesAndEvents.events.empty()) {
//         cout << "The events file is empty." << endl;
//         return false;
//     }

//     return true;
// }