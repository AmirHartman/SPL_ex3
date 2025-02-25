#pragma once
#include "StompProtocol.h"
#include "CommandsHandler.h"

extern atomic<bool> should_terminate;
extern mutex screen_access;

class StompClient {
    public:
        StompClient(CommandsHandler& _command_handler, StompProtocol& _stomp);
        virtual ~StompClient();
        virtual void run() = 0;
        void ensureLogout();
    protected:
        CommandsHandler &command_handler;
        StompProtocol &stomp;
};

class UserClient : public StompClient {
    public:
        UserClient(CommandsHandler& _command_handler, StompProtocol& _stomp);
        virtual void run() override;
};

class AdminClient : public StompClient {
    public:
        AdminClient(CommandsHandler& _command_handler, StompProtocol& _stomp);
        virtual void run() override;
    private:
        void clearScreen();
        void adminLogin();
        void runTests();
        void adminCommands();
};