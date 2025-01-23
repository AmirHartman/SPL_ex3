#include "../include/ClientFrames.h"

/**
 * @class ClientFrames
 * @brief A class to generate various STOMP protocol frames for client communication.
 *
 * This class provides methods to generate SEND, SUBSCRIBE, UNSUBSCRIBE, and DISCONNECT frames
 * for a client to communicate with a STOMP server.
 */



// Constructor
ClientFrames::ClientFrames(): subscriptionId(0), receiptId(0){}

/**
 * @brief returns the current subscriptionId and increasing its value by one. (ready for the next subscription)
 * 
 * @return A unique subscription id.
 */
unsigned int ClientFrames::generateSubscriptionId(){
    return subscriptionId++;
}

/**
 * @brief returns the current reciptId and increasing its value by one. (ready for the next recipt)
 * 
 * @return A unique recipt id.
 */
unsigned int ClientFrames::generateReciptId(){
    return receiptId++;
}

/**
 * @brief 
 * 
 * @param topic The topic to which the message is sent.
 * @param message The message content to be sent.
 * @return A string representing the SEND frame.
 */
std::string ClientFrames::generateSendFrame(const std::string &topic, const std::string &message){
    return "SEND\n"
           "destination:/topic/" + topic + "\n" +
           "receipt:" + std::to_string(ClientFrames::generateReciptId()) + "\n" +
           "\n" + 
           message + "\n" + 
           '\0';
}

/**
 * @brief 
 * 
 * @param topic The topic to subscribe to.
 * @return A string representing the SUBSCRIBE frame.
 */
std::string ClientFrames::generateSubscribeFrame(const std::string &topic){
    unsigned int id = ClientFrames::generateSubscriptionId();
    topicSubscriptionMap[topic] = id;
    return "SUBSCRIBE\n"
           "destination:/topic/" + topic + "\n" +
           "id:" + std::to_string(id) + "\n" +
           "receipt:" + std::to_string(ClientFrames::generateReciptId()) + "\n" +
           "\n" + 
           '\0';
}

/**
 * @brief 
 * 
 * @param topic The topic to unsubscribe from.
 * @return A string representing the UNSUBSCRIBE frame.
 */
std::string ClientFrames::generateUnsubscribeFrame(const std::string &topic){
    unsigned int id = topicSubscriptionMap[topic];
    topicSubscriptionMap.erase(topic);
    return "UNSUBSCRIBE\n"
           "id:" + std::to_string(id) + "\n" +
           "receipt:" + std::to_string(ClientFrames::generateReciptId()) + "\n" +
           "\n" + 
           '\0';
}

/**
 * @brief 
 * 
 * @return A string representing the DISCONNECT frame.
 */
std::string ClientFrames::generateDisconnectFrame(){
    return "DISCONNECT\n"
           "receipt:" + std::to_string(ClientFrames::generateReciptId()) + "\n" +
           "\n" + 
           '\0';
}
