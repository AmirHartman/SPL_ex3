#pragma once
#include "frame.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <mutex>

using namespace std;

extern mutex screen_access;

class MessageEncoderDecoder{
  public:
    MessageEncoderDecoder();

    /* map of topics and their subscription ids
        could implement without it but it makes the code more readable.
    */
    map<string,int> topicSubscriptionMap;

    /*
    ______________Encode methods______________
            (client frame generators)
    */
    Frame generateConnectFrame(const string &username, const string &password);
    Frame generateSendFrame(const string &destination, const string &message);
    Frame generateSubscribeFrame(const string &topic);
    Frame generateUnsubscribeFrame(const string &topic);
    Frame generateDisconnectFrame();

    /*
    ______________Decode methods_______________
              (server frame decoder)
    */
    Frame generateFrameFromString(const string &frame);
    
  private:
    // Parsing a frame to a vector of vectors of strings by the delimiters '\n' and ':'
    vector<vector<string>> parseStringFrameToArgs(const string &frame);
    // Concatenating a vector of strings to a single string. Used to concatenate back the message body of a frame.
    string concatenateMessageBody(vector<vector<string>> &frameArgs, int messageStartLineIndex);
    // Parsing a string to a vector of strings by a delimiter
    vector<string> parseStringByDelimeter(const string &frame, char delimiter);
};

