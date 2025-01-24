#include "../include/MessageEncoderDecoder.h"

MessageEncoderDecoder::MessageEncoderDecoder(): subscriptionId(0), receiptId(0){}

// Encode methods
/**
 * @brief Generates a CONNECT frame for the STOMP protocol.
 * 
 * @param host The host to connect to.
 * @param port The port to connect to.
 * @param username The username to connect with.
 * @param password The password to connect with.
 * @return A string representing the CONNECT frame.
 */
std::string MessageEncoderDecoder::generateConnectFrame(const std::string &host,short port,const std::string &username, const std::string &password){
    return "CONNECT\n"
           "accept-version:1.2\n"
           "host:" + host + "\n" +
           "login:" + username + "\n" +
           "passcode:" + password + "\n" +
           "\n" + 
           '\0';
}

/**
 * @brief Generates a SEND frame for the STOMP protocol.
 * 
 * @param topic The topic to which the messageBody is sent.
 * @param messageBody The messageBody content to be sent.
 * @return A string representing the SEND frame.
 */
std::string MessageEncoderDecoder::generateSendFrame(const std::string &topic, const std::string &messageBody){
    return "SEND\n"
           "destination:/topic/" + topic + "\n" +
           "receipt:" + std::to_string(MessageEncoderDecoder::generateReciptId()) + "\n" +
           "\n" + 
           messageBody + "\n" + 
           '\0';
}

/**
 * @brief Generates a SUBSCRIBE frame for the STOMP protocol.
 * 
 * @param topic The topic to subscribe to.
 * @return A string representing the SUBSCRIBE frame.
 */
std::string MessageEncoderDecoder::generateSubscribeFrame(const std::string &topic){
    unsigned int id = MessageEncoderDecoder::generateSubscriptionId();
    topicSubscriptionMap[topic] = id;
    return "SUBSCRIBE\n"
           "destination:/topic/" + topic + "\n" +
           "id:" + std::to_string(id) + "\n" +
           "receipt:" + std::to_string(MessageEncoderDecoder::generateReciptId()) + "\n" +
           "\n" + 
           '\0';
}

/**
 * @brief Generates a UNSUBSCRIBE frame for the STOMP protocol.
 * 
 * @param topic The topic to unsubscribe from.
 * @return A string representing the UNSUBSCRIBE frame.
 */
std::string MessageEncoderDecoder::generateUnsubscribeFrame(const std::string &topic){
    unsigned int id = topicSubscriptionMap[topic];
    topicSubscriptionMap.erase(topic);
    return "UNSUBSCRIBE\n"
           "id:" + std::to_string(id) + "\n" +
           "receipt:" + std::to_string(MessageEncoderDecoder::generateReciptId()) + "\n" +
           "\n" + 
           '\0';
}

/**
 * @brief Generates a DISCONNECT frame for the STOMP protocol.
 * 
 * @return A string representing the DISCONNECT frame.
 */
std::string MessageEncoderDecoder::generateDisconnectFrame(){
    return "DISCONNECT\n"
           "receipt:" + std::to_string(MessageEncoderDecoder::generateReciptId()) + "\n" +
           "\n" + 
           '\0';
}


// Decode method
/**
 * @brief Decodes a frame received from the server.
 * 
 * @param frame The frame to decode.
 * @return false if the frame is an ERROR frame, or the receipt id received from the server doesn't match the one sent by client.
 */
bool MessageEncoderDecoder::decodeFrame(const string &frame, unsigned int &sentRecieptId){
    vector<vector<string>> parsedFrameByArgs = parseFrameByArgs(frame);

    if (parsedFrameByArgs.size() == 0){
        cerr << "Frame is empty" << endl;
        return false;
    }

    ServerFrameType frameType = getServerMessageType(parsedFrameByArgs[0][0]);

    if (frameType == ServerFrameType::UNKNOWN){
        cerr << "Frame type is not recognized" << endl;
        return false;
    }


    switch (frameType)
    {
    case CONNECTED:
        cout << "Connected to server" << endl;
        break;
    case MESSAGE:
        cout << "Message received from the server: " << endl;
        decodeAndPrintMessageFrame(parsedFrameByArgs);
        break;
    case RECEIPT:
        unsigned int receivedRecieptId = stoi(parsedFrameByArgs[1][1]);
        if (!checkRecieptId(sentRecieptId, receivedRecieptId)){
            return false;
        }
        break;
    case ERROR:
        cerr << "Error received from the server: " << endl;
        decodeAndPrintErrorFrame(parsedFrameByArgs);
        return false;
    }
    return true;
}


// Private methods

unsigned int MessageEncoderDecoder::generateSubscriptionId(){
    return subscriptionId++;
}

unsigned int MessageEncoderDecoder::generateReciptId(){
    return receiptId++;
}

ServerFrameType MessageEncoderDecoder::getServerMessageType(const string &frame){
    if (serverFramesMap.find(frame) == serverFramesMap.end())
        return ServerFrameType::UNKNOWN;
    return serverFramesMap[frame];
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

vector<vector<string>> MessageEncoderDecoder::parseFrameByArgs(const string &frame){
    vector<string> parsedFrameByLines = parseStringByDelimeter(frame, '\n');
    vector<vector<string>> parsedFrameByArgs;
    for (size_t i = 1; i < parsedFrameByLines.size(); i++)
    {
        parsedFrameByArgs.push_back(parseStringByDelimeter(parsedFrameByLines[i], ':'));
    }
    return parsedFrameByArgs;
}

bool MessageEncoderDecoder::checkRecieptId(unsigned int &sentRecieptId, unsigned int &receivedRecieptId){
    if (sentRecieptId != receivedRecieptId){
        cerr << "Receipt id from the server mismatch the id sent." << endl;
        cout << "Sent receipt id: " << sentRecieptId << "Received receipt id: " << receivedRecieptId << endl;
        return false;
    }
    return true;
}

bool MessageEncoderDecoder::decodeAndPrintMessageFrame(vector<vector<string>> &frameArgs){ 
    unsigned int subscriptionId;
    unsigned int messageId;
    string channelName;

    int messageStartLineIndex;
    bool argsFound[] = {false, false, false, false};

    size_t i = 1; // start from 1 to skip the frame type
    while (!argsFound[0] && !argsFound[1] && !argsFound[2] && !argsFound[3]) // using bool array to indicate if all args were found
    {
        if (i == frameArgs.size()){ // if reached the end of the frame and not all args were found
            cerr << "Message frame is missing arguments" << endl;
            return false;
        }

        if (frameArgs[i][0] == "subscription"){
            subscriptionId = stoi(frameArgs[i][1]);
            argsFound[0] = true;
        }
        else if (frameArgs[i][0] == "messageBody-id"){
            messageId = stoi(frameArgs[i][1]);
            argsFound[1] = true;
        }
        else if (frameArgs[i][0] == "destination"){
            channelName = parseStringByDelimeter(frameArgs[i][1], '/')[2];
            argsFound[2] = true;
        }
        else if (frameArgs[i].size() == 0){ // indicates the start of the MessageBody (empty line)
            messageStartLineIndex = i+1;
            argsFound[3] = true;
        }
        i++;
    }

    // concatenate back the MessageBody lines
    string messageBody = concatenateMessageBody(frameArgs, messageStartLineIndex);

    string output = "Subscription id: " + to_string(subscriptionId) + "\n" +
                    "Message id: " + to_string(messageId) + "\n" +
                    "Message received from channel: " + channelName + "\n" + 
                    "Message content:\n" + 
                    messageBody;
    cout << output << endl;
    return true;
}

bool MessageEncoderDecoder::decodeAndPrintErrorFrame(vector<vector<string>> &frameArgs){
    unsigned int receiptId;
    string errorMessage;
    
    int messageStartLineIndex;
    bool argsFound[] = {false, false,false};

    size_t i = 1; // start from 1 to skip the frame type
    while (!argsFound[0] && !argsFound[1] && !argsFound[2]) // using bool array to indicate if all args were found
    {
        if (i == frameArgs.size()){ // if reached the end of the frame and not all args were found
            cerr << "Error frame is missing arguments" << endl;
            return false;
        }

        if (frameArgs[i][0] == "receipt"){
            receiptId = stoi(frameArgs[i][1]);
            argsFound[0] = true;
        }
        else if (frameArgs[i][0] == "message"){
            errorMessage = frameArgs[i][1];
            argsFound[1] = true;
        }
        else if (frameArgs[i].size() == 0){ // indicates the start of the MessageBody (empty line)
            messageStartLineIndex = i+1;
            argsFound[2] = true;
        }
        i++;
    }

    // concatenate back the MessageBody lines
    string messageBody = concatenateMessageBody(frameArgs, messageStartLineIndex);

    string output = "Receipt id:" + to_string(receiptId) + "\n" +
                    "Error message:" + errorMessage + "\n" + 
                    "Error content:\n" + 
                    messageBody;
    cout << output << endl;
    return true;
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