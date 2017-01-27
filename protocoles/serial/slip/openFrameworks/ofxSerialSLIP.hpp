//
//  ofxSerialSLIP.hpp
//  test-slip
//
//  Created by Patrick Saint-Denis on 3/1/16.
//
//

#ifndef ofxSerialSLIP_hpp
#define ofxSerialSLIP_hpp

#include "ofMain.h"

class ofxSerialSLIP {
public:
    void setup(string portName, int baudRate);
    void listDevices();
    void write(int value);
    int messageBuild();
    void start();
    void end();
    
    const int END=192;
    const int ESC=219;
    const int ESC_END=220;
    const int ESC_ESC=221;
    
    ofSerial serial;
    
    Byte slipPacket[256];
    int packetIndex = 0;
};


#endif /* ofxSerialSLIP_hpp */
