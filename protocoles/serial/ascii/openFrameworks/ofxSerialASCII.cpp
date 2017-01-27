//
//  ofxSerialASCII.cpp
//  example-ofxSerialASCII
//
//  Created by Patrick Saint-Denis on 2/29/16.
//
//

#include "ofxSerialASCII.hpp"

void ofxSerialASCII::setup(string portName, int baudRate) {
    serial.setup(portName, baudRate);
}

void ofxSerialASCII::listDevices() {
    serial.listDevices();
}

void ofxSerialASCII::send(string message) {
    unsigned char *buf=new unsigned char[message.size()+1];
    buf[message.size()]=0;
    memcpy(buf,message.c_str(),message.size());
    
    serial.writeBytes(buf, message.size());
}

int ofxSerialASCII::messageBuild() {
    int size = 0;
    messageState = 0;
    while (serial.available() > 0) {
        messageBufferSerialByte = serial.readByte();
        
        switch (messageBufferSerialByte) {
            case 10:
                break;
            case 13:
                size = messageBufferIndex;
                messageBuffer[messageBufferIndex]=0;
                messageBufferIndex=0;
                messageState = 1;
                break;
                
            default:
                messageBuffer[messageBufferIndex]=messageBufferSerialByte;
                messageBufferIndex = messageBufferIndex + 1;
        }
        if (messageBufferIndex >= MESSAGE_BUFFER_LAST_ELEMENT) messageBufferIndex=0;
    }
    return size;
}


int ofxSerialASCII::messageGetInt() {
    if (messageNext()) return atoi(messageBufferWord);
    return 0;
}

char ofxSerialASCII::messageNext() {
    char * temppointer= NULL;
    switch (messageState) {
        case 0:
            return 0;
        case 1:
            temppointer = messageBuffer;
            messageState = 2;
        default:
            messageBufferWord = strtok_r(temppointer," ",&messageBufferLast);
            if (messageBufferWord != NULL) return 1;
    }
    return 0;
}
