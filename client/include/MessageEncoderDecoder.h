#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

// enum class for the server frames
enum ServerFrameType{
    CONNECTED,
    MESSAGE,
    RECEIPT,
    ERROR
};

class MessageEncoderDecoder{
    public:
        // Constructor
        MessageEncoderDecoder();

        // Encode methods
        string generateConnectFrame(const string &host,short port,const string &username, const std::string &password);
        string generateSendFrame(const string &topic, const string &message);
        string generateSubscribeFrame(const string &topic);
        string generateUnsubscribeFrame(const string &topic);
        string generateDisconnectFrame();

        // Decode methods
        bool decodeFrame(const string &frame, unsigned int &sentRecieptId); // TODO : implement
        
    private:
        // id counters for generating unique ids
        unsigned int subscriptionId;
        unsigned int receiptId;

        // map of topics and their subscription ids (make it easier and more natural for users)
        map<string, unsigned int> topicSubscriptionMap;

        // map for the server frames to convert string to enum - could implement without it but it makes the code more readable
        map<string, ServerFrameType> serverFramesMap = {
            {"CONNECTED", CONNECTED},
            {"MESSAGE", MESSAGE},
            {"RECEIPT", RECEIPT},
            {"ERROR", ERROR}
        };

        // generators for subscription and recipt ids
        unsigned int generateSubscriptionId();
        unsigned int generateReciptId();

        // private decode methods (auxiliary methods)
        string decodeAndPrintMessageFrame(vector<vector<string>> &frameArgs); // TODO : implement
        string decodeErrorFrame(vector<vector<string>> &frameArgs); // TODO : implement
        ServerFrameType getServerMessageType(const string &frame);
        vector<string> parseStringByDelimeter(const string &frame, char delimeter);
        vector<vector<string>> parseFrameByArgs(const string &frame);
        bool checkRecieptId(unsigned int &sentRecieptId, unsigned int &receivedRecieptId); // TODO : implement
};