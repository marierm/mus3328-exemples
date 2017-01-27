#include <SPI.h>
#include <Ethernet.h>

#include <ArdOSC.h>


byte myMac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x37, 0xC2 };
byte myIp[]  = { 10, 10, 10, 1 }; 

int  serverPort  = 10000;
int destPort = 12000;
byte destIp[]  = { 10, 10, 10, 2 };


OSCServer server;
OSCClient client;

OSCMessage global_mes;

int ledPin = 9;
int t;

void setup(){ 

 Ethernet.begin(myMac ,myIp); 
 server.begin(serverPort);
 
 //set callback function
 server.addCallback("/led",&func1);
 
 pinMode(ledPin, OUTPUT);  
 
}


void loop(){
    if(server.aviableCheck()>0){
 //    Serial.println("alive! "); 
  }
  
  analogWrite(9, t);
  
  float sensorValue = analogRead(0);
  
  global_mes.setAddress(destIp,destPort);
  global_mes.beginMessage("/pot");
  global_mes.addArgInt32(123);
  global_mes.addArgFloat(sensorValue);
  global_mes.addArgString("qwerty");
  client.send(&global_mes); 
  global_mes.flush(); //object data clear
}


void func1(OSCMessage *_mes){

  //get 1st argument(int32)
  t = _mes->getArgFloat(0);

}
