#include "../../include/ClientFrames.h" 
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

static void printFrame(const string &frame){
    cout << "Frame:\n---------------------------------" << endl;
    cout << frame << endl;
    cout << "---------------------------------" << endl;
}

int main() {
	ClientFrames clientFrames;
	cout << "ClientFrames object created\n" << endl;

    // Testing generateSendFrame
    string topic = "MyTopic";
    string message = "Hello my name is Amir";
    cout << "________________Send Frame:________________\nTopic: " << topic << "\nMessage: " << message << "\n" << endl;
    printFrame(clientFrames.generateSendFrame(topic,message));
    cout << "\nGenerating second SEND frame to check if receipt id is different" << endl;
    printFrame(clientFrames.generateSendFrame("SecondTopic" ,"Hello my name is Yam"));

    // Testing generateSubscribeFrame
    cout << "\n\n________________Subscribe Frame:________________\nTopic: " << topic << "\n" << endl;
    printFrame(clientFrames.generateSubscribeFrame(topic));
    cout << "\nGenerating second SUBSCRIBE frame to check if subscription id is different" << endl;
    printFrame(clientFrames.generateSubscribeFrame("SecondTopic"));

    // Testing generateUnsubscribeFrame
    cout << "\n\n________________Unsubscribe Frame:________________\nTopic: " << topic << "\n" << endl;
    cout << "Testing unsubscribe from both topics previousy subscribed to in reverse order to check if the ids match" << endl;
    cout << "Unsubscribing from topic: SecondTopic" << endl;
    printFrame(clientFrames.generateUnsubscribeFrame("SecondTopic"));
    cout << "Unsubscribing from topic: " << topic << endl;
    printFrame(clientFrames.generateUnsubscribeFrame(topic));

    // Testing generateDisconnectFrame
    cout << "\n\n________________Disconnect Frame:________________\n" << endl;
    printFrame(clientFrames.generateDisconnectFrame());
	return 0;
}

