#include "../include/CommandsHandler.h"

class CommandsTest {
    public:
        void testLogin();
};

void CommandsTest::testLogin(){
    cout << "### Testing Login Command: ###\n";
    CommandsHandler commandsHandler;
    vector<string> command = {"login", "127.0.0.1", "8888", "Amir", "123"};
    commandsHandler.runCommand(command);
}