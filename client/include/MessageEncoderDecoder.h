#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include "Frames.h"

using namespace std;

class MessageEncoderDecoder{
    public:
        // Constructor
        MessageEncoderDecoder();

        /*
        ______________Encode methods______________
                    (frame generators)
        */
        string generateConnectFrame(const string &host,short port,const string &username, const std::string &password);
        string generateSendFrame(const string &topic, const string &message);
        string generateSubscribeFrame(const string &topic);
        string generateUnsubscribeFrame(const string &topic);
        string generateDisconnectFrame();


        /*______________Decode methods_______________*/

        // returns server frame type
        ServerFrameType getServerMessageType(const string &frame);

        // TODO: add comment here
        // TODO: remove this method and make the methods it uses public
        bool decodeFrame(const string &frame);

        // Parsing a frame to a vector of vectors of strings by the delimiters '\n' and ':'
        vector<vector<string>> parseFrameByArgs(const string &frame);
        

    private:
        //__________________##### Fields #####__________________
        
        // id counters for generating unique ids
        unsigned int subscriptionId;
        unsigned int receiptId;

        /* 
           ___________________Maps___________________
           could implement without those but it makes the code more readable.
        */

        // map of topics and their subscription ids
        map<string, unsigned int> topicSubscriptionMap;
        // map for the server frames to convert string to enum
        map<string, ServerFrameType> serverFramesMap = {
            {"CONNECTED", CONNECTED},
            {"MESSAGE", MESSAGE},
            {"RECEIPT", RECEIPT},
            {"ERROR", ERROR}
        };


        //__________________##### Methods #####__________________

        // generators for subscription and recipt ids
        unsigned int generateSubscriptionId();
        unsigned int generateReciptId();

        /*
          __________private decode methods__________
                (auxiliary methods for decoding)
        */

        // Parsing a string to a vector of strings by a delimiter
        vector<string> parseStringByDelimeter(const string &frame, char delimeter);

        // Checks if the receipt id received from the server matches the one sent by the client.
        bool checkRecieptId(unsigned int &sentRecieptId, unsigned int &receivedRecieptId);

        // Handling MESSAGE frame received from the server. 
        // Returns false if the frame format is incorrect. 
        // Returns true if the frame was decoded and printed successfully.
        bool decodeAndPrintMessageFrame(vector<vector<string>> &frameArgs); // TODO : implement

        // Handling ERROR frame received from the server. 
        // Returns false if the frame format is incorrect. 
        // Returns true if the frame was decoded and printed successfully.
        bool decodeAndPrintErrorFrame(vector<vector<string>> &frameArgs); // TODO : implement

        // Concatenating a vector of strings to a single string
        // Used to concatenate back the message body of a frame
        string concatenateMessageBody(vector<vector<string>> &frameArgs, int messageStartLineIndex);
};

