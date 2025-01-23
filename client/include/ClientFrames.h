#pragma once

#include <string>
#include <map>

class ClientFrames{
    public:
        ClientFrames();
        unsigned int generateSubscriptionId();
        unsigned int generateReciptId();
        std::string generateSendFrame(const std::string &topic, const std::string &message);
        std::string generateSubscribeFrame(const std::string &topic);
        std::string generateUnsubscribeFrame(const std::string &topic);
        std::string generateDisconnectFrame();

        
    private:
        // id counter for subscriptions
        unsigned int subscriptionId;
        // id counter for recipts
        unsigned int receiptId;
        // map of topics and their subscription ids (make it easier and more natural for users)
        std::map<std::string, unsigned int> topicSubscriptionMap;
};