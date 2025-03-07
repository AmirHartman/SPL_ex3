#include "StompClient.h"

StompClient::StompClient(CommandsHandler& _command) : command_handler(_command) {}
StompClient::~StompClient() {}

void StompClient::close() {}