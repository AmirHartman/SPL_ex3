#include "../../include/StompClient.h"
#include "StompClient.h"

StompClient::StompClient() : stomp(StompProtocol()) {}
StompClient::~StompClient() {}

void StompClient::ensureLogout() {
    if (stomp.isLoggedIn()) {
        vector<string> logout = {"logout"};
        stomp.proccess(logout);
    }
}
