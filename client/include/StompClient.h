#pragma once
#include "StompProtocol.h"

class StompClient {
    public:
        StompClient();
        virtual ~StompClient();
        virtual void run() = 0;
        // void ensureLogout();
    protected:
        CommandsHandler command;
};

class UserClient : public StompClient {
    public:
        virtual void run() override;
};

class AdminClient : public StompClient {
    public:
        virtual void run() override;
    private:
        void clearScreen();
        void adminLogin();
        void runTests();
        void adminCommands();
};