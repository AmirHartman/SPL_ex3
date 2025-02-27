#include "StompClient.h"

StompClient::StompClient(CommandsHandler& _command, StompProtocol& _stomp) : command_handler(_command), stomp(_stomp) {}
StompClient::~StompClient() {}

void StompClient::ensureLogout() {
    if (stomp.isLoggedIn()) {
        stomp.out.logout();
    }
    stomp.closeConnection();
}
