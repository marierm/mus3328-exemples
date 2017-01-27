//
//  ofxSerialASCII.hpp
//  example-ofxSerialASCII
//
//  Created by Patrick Saint-Denis on 2/29/16.
//

// This code mainly comes from Thomas Fredericks' messenger library for arduino.
// You can find it here: https://github.com/thomasfredericks/Messenger
//
// It facilitates sending/receiving messages encoded as ASCII characters on the serial port.
// Arguments need to be space separated and messages need to end with a new line character ('\n'). Use
// the serial.print() and the serial.println() in arduino (see in the example).
//
// This addon features less services than Fredericks' original library. It only does the ASCII parsing and translation.



#ifndef ofxSerialASCII_hpp
#define ofxSerialASCII_hpp

#include "ofMain.h"

#define MESSAGE_BUFFER_SIZE 64
#define MESSAGE_BUFFER_LAST_ELEMENT 63

class ofxSerialASCII {
public:
    void setup(string portName, int baudRate);
    void readSerial();
    int messageBuild();
    int messageGetInt();
    char messageNext();
    void listDevices();
    void send(string message);
    
protected:
    ofSerial serial;
    
    char messageState = 0;
    char messageBufferSerialByte;
    int messageBufferIndex = 0;
    char messageBuffer[MESSAGE_BUFFER_SIZE];
    char *messageBufferLast;
    char *messageBufferWord;
    char messageSendState = 0;

};

#endif /* ofxSerialASCII_hpp */
