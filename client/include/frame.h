#pragma once

#include <string>
#include <map>

using namespace std;

// class for generating unique ids
// the ids are static means they are shared between all instances of the class (like a global variable)
struct idCounter { 
    static int subscriptionId;
    static int receiptId;

    static int generateSubscriptionId();
    static int generateReceiptId();
};


enum FrameType{
    // Server frames:
    CONNECTED,
    MESSAGE,
    RECEIPT,
    ERROR,

    // Client frames:
    CONNECT,
    SEND,
    SUBSCRIBE,
    UNSUBSCRIBE,
    DISCONNECT,

    // Unknown frame type (indicating an error)
    UNKNOWN
};

string frameTypeToString(FrameType type);
FrameType stringToFrameType(const string &type);

struct Frame {
    FrameType type;
    map<string, string> headers;
    string body;

    Frame(); // default constructor for unknown frame indicating an error
    Frame(FrameType type, map<string, string> headers, string body);
    string toString();
};
