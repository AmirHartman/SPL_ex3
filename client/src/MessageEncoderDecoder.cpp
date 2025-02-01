#include "../include/MessageEncoderDecoder.h"

<<<<<<< HEAD
MessageEncoderDecoder::MessageEncoderDecoder() : topicSubscriptionMap(map<string, unsigned int>()) {}
    

// Encode methods
/**
 * @brief Generates a CONNECT frame for the STOMP protocol.
 * 
 * @param host The host to connect to.
 * @param port The port to connect to.
 * @param username The username to connect with.
 * @param password The password to connect with.
 * @return CONNECT frame.
 */
Frame MessageEncoderDecoder::generateConnectFrame(const string &host,short &port,const string &username, const string &password){
    map<string, string> headers;
    headers["host"] = host;
    headers["accept-version"] = "1.2";
    headers["login"] = username;
    headers["passcode"] = password;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
    return Frame(FrameType::CONNECT, headers, "");
}

/**
 * @brief Generates a SEND frame for the STOMP protocol.
 * 
 * @param destination The topic to which the message is sent to.
 * @param messageBody The message content to be sent.
 * @return SEND frame.
 */
Frame MessageEncoderDecoder::generateSendFrame(const string &destination, const string &messageBody){
    map<string, string> headers;
    headers["destination"] = "/" + destination;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
=======
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
Frame MessageEncoderDecoder::generateConnectFrame(const string &host,short &port,const string &username, const string &password){
    map<string, string> headers;
    headers["host"] = "stomp.cs.bgu.ac.il";
    headers["accept-version"] = "1.2";
    headers["login"] = username;
    headers["passcode"] = password;
    // headers["receipt"] = to_string(idCounter::generateReceiptId());
    return Frame(FrameType::CONNECT, headers, "");
}

Frame MessageEncoderDecoder::generateSendFrame(const string &destination, const string &messageBody){
    map<string, string> headers;
    headers["destination"] = "/" + destination;
    // headers["destination"] = destination;
>>>>>>> tst

    return Frame(FrameType::SEND, headers, messageBody);
}

<<<<<<< HEAD
/**
 * @brief Generates a SUBSCRIBE frame for the STOMP protocol.
 * 
 * @param topic The topic to subscribe to.
 * @return SUBSCRIBE frame.
 */
Frame MessageEncoderDecoder::generateSubscribeFrame(const string &topic){
    unsigned int subscriptionId = idCounter::generateSubscriptionId();
=======
Frame MessageEncoderDecoder::generateSubscribeFrame(const string &topic){
    int subscriptionId = idCounter::generateSubscriptionId();
>>>>>>> tst
    topicSubscriptionMap[topic] = subscriptionId;

    map<string, string> headers;
    headers["destination"] = "/" + topic;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::SUBSCRIBE, headers, "");
}

<<<<<<< HEAD
/**
 * @brief Generates a UNSUBSCRIBE frame for the STOMP protocol.
 * 
 * @param topic The topic to unsubscribe from.
 * @return UNSUBSCRIBE frame.
 */
Frame MessageEncoderDecoder::generateUnsubscribeFrame(const string &topic){
    unsigned int subscriptionId = topicSubscriptionMap[topic];
=======
Frame MessageEncoderDecoder::generateUnsubscribeFrame(const string &topic){
    
    int subscriptionId = topicSubscriptionMap[topic];
>>>>>>> tst
    topicSubscriptionMap.erase(topic);

    map<string, string> headers;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::UNSUBSCRIBE, headers, "");
}

<<<<<<< HEAD
/**
 * @brief Generates a DISCONNECT frame for the STOMP protocol.
 * 
 * @return DISCONNECT frame.
 */
=======
>>>>>>> tst
Frame MessageEncoderDecoder::generateDisconnectFrame(){
    map<string, string> headers;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
    
    return Frame(FrameType::DISCONNECT, headers, "");
}

<<<<<<< HEAD

// Decode method
/**
 * @brief Decodes a frame received from the server.
 * 
 * @param frame The frame to decode (as a string).
 * @return The decoded Frame struct.
 */
Frame MessageEncoderDecoder::decodeFrame(const string &frame){
    vector<vector<string>> parsedFrameByArgs = parseStringFrameToArgs(frame);

    if (parsedFrameByArgs.size() == 0){
=======
// Decode methods
Frame MessageEncoderDecoder::generateFrameFromString(const string &frame){
    // Parse the frame to a vector of vectors of strings
    vector<vector<string>> frameArgs = parseStringFrameToArgs(frame);

    // Check if the frame is empty
    if (frameArgs.size() == 0){
>>>>>>> tst
        cerr << "Error reading frame. frame is empty!" << endl;
        return Frame();
    }

<<<<<<< HEAD
    FrameType frameType = stringToFrameType(parsedFrameByArgs[0][0]);
    map<string, string> headers;

    switch (frameType)
    {
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
        headers["receipt-id"] = "";
        break;
    case ERROR:
        headers["receipt-id"] = "";
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
        headers["receipt"] = "";
        break;
    case SUBSCRIBE:
        headers["destination"] = "";
        headers["id"] = "";
        headers["receipt"] = "";
        break;
    case UNSUBSCRIBE:
        headers["id"] = "";
        headers["receipt"] = "";
        break;
    case DISCONNECT:
        headers["receipt"] = "";
        break;

    // Frame type indicates an error
    case UNKNOWN:
        cerr << "Could not read frame type" << endl;
        return Frame();
    }
    Frame output = generateFrameFromArgs(parsedFrameByArgs, headers);
    return output;
}


// Private methods
vector<string> MessageEncoderDecoder::parseStringByDelimeter(const string &frame, char delimiter){
    vector<string> parsedString;
    stringstream stream(frame);
    string token;
    while (getline(stream, token, delimiter)) {
        parsedString.push_back(token);
    }
    return parsedString;
}

vector<vector<string>> MessageEncoderDecoder::parseStringFrameToArgs(const string &frame){
    vector<string> parsedFrameByLines = parseStringByDelimeter(frame, '\n');
    vector<vector<string>> parsedFrameByArgs;
    size_t i = 0;
    while (i < parsedFrameByLines.size() && parsedFrameByLines[i] != ""){ // handles headers
        parsedFrameByArgs.push_back(parseStringByDelimeter(parsedFrameByLines[i], ':'));
        i++;
    }
    i++; // skip the empty line
    parsedFrameByArgs.push_back({}); // add an empty line to separate the headers from the message body
    while(i < parsedFrameByLines.size()) { // handles the message body - each line is a string.
        vector<string> messageLine = {parsedFrameByLines[i]};
        parsedFrameByArgs.push_back(messageLine); 
        i++;
    }
    return parsedFrameByArgs;
}

Frame MessageEncoderDecoder::generateFrameFromArgs(vector<vector<string>> &frameArgs, map<string, string> &headers){
    bool argsFound[headers.size()] = {false}; // using bool array to indicate if all args needed are in the frame
    bool allArgsFound = false;
    bool errorFound = false;

    size_t i = 1; // representing line index in the frame. start from 1 to skip the frame type
    while (frameArgs[i].size() > 0 && frameArgs[i][0] != "\n" && !errorFound){ // keeps iterating until reached an error, or a line with only '\n' or an empty line
        if (allArgsFound) {
            cerr << "Frame has too many arguments" << endl;
            errorFound = true;
            break;
        }

        // update the headers map with the values from the frame
        for (const auto &pair : headers) {
            string header = pair.first;
            if (frameArgs[i][0] == header){
                headers[header] = frameArgs[i][1];
                argsFound[i-1] = true;
            }
        }

=======
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
            cerr << "Could not read frame type" << endl;
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
        else if (DEBUG_MODE && headers.find(frameArgs[i][0]) == headers.end()){
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
>>>>>>> tst
        // check if all args were found
        allArgsFound = true;
        for (bool argFound : argsFound){
            allArgsFound = allArgsFound && argFound;
        }

        i++;
    }

<<<<<<< HEAD
=======
    if (DEBUG_MODE && unexpectedHeadersFlag) cout << "[DEBUG] unexpected headers were found in the server frame: " << unexpectedHeaders << endl;

>>>>>>> tst
    if (!allArgsFound) {
        cerr << "Frame is missing arguments" << endl;
        cout << "Missing headers: ";
        for (const auto &pair : headers) {
            string header = pair.first;
            string value = pair.second;
            if (value == ""){
                cout << header << " ";
            }
<<<<<<< HEAD
        cout << endl;
        errorFound = true;
        }
    }

    if (errorFound){
=======
        }
        cout << endl;
>>>>>>> tst
        return Frame();
    }
    int messageStartLineIndex = i+1; // The index of the line where the message body starts
    string messageBody = concatenateMessageBody(frameArgs, messageStartLineIndex);
    return Frame(stringToFrameType(frameArgs[0][0]), headers, messageBody);
}

<<<<<<< HEAD
string MessageEncoderDecoder::concatenateMessageBody(vector<vector<string>> &frameArgs, int messageStartLineIndex){
    string message = "";
    for (size_t i = messageStartLineIndex; i < frameArgs.size(); i++) {
        message += frameArgs[i][0] +"\n";
    }
    return message;
}

bool MessageEncoderDecoder::checkRecieptId(unsigned int &sentRecieptId, unsigned int &receivedRecieptId){
    if (sentRecieptId != receivedRecieptId){
        cerr << "Receipt id from the server mismatch the id sent." << endl;
        cout << "Sent receipt id: " << sentRecieptId << "Received receipt id: " << receivedRecieptId << endl;
        return false;
    }
    return true;
}

=======
vector<string> MessageEncoderDecoder::parseStringByDelimeter(const string &frame, char delimiter){
    vector<string> parsedString;
    stringstream stream(frame);
    string token;
    while (getline(stream, token, delimiter)) {
        parsedString.push_back(token);
    }
    return parsedString;
}
>>>>>>> tst
