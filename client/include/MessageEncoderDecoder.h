#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include "Frames.h"

using namespace std;

class MessageEncoderDecoder{
  public:
    MessageEncoderDecoder();
    /*
    ______________Encode methods______________
            (client frame generators)
    */
    Frame generateConnectFrame(const string &host,short &port,const string &username, const string &password);
    Frame generateSendFrame(const string &destination, const string &message);
    Frame generateSubscribeFrame(const string &topic);
    Frame generateUnsubscribeFrame(const string &topic);
    Frame generateDisconnectFrame();


    /*
    ______________Decode methods_______________
              (server frame decoder)
    */
    Frame decodeFrame(const string &frame);

private:
    /* map of topics and their subscription ids
        could implement without it but it makes the code more readable.
    */
    map<string, unsigned int> topicSubscriptionMap;


    /*
      __________private auxiliary methods__________
    */

    // Parsing a string to a vector of strings by a delimiter
    vector<string> parseStringByDelimeter(const string &frame, char delimeter);

    // Parsing a frame to a vector of vectors of strings by the delimiters '\n' and ':'
    vector<vector<string>> parseFrameByArgs(const string &frame);

    // Concatenating a vector of strings to a single string. Used to concatenate back the message body of a frame.
    string concatenateMessageBody(vector<vector<string>> &frameArgs, int messageStartLineIndex);

    // Decoding a frame by an argument vector.
    Frame decodeFrameByArgs(vector<vector<string>> &frameArgs, FrameType &type, map<string, string> &headers);

    // TODO: check if need to delete this method
    bool checkRecieptId(unsigned int &sentRecieptId, unsigned int &receivedRecieptId);

};

