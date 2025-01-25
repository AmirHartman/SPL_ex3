#include "../include/Frames.h"

unsigned int idCounter::subscriptionId = 0;
unsigned int idCounter::receiptId = 0;
unsigned int idCounter::generateSubscriptionId() {
    return subscriptionId++;
}
unsigned int idCounter::generateReceiptId() {
    return receiptId++;
}


Frame::Frame(FrameType type) : type(type), body("") {}

string frameTypeToString(FrameType type){
    switch (type){
        // Server frames:
        case CONNECTED:
            return "CONNECTED";
        case MESSAGE:
            return "MESSAGE";
        case RECEIPT:
            return "RECEIPT";
        case ERROR:
            return "ERROR";

        // Client frames:
        case CONNECT:
            return "CONNECT";
        case SEND:
            return "SEND";
        case SUBSCRIBE:
            return "SUBSCRIBE";
        case UNSUBSCRIBE:
            return "UNSUBSCRIBE";
        case DISCONNECT:
            return "DISCONNECT";
        default:
            return "UNKNOWN";
    }
}

string Frame::toString(){
    string output = frameTypeToString(type) + "\n";
    for (const auto &[header, value] : headers) {
        output += header + ":" + value + "\n";
    }
    output += "\n";
    if (body != "") {
        output += body;
    }
    output += '\0';
    return output;
}


// Client frames:
ConnectFrame::ConnectFrame(string &host, short &port, string &username, string &password) : Frame(CONNECT) {
    headers["accept-version"] = "1.2";
    headers["host"] = host;
    headers["login"] = username;
    headers["passcode"] = password;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
}

SendFrame::SendFrame(string &topic, string &messageBody) : Frame(SEND) {
    headers["destination"] = "/topic/" + topic;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
    body = messageBody;
}

SubscribeFrame::SubscribeFrame(string &topic, unsigned int subscriptionId) : Frame(SUBSCRIBE) {
    headers["destination"] = "/topic/" + topic;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());
}

UnsubscribeFrame::UnsubscribeFrame(string &topic, unsigned int subscriptionId) : Frame(UNSUBSCRIBE) {
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());
}

DisconnectFrame::DisconnectFrame() : Frame(DISCONNECT) {
    headers["receipt"] = to_string(idCounter::generateReceiptId());
}


// Server frames:
ConnectedFrame::ConnectedFrame() : Frame(CONNECTED) {
    headers["version"] = "1.2";
}

MessageFrame::MessageFrame(unsigned int &subscriptionId, unsigned int &messageId, string &channelName, string& messageBody) : Frame(MESSAGE) {
    headers["subscription"] = to_string(subscriptionId);
    headers["message-id"] = to_string(messageId);
    headers["destination"] = "/topic/" + channelName;
    body = messageBody;
}

ReceiptFrame::ReceiptFrame(unsigned int &receiptId) : Frame(RECEIPT) {
    headers["receipt-id"] = to_string(receiptId);
}

ErrorFrame::ErrorFrame(unsigned int &receiptId, string &errorHeader, string &messageBody) : Frame(ERROR) {
    headers["receipt-id"] = to_string(receiptId);
    headers["message"] = errorHeader;
    body = messageBody;
}