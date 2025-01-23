#include "../../include/ClientFrames.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
	ClientFrames clientFrames();
	cout << "ClientFrames object created\n" << endl;
    string topic = "MyTopic";
    string message = "Hello my name is Amir";
    cout << "Send Frame:\n" << clientFrames.getSENDframe(topic, message) << endl;
	return 0;
}

