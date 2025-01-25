#include "../include/MessageEncoderDecoder.h"

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

    return Frame(FrameType::SEND, headers, messageBody);
}

/**
 * @brief Generates a SUBSCRIBE frame for the STOMP protocol.
 * 
 * @param topic The topic to subscribe to.
 * @return SUBSCRIBE frame.
 */
Frame MessageEncoderDecoder::generateSubscribeFrame(const string &topic){
    unsigned int subscriptionId = idCounter::generateSubscriptionId();
    topicSubscriptionMap[topic] = subscriptionId;

    map<string, string> headers;
    headers["destination"] = "/" + topic;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::SUBSCRIBE, headers, "");
}

/**
 * @brief Generates a UNSUBSCRIBE frame for the STOMP protocol.
 * 
 * @param topic The topic to unsubscribe from.
 * @return UNSUBSCRIBE frame.
 */
Frame MessageEncoderDecoder::generateUnsubscribeFrame(const string &topic){
    unsigned int subscriptionId = topicSubscriptionMap[topic];
    topicSubscriptionMap.erase(topic);

    map<string, string> headers;
    headers["id"] = to_string(subscriptionId);
    headers["receipt"] = to_string(idCounter::generateReceiptId());

    return Frame(FrameType::UNSUBSCRIBE, headers, "");
}

/**
 * @brief Generates a DISCONNECT frame for the STOMP protocol.
 * 
 * @return DISCONNECT frame.
 */
Frame MessageEncoderDecoder::generateDisconnectFrame(){
    map<string, string> headers;
    headers["receipt"] = to_string(idCounter::generateReceiptId());
    
    return Frame(FrameType::DISCONNECT, headers, "");
}


// Decode method
/**
 * @brief Decodes a frame received from the server.
 * 
 * @param frame The frame to decode (as a string).
 * @return The decoded Frame struct.
 */
Frame MessageEncoderDecoder::decodeFrame(const string &frame){
    vector<vector<string>> parsedFrameByArgs = parseFrameByArgs(frame);

    if (parsedFrameByArgs.size() == 0){
        cerr << "Error reading frame. frame is empty!" << endl;
        return Frame();
    }

    FrameType frameType = stringToFrameType(parsedFrameByArgs[0][0]);

    if (frameType == FrameType::UNKNOWN){
        cerr << "Could not read frame type" << endl;
        return Frame();
    }

    map<string, string> headers;
    switch (frameType)
    {
    case CONNECTED:
        headers["version"] = "";
        headers["receipt-id"] = "";
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
    }
    return decodeFrameByArgs(parsedFrameByArgs, frameType, headers);
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

vector<vector<string>> MessageEncoderDecoder::parseFrameByArgs(const string &frame){
    vector<string> parsedFrameByLines = parseStringByDelimeter(frame, '\n');
    vector<vector<string>> parsedFrameByArgs;
    for (size_t i = 1; i < parsedFrameByLines.size(); i++)
    {
        parsedFrameByArgs.push_back(parseStringByDelimeter(parsedFrameByLines[i], ':'));
    }
    return parsedFrameByArgs;
}

Frame MessageEncoderDecoder::decodeFrameByArgs(vector<vector<string>> &frameArgs, FrameType &type, map<string, string> &headers){
    bool argsFound[headers.size()] = {false}; // using bool array to indicate if all args needed are in the frame
    bool allArgsFound = false;
    bool errorFound = false;

    size_t i = 1; // representing line index in the frame. start from 1 to skip the frame type
    while (frameArgs[i][0] == "\n" && !errorFound){
        if (allArgsFound) {
            cerr << "Frame has too many arguments" << endl;
            errorFound = true;
            break;
        }

        // update the headers map with the values from the frame
        for (const auto &[header, value] : headers) {
            if (frameArgs[i][0] == header){
                headers[header] = frameArgs[i][1];
                argsFound[i-1] = true;
            }
        }

        // check if all args were found
        allArgsFound = true;
        for (bool argFound : argsFound){
            allArgsFound = allArgsFound && argFound;
        }

        i++;
    }

    if (!allArgsFound) {
        cerr << "Frame is missing arguments" << endl;
        cout << "Missing headers: ";
        for (const auto &[header, value] : headers) {
            if (!argsFound[i-1]){
                cout << header << " ";
            }
        }
        errorFound = true;
    }

    if (errorFound){
        return Frame();
    }

    int messageStartLineIndex = i+1; // The index of the line where the message body starts
    string messageBody = concatenateMessageBody(frameArgs, messageStartLineIndex);

    return Frame(type, headers, messageBody);
}

string MessageEncoderDecoder::concatenateMessageBody(vector<vector<string>> &frameArgs, int messageStartLineIndex){
    string message = "";
    for (size_t i = messageStartLineIndex; i < frameArgs.size(); i++) {
        int numOfArgsInLine = frameArgs[i].size();
        for (size_t j = 0; j < numOfArgsInLine; j++) {
            message += frameArgs[i][j];
            if (j < numOfArgsInLine - 1) // This means that ":" was part of the MessageBody, add it back.
                message += ":";
        }
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

