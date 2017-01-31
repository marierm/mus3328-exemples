#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ASCIIserial.listDevices(); //check concole output to find the serial device you want
    ASCIIserial.setup("tty.usbmodem1411", 115200); //portname and baudrate
}

//--------------------------------------------------------------
void ofApp::update(){
    //receive messages
    if((ASCIIserial.messageBuild() > 0)) {
        int a = ASCIIserial.messageGetInt();
        int b = ASCIIserial.messageGetInt();
        printf("%i, %i\n", a, b);
        
        string message = ofToString(a) + " " + ofToString(abs(a-1023)) + " " + ofToString(b >> 2) + char(13);
        ASCIIserial.send(message);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
