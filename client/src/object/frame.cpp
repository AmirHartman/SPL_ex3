#include "../../include/frame.h"

int idCounter::subscriptionId = 1; // start the count from 1 because 0 is defualt value in c++ (will indicate an error)
int idCounter::receiptId = 1;
int idCounter::generateSubscriptionId() {
    return subscriptionId++;
}
int idCounter::generateReceiptId() {
    return receiptId++;
}


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

FrameType stringToFrameType(const string &type){
    if (type == "CONNECTED"){
        return CONNECTED;
    }
    else if (type == "MESSAGE"){
        return MESSAGE;
    }
    else if (type == "RECEIPT"){
        return RECEIPT;
    }
    else if (type == "ERROR"){
        return ERROR;
    }
    else if (type == "CONNECT"){
        return CONNECT;
    }
    else if (type == "SEND"){
        return SEND;
    }
    else if (type == "SUBSCRIBE"){
        return SUBSCRIBE;
    }
    else if (type == "UNSUBSCRIBE"){
        return UNSUBSCRIBE;
    }
    else if (type == "DISCONNECT"){
        return DISCONNECT;
    }
    else{
        return UNKNOWN;
    }
}


Frame::Frame() : type(UNKNOWN), headers(), body("") {}
Frame::Frame(FrameType type, map<string,string> headers, string body) : type(type),headers(headers), body(body) {}


string Frame::toString(){
    string output = frameTypeToString(type) + "\n";
    for (const auto &pair : headers) {
        string header = pair.first;
        string value = pair.second;
        output += header + ":" + value + "\n";
    }
    output += "\n";
    if (body != "") {
        output += body;
    }
    return output;
}
