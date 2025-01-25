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

static void testClientFrames(){
    MessageEncoderDecoder encdec;
    cout << "MessageEncoderDecoder object created\n" << endl;

    string output = "";

    // // Testing generateConnectFrame
    // cout << "________________Connect Frame:________________________________________________\n";
    // string host = "localhost";
    // short port = 8080;
    // string username = "Amir";
    // string password = "1234";

    // output = "Frame info:\n";
    // output += "Host: " + host + "\n";
    // output += "Port: " + to_string(port) + "\n";
    // output += "Username: " + username + "\n";
    // output += "Password: " + password + "\n\n";
    // cout << output;

    // Frame connectFrame = encdec.generateConnectFrame(host, port, username, password);
    // printFrame(connectFrame.toString());
    // cout << "________________________________________________________________________________\n\n\n\n";

    // Testing generateSendFrame
    cout << "________________Send Frame:____________________________________________________\n";
    string topic = "MyTopic";
    string message = "Hello my name is Amir";

    output = "Frame info:\n";
    output += "Topic: " + topic + "\n";
    output += "Message: " + message + "\n\n";
    cout << output;

    Frame sendFrame = encdec.generateSendFrame(topic, message);
    printFrame(sendFrame.toString());

    cout << "\nGenerating second SEND frame to check if receipt id is different" << endl;
    Frame sendFrame2 = encdec.generateSendFrame("SecondTopic" ,"Hello my name is Yam");
    printFrame(sendFrame2.toString());
    cout << "________________________________________________________________________________\n\n\n\n";

    // // Testing generateSubscribeFrame
    // cout << "________________Subscribe Frame:______________________________________________\n";
    // output = "Frame info:\n";
    // output += "Topic: " + topic + "\n\n";
    // cout << output;
    // Frame subscribeFrame = encdec.generateSubscribeFrame(topic);
    // printFrame(subscribeFrame.toString());

    // cout << "\nGenerating second SUBSCRIBE frame to check if subscription id is different" << endl;
    // Frame subscribeFrame2 = encdec.generateSubscribeFrame("SecondTopic");
    // printFrame(subscribeFrame2.toString());
    // cout << "________________________________________________________________________________\n\n\n\n";

    // // Testing generateUnsubscribeFrame
    // cout << "________________Unsubscribe Frame:____________________________________________\n";
    // cout << "Unsubscribing from both topics in reverse order to check if the subscription ids match" << endl;
    // cout << "\nUnsubscribing from topic: SecondTopic\n" << endl;
    // Frame unsubscribeFrame2 = encdec.generateUnsubscribeFrame("SecondTopic");
    // printFrame(unsubscribeFrame2.toString());
    // cout << "Unsubscribing from topic: \n" << topic << endl;
    // Frame unsubscribeFrame = encdec.generateUnsubscribeFrame(topic);
    // printFrame(unsubscribeFrame.toString());
    // cout << "________________________________________________________________________________\n\n\n\n";

    // // Testing generateDisconnectFrame
    // cout << "________________Disconnect Frame:_____________________________________________\n";
    // Frame disconnectFrame = encdec.generateDisconnectFrame();
    // printFrame(disconnectFrame.toString());
    // cout << "________________________________________________________________________________\n\n\n\n";
}

static void testServerFrames(){

}

int main() {
    cout << "MessageEncoderDecoder Tester\n" << endl;
    cout << "Which frames would you like to test?\n1. Client frames\n2. Server frames\n3. Both\n" << endl;
    int choice=1;
    // cin >> choice;
    switch (choice)
    {
    case 1:
        testClientFrames();
        break;
    case 2:
        testServerFrames();
        break;
    case 3:
        testClientFrames();
        testServerFrames();
        break;
    default:
        cout << "Invalid choice. Closing tester" << endl;
        break;
    }
	return 0;
}

