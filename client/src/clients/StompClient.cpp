#include "StompClient.h"

StompClient::StompClient(CommandsHandler& _command, StompProtocol& _stomp) : command_handler(_command), stomp(_stomp) {}
StompClient::~StompClient() {}

void StompClient::close() {
    stomp.closeConnection();
    should_terminate = true;
}