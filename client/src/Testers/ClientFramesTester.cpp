#include "../../include/MessageEncoderDecoder.h" 
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
	MessageEncoderDecoder encdec;
	cout << "MessageEncoderDecoder object created\n" << endl;

    // Testing generateConnectFrame
    string host = "localhost";
    short port = 8080;
    string username = "Amir";
    string password = "1234";
    cout << "________________Connect Frame:________________\nHost: " << host << "\nPort: " << port << "\nUsername: " << username << "\nPassword: " << password << "\n" << endl;
    printFrame(encdec.generateConnectFrame(host,port,username,password));

    // Testing generateSendFrame
    string topic = "MyTopic";
    string message = "Hello my name is Amir";
    cout << "________________Send Frame:________________\nTopic: " << topic << "\nMessage: " << message << "\n" << endl;
    printFrame(encdec.generateSendFrame(topic,message));
    cout << "\nGenerating second SEND frame to check if receipt id is different" << endl;
    printFrame(encdec.generateSendFrame("SecondTopic" ,"Hello my name is Yam"));

    // Testing generateSubscribeFrame
    cout << "\n\n________________Subscribe Frame:________________\nTopic: " << topic << "\n" << endl;
    printFrame(encdec.generateSubscribeFrame(topic));
    cout << "\nGenerating second SUBSCRIBE frame to check if subscription id is different" << endl;
    printFrame(encdec.generateSubscribeFrame("SecondTopic"));

    // Testing generateUnsubscribeFrame
    cout << "\n\n________________Unsubscribe Frame:________________\nTopic: " << topic << "\n" << endl;
    cout << "Testing unsubscribe from both topics previousy subscribed to in reverse order to check if the ids match" << endl;
    cout << "Unsubscribing from topic: SecondTopic" << endl;
    printFrame(encdec.generateUnsubscribeFrame("SecondTopic"));
    cout << "Unsubscribing from topic: " << topic << endl;
    printFrame(encdec.generateUnsubscribeFrame(topic));

    // Testing generateDisconnectFrame
    cout << "\n\n________________Disconnect Frame:________________\n" << endl;
    printFrame(encdec.generateDisconnectFrame());
	return 0;
}

