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
 * @param topic The topic to which the message is sent.
 * @param message The message content to be sent.
 * @return A string representing the SEND frame.
 */
std::string MessageEncoderDecoder::generateSendFrame(const std::string &topic, const std::string &message){
    return "SEND\n"
           "destination:/topic/" + topic + "\n" +
           "receipt:" + std::to_string(MessageEncoderDecoder::generateReciptId()) + "\n" +
           "\n" + 
           message + "\n" + 
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


// Decode methods

/**
 * @brief Decodes a frame received from the server.
 * 
 * @param frame The frame to decode.
 * @return false if the frame is an ERROR frame, or the receipt id received from the server doesn't match the one sent by client.
 */
bool MessageEncoderDecoder::decodeFrame(const string &frame, unsigned int &sentRecieptId){
    vector<vector<string>> parsedFrameByArgs = parseFrameByArgs(frame);
    ServerFrameType frameType = getServerMessageType(parsedFrameByArgs[0][0]);

    switch (frameType)
    {
    case CONNECTED:
        cout << "Connected to server" << endl;
        break;
    case MESSAGE:
        cout << "Message received from the server: " << endl;
        decodeAndPrintMessageFrame(frame);
        break;
    case RECEIPT:
        unsigned int receivedRecieptId = stoi(parsedFrameByArgs[1][1]);
        if (!checkRecieptId(sentRecieptId, receivedRecieptId)){
            return false;
        }
        break;
    case ERROR:
        cerr << "Error received: " << endl;
        decodeErrorFrame(frame);
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

string MessageEncoderDecoder::decodeAndPrintMessageFrame(vector<vector<string>> &frameArgs){
    unsigned int subscriptionId;
    unsigned int messageId;
    string channelName;
    string message;
    for (size_t i = 1; i < frameArgs.size(); i++)
    {
        if (frameArgs[i][0] == "subscription"){
            subscriptionId = stoi(frameArgs[i][1]);
        }
        else if (frameArgs[i][0] == "message-id"){
            messageId = stoi(frameArgs[i][1]);
        }
        else if (frameArgs[i][0] == "destination"){
            channelName = frameArgs[i][1];
        }
    }
}

ServerFrameType MessageEncoderDecoder::getServerMessageType(const string &frame){
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