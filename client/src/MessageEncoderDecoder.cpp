#include "../include/MessageEncoderDecoder.h"

MessageEncoderDecoder::MessageEncoderDecoder() : topicSubscriptionMap(map<string, int>()) {}
    
// _____ Private methods
vector<vector<string>> MessageEncoderDecoder::parseStringFrameToArgs(const string &frame){
    vector<string> parsedFrameByLines = parseStringByDelimeter(frame, '\n');
    vector<vector<string>> frameArgs;
    size_t i = 0;
    while (i < parsedFrameByLines.size() && parsedFrameByLines[i] != ""){ // handles headers
        frameArgs.push_back(parseStringByDelimeter(parsedFrameByLines[i], ':'));
        i++;
    }
    i++; // skip the empty line
    frameArgs.push_back({}); // add an empty line to separate the headers from the message body
    while(i < parsedFrameByLines.size()) { // handles the message body - each line is a string.
        vector<string> messageLine = {parsedFrameByLines[i]};
        frameArgs.push_back(messageLine); 
        i++;
    }
    return frameArgs;
}

string MessageEncoderDecoder::concatenateMessageBody(vector<vector<string>> &frameArgs, int messageStartLineIndex){
    string message = "";
    for (size_t i = messageStartLineIndex; i < frameArgs.size(); i++) {
        message += frameArgs[i][0] +"\n";
    }
    return message;
}


// _____ Public methods
// Encode methods
Frame MessageEncoderDecoder::generateConnectFrame(const string &username, const string &password){
    map<string, string> headers;
    headers["host"] = "stomp.cs.bgu.ac.il";
    headers["accept-version"] = "1.2";
    headers["login"] = username;
    headers["passcode"] = password;
    return Frame(FrameType::CONNECT, headers, "");
}

Frame MessageEncoderDecoder::generateSendFrame(const string &destination, const string &messageBody){
    map<string, string> headers;
    headers["destination"] = "/" + destination;
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::SEND, headers, messageBody);
}

Frame MessageEncoderDecoder::generateSubscribeFrame(const string &topic){
    int subscriptionId = idCounter::generateSubscriptionId();
    topicSubscriptionMap[topic] = subscriptionId;

    map<string, string> headers;
    headers["destination"] = "/" + topic;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::SUBSCRIBE, headers, "");
}

Frame MessageEncoderDecoder::generateUnsubscribeFrame(const string &topic){
    
    int subscriptionId = topicSubscriptionMap[topic];

    map<string, string> headers;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::UNSUBSCRIBE, headers, "");
}

Frame MessageEncoderDecoder::generateDisconnectFrame(){
    map<string, string> headers;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
    
    return Frame(FrameType::DISCONNECT, headers, "");
}

// Decode methods
Frame MessageEncoderDecoder::generateFrameFromString(const string &frame){
    
    // Check if the frame is empty
    if (frame.size() == 0){
        return Frame();
    }
    
    // Parse the frame to a vector of vectors of strings
    vector<vector<string>> frameArgs = parseStringFrameToArgs(frame);

    // Defining the frame type
    FrameType frameType = stringToFrameType(frameArgs[0][0]);

    // Initialize new header map with empty values (different for each frame type) 
    map<string, string> headers;

    switch (frameType) {
        // Server frames:
        case CONNECTED:
            headers["version"] = "";
            break;
        case MESSAGE:
            headers["subscription"] = "";
            headers["message-id"] = "";
            headers["destination"] = "";
            break;
        case RECEIPT:
            break;
        case ERROR:
            headers["message"] = "";
            break;

        // Client frames:
        case CONNECT:
            headers["accept-version"] = "";
            headers["host"] = "";
            headers["login"] = "";
            headers["passcode"] = "";
            break;
        case SEND:
            headers["destination"] = "";
            break;
        case SUBSCRIBE:
            headers["destination"] = "";
            headers["id"] = "";
            break;
        case UNSUBSCRIBE:
            headers["id"] = "";
            break;
        case DISCONNECT:
            break;

        // Frame type indicates an error
        case UNKNOWN:
            screen_access.try_lock();
            cerr << "Could not read frame type" << endl;
            screen_access.unlock();
            return Frame();
    }

    //__________________________________________________________________________________________________________________
    // Update the headers map with the values from the frame args vector
    deque<bool> argsFound(headers.size(), false); // using bool deque (like an array that you can add items to the front) to indicate if all args needed found
    bool allArgsFound = false;
    bool unexpectedHeadersFlag = false;
    string unexpectedHeaders = "Unexpected headers: ";
    size_t i = 1; // representing line index in the frame. start from 1 to skip the frame type

    // keeps iterating the frame lines until reached an error, or a line with only '\n' or an empty line
    while (frameArgs[i].size() > 0 && frameArgs[i][0] != "\n" && frameArgs[i][0] != "\0"){ 

        // if the frame contains a receipt-id or receit header, add it to the headers map and assign the value
        if (frameArgs[i][0] == "receit" || frameArgs[i][0] == "receipt-id"){
            headers[frameArgs[i][0]] = frameArgs[i][1];
            argsFound.push_front(true); // push front so i will be the index of the last arg
        }
        // check if header is unexpected (only in debug mode)
        else if (headers.find(frameArgs[i][0]) == headers.end()){
            unexpectedHeadersFlag = true;
            unexpectedHeaders += frameArgs[i][0] + " ";
        }
        // update the matching header in the headers map with the value from the frame
        else {
            // iterate over the headers map and update the matching header
            for (const auto &pair : headers) {
                string header = pair.first;
                if (frameArgs[i][0] == header){
                    // concatenate the header value
                    for (size_t j = 1; j < frameArgs[i].size(); j++){
                        headers[header] += frameArgs[i][j]; 
                        // add ':' if it was part of the header value
                        if (j < frameArgs[i].size()-1){ 
                            headers[header] += ":";
                        }
                    }
                    argsFound[i-1] = true;
                }
            }
        }
        // check if all args were found
        allArgsFound = true;
        for (bool argFound : argsFound){
            allArgsFound = allArgsFound && argFound;
        }

        i++;
    }

    if (unexpectedHeadersFlag) {
        screen_access.try_lock();
        cout << "unexpected headers were found in the server frame: " << unexpectedHeaders << endl;
        screen_access.unlock();
    }

    if (!allArgsFound) {
        screen_access.try_lock();
        cerr << "Frame is missing arguments" << endl;
        cout << "Missing headers: ";
        for (const auto &pair : headers) {
            string header = pair.first;
            string value = pair.second;
            if (value == ""){
                cout << header << " ";
            }
        }
        cout << endl;
        screen_access.unlock();
        return Frame();
    }
    int messageStartLineIndex = i+1; // The index of the line where the message body starts
    string messageBody = concatenateMessageBody(frameArgs, messageStartLineIndex);
    return Frame(stringToFrameType(frameArgs[0][0]), headers, messageBody);
}

vector<string> MessageEncoderDecoder::parseStringByDelimeter(const string &frame, char delimiter){
    vector<string> parsedString;
    stringstream stream(frame);
    string token;
    while (getline(stream, token, delimiter)) {
        parsedString.push_back(token);
    }
    return parsedString;
}