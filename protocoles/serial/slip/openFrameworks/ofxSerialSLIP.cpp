//
//  ofxSerialSLIP.cpp
//  test-slip
//
//  Created by Patrick Saint-Denis on 3/1/16.
//
//

#include "ofxSerialSLIP.hpp"

void ofxSerialSLIP::setup(string portName, int baudRate) {
    serial.setup(portName, baudRate);
}

void ofxSerialSLIP::listDevices() {
    serial.listDevices();
}

void ofxSerialSLIP::start() {
    serial.writeByte(END);
}

void ofxSerialSLIP::end() {
    serial.writeByte(END);
}

void ofxSerialSLIP::write(int value){
    if(value == END){
        serial.writeByte(ESC);
        serial.writeByte(ESC_END);
        return;
    } else if(value == ESC) {
        serial.writeByte(ESC);
        serial.writeByte(ESC_ESC);
        return;
    } else {
        serial.writeByte(value);
        return;
    }
}

int ofxSerialSLIP::messageBuild () {
    if(serial.available() > 0) {
        Byte b;
        int size = 0;
        b = serial.readByte();
        if (b == END) {
            size = packetIndex;
            packetIndex = 0;
            return size;
        } else {
            if ( b == ESC ) {
                b = serial.readByte();
                if (b == ESC_END) {
                    slipPacket[packetIndex] = END;
                } else {
                    if (b == ESC_ESC) {
                        slipPacket[packetIndex] = ESC;
                    }
                }
            } else {
                slipPacket[packetIndex] = b;
            }
            packetIndex++; 
            return 0;
        }
    } else {
        return 0;
    }
}
