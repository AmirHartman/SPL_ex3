#include <thread>
#include <queue>

#include "../include/CommandsHandler.h"

bool DEBUG_MODE = false;
queue<vector<string>> commandsQueue; // Shared data

void runTests() {
    int userChoice = -1;
    cin >> userChoice;
    while (1) {
        cout << "__________________________" << endl;
        cout << "Choose a test to run:" << endl;
        cout << "1. encoder decoder test" << endl;
        cout << "2. commands test" << endl;
        cout << "3. connection handler test" << endl;
        cout << "4. event test\n" << endl;
        cout << "0. go back" << endl;
        cout << "Enter a number: ";
        cin >> userChoice;
        cout << "__________________________" << endl;

        // TODO: implement the tests
        switch (userChoice) {
            case 1:
                // commandsTest.testEncoderDecoder();
                break;
            case 2:
                // commandsTest.testCommands();
                break;
            case 3:
                // commandsTest.testConnectionHandler();
                break;
            case 4:
                // commandsTest.testEvent();
                break;
            case 0:
                return;
            default:
                cout << "Invalid test!\n" << endl;
        }
    }
}

bool adminCommands() {
    int userChoice = -1;
    while (userChoice != 0) {
        cout << "__________________________" << endl;
        cout << "Admin commands" << endl;
        cout << "1. admin auto login -> automate login to 127.0.0.1:7777 as {username:admin, password:1234}" << endl;
        cout << "2. run test" << endl;
        cout << "0. go back" << endl;
        cout << "Enter a number: ";
        cin >> userChoice;
        cout << "__________________________" << endl;
        if (userChoice == 1) {
            return true;
        } else if (userChoice == 2) {
            runTests();
        } else if (userChoice == 0) {
            break;
        } else {
            cout << "Invalid command" << endl;
        }
    }
    return false;
}

void runNormalMode() {
    CommandsHandler commandsHandler;

    cout << "Client started\n" << endl;
    cout << "Commands available: login, join, exit, report, summary, logout" << endl;
    cout << "To exit the client, type 'quit'\n" << endl;
    cout << "Please enter a command" << endl;

    string command;
    while (getline(cin, command)) {
        cout << endl;
        vector<string> commandVector;
        stringstream ss(command);
        string word;
        while (ss >> word) {
            commandVector.push_back(word);
        }
        if (commandVector[0] == "quit") {
            cout << "Exiting client" << endl;
            break;
        }
        commandsHandler.runCommand(commandVector);
    }
}

void runDebugMode() {
    CommandsHandler commandsHandler;
    cout << "Started client in debug mode!" << endl;

    int userChoice = -1;
    while (userChoice != 0) {
        cout << "__________________________" << endl;
        cout << "Please choose a command to execute:" << endl;
        cout << "1. login" << endl;
        cout << "2. join" << endl;
        cout << "3. exit" << endl;
        cout << "4. report" << endl;
        cout << "5. summary" << endl;
        cout << "6. logout\n" << endl;
        cout << "7. admin commands\n" << endl;
        cout << "0. quit\n" << endl;
        cout << "Enter a number: ";
        cin >> userChoice;
        cout << "__________________________" << endl;

        if (userChoice < 0 || userChoice > 7) {
            cout << "Invalid command" << endl;
            continue;
        } else if (userChoice == 0) {
            vector<string> logout = {"logout"}; // logout before exiting
            commandsHandler.runCommand(logout);
            cout << "Exiting client\n\n" << endl;
            break;
        } else if (userChoice == 7) {
            bool should_run_admin_login = adminCommands();
            if (should_run_admin_login) {
                vector<string> admin_login = {"login", "127.0.0.1", "7777", "admin", "1234"};
                commandsHandler.runCommand(admin_login);
            }
        } else { // userChoice is between 1 and 6 -> run the command
            vector<string> args;
            switch (userChoice) {
                case 1:
                    args.resize(5);
                    args[0] = "login";
                    cout << "Enter host: ";
                    cin >> args[1];
                    cout << "Enter port: ";
                    cin >> args[2];
                    cout << "Enter username: ";
                    cin >> args[3];
                    cout << "Enter password: ";
                    cin >> args[4];
                    break;
                case 2:
                    args.resize(2);
                    args[0] = "join";
                    cout << "Enter channel name: ";
                    cin >> args[1];
                    break;
                case 3:
                    args.resize(2);
                    args[0] = "exit";
                    cout << "Enter channel name: ";
                    cin >> args[1];
                    break;
                case 4:
                    args.resize(2);
                    args[0] = "report";
                    cout << "Enter file path: ";
                    cin >> args[1];
                    break;
                case 5:
                    args.resize(4);
                    args[0] = "summary";
                    cout << "Enter channel name: ";
                    cin >> args[1];
                    cout << "Enter user: ";
                    cin >> args[2];
                    cout << "Enter file path: ";
                    cin >> args[3];
                    break;
                case 6:
                    args.resize(1);
                    args[0] = "logout";
                    break;
            }
            commandsHandler.runCommand(args);
        }
    }
}

int main(int argc, char *argv[]) {
    // Check if the client should run in debug mode
    if (argc > 1) {
        if (string(argv[1]) == "debug" && argc == 2) {
            DEBUG_MODE = true;
        } else {
            cout << "Invalid arguments" << endl;
            return 1;
        }
    }

    // TODO: start the client on a different thread (for the input loop)
    // Start the client

    if (DEBUG_MODE) {
        runDebugMode();
    } else {
        runNormalMode();
    }
}