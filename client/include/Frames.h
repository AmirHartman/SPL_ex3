#pragma once
#include <string>
#include <map>

using namespace std;

// class for generating unique ids
// the ids are static means they are shared between all instances of the class (like a global variable)
struct idCounter { 
    static unsigned int subscriptionId;
    static unsigned int receiptId;

    static unsigned int generateSubscriptionId();
    static unsigned int generateReceiptId();
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

struct Frame {
    FrameType type;
    map<string, string> headers;
    string body;

    Frame(FrameType type);
    string toString();
};


// Client frames:
struct ConnectFrame : Frame {
    ConnectFrame(string &host, short &port, string &username, string &password);
};

struct SendFrame : Frame {
    SendFrame(string &topic, string &messageBody);

};

struct SubscribeFrame : Frame {
    SubscribeFrame(string &topic, unsigned int subscriptionId);
};

struct UnsubscribeFrame : Frame {
    UnsubscribeFrame(string &topic, unsigned int subscriptionId);
};

struct DisconnectFrame : Frame {
    DisconnectFrame();
};


// Server frames:
struct ConnectedFrame : Frame {
    ConnectedFrame();
};

struct MessageFrame : Frame {
    MessageFrame(unsigned int &subscriptionId, unsigned int &messageId, string &channelName, string& messageBody);
};

struct ReceiptFrame : Frame {
    ReceiptFrame(unsigned int &receiptId);
};

struct ErrorFrame : Frame {
    ErrorFrame(unsigned int &receiptId, string &errorHeader, string &messageBody);
};