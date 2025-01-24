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
    ERROR,
    UNKNOWN
};

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


        /*
        ______________Decode method_______________
        Using only one public method to decode frames. 
        This method does all the work: decoding. printing output to client if needed.
        Returns false if error was occured during decoding.
        */
        bool decodeFrame(const string &frame, unsigned int &sentRecieptId); // TODO : implement
        

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

        // returns server frame type
        ServerFrameType getServerMessageType(const string &frame);

        // Parsing a string to a vector of strings by a delimiter
        vector<string> parseStringByDelimeter(const string &frame, char delimeter);

        // Parsing a frame to a vector of vectors of strings by the delimiters '\n' and ':'
        vector<vector<string>> parseFrameByArgs(const string &frame);

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
        string concatenateStringVector(vector<string> &frameArgs, int messageStartLineIndex);
};