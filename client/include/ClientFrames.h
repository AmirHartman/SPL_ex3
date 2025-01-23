#pragma once

#include <string>
#include <map>

class ClientFrames{
    private:
        // id counter for subscriptions
        unsigned int subscriptionId;
        // id counter for recipts
        unsigned int reciptId;
        // map of topics and their subscription ids (make it easier and more natural for users)
        std::map<std::string, unsigned int> topicSubscriptionMap;

    public:
        unsigned int generateSubscriptionId();
        unsigned int generateReciptId();
        std::string getSENDframe(std::string topic, std::string message);
        std::string getSubscribeFrame(std::string topic);
        std::string getUnsubscribeFrame(std::string topic);
        std::string getDisconnectFrame();
};