#include "../../include/StompClient.h"

AdminClient::AdminClient(CommandsHandler& _command_handler, StompProtocol& _stomp) : StompClient(_command_handler, _stomp) {}

void AdminClient::clearScreen() {
    system("clear");
    cout << "\n";
}

void AdminClient::adminLogin(){
    vector<string> admin_login = {"login", "127.0.0.1", "7777", "admin", "1234"};
    command_handler.execute(admin_login);
}

void AdminClient::runTests() {
    int userChoice = -1;
    cin >> userChoice;
    while (1) {
        cout << "__________________________" << endl;
        cout << "Choose a test to run:" << endl;
        cout << "1. encoder decoder test" << endl;
        cout << "2. commands test" << endl;
        cout << "3. connection handler test" << endl;
        cout << "4. event test\n" << endl;
        cout << "0. go back to main menu" << endl;
        cout << "Enter a number: ";
        cin >> userChoice;
        cout << "__________________________" << endl;
        clearScreen();

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

void AdminClient::adminCommands() {
    // adminLogin();
    
    int userChoice = -1;
    while (userChoice != 0) {
        cout << "__________________________" << endl;
        cout << "Admin commands" << endl;
        cout << "1. admin auto login -> automate login to 127.0.0.1:7777 as {username:admin, password:1234}" << endl;
        cout << "2. run test" << endl;
        cout << "0. go back" << endl;
        cout << "__________________________" << endl;
        cout << "Enter a number: ";
        cin >> userChoice;
        clearScreen();
        if (userChoice == 1) {
            adminLogin();
            break;
        } else if (userChoice == 2) {
            runTests();
            break;
        } else if (userChoice == 0) {
            break;
        } else {
            cout << "Invalid command" << endl;
        }
    }
}

void AdminClient::run() {

    screen_access.try_lock();
    cout << "Started client in debug mode!" << endl;
    screen_access.unlock();

    int userChoice = -1;
    while (userChoice != 0) {
        this_thread::sleep_for(chrono::milliseconds(100));
        screen_access.try_lock();
        cout << "__________________________" << endl;
        cout << "Please choose a command to execute:" << endl;
        cout << "1. login" << endl;
        cout << "2. join" << endl;
        cout << "3. exit" << endl;
        cout << "4. report" << endl;
        cout << "5. summary" << endl;
        cout << "6. logout\n" << endl;
        cout << "7. admin commands\n" << endl;
        cout << "0. quit" << endl;
        cout << "__________________________" << endl;
        cout << "Enter a number: ";
        screen_access.unlock();
        cin >> userChoice;
        screen_access.try_lock();
        clearScreen();
        
        if (userChoice < 0 || userChoice > 7 || cin.fail()) {
            cout << "Invalid command" << endl;
            cin.clear(); // reset the fail flag
            while (cin.get() != '\n') ; // clear the buffer
            userChoice = -1;
            continue;
        } else if (userChoice == 0) {
            cout << "Exiting client\n\n" << endl;
            break;
        } else if (userChoice == 7) {
            clearScreen();
            adminCommands();
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
                    cout << "Enter file name: ";
                    cin >> args[1];
                    break;
                case 5:
                    args.resize(4);
                    args[0] = "summary";
                    cout << "Enter channel name: ";
                    cin >> args[1];
                    cout << "Enter user: ";
                    cin >> args[2];
                    cout << "Enter file name: ";
                    cin >> args[3];
                    break;
                case 6:
                    args.resize(1);
                    args[0] = "logout";
                    break;
            }
            command_handler.execute(args);
        }
        screen_access.unlock();
    }
}
