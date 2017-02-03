#include "SparkFunLSM6DS3.h"
#include <SPI.h>
#include <Wire.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
 
const char* ssid     = "Patrick's Wi-Fi Network";
const char* password = "12345678";

 
WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(10,0,1,255); // remote IP of your computer
const unsigned int outPort = 50501; // remote port to receive OSC
const unsigned int localPort = 50502; // local port to listen for OSC packets


OSCErrorCode error;

LSM6DS3 SensorOne( SPI_MODE, 10 );

void setup(){
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

  Serial.begin(115200);
  analogReadResolution(12);
  /* Wire.begin(); */

  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
  //Over-ride default settings if desired
  SensorOne.settings.gyroEnabled = 1;  //Can be 0 or 1
  SensorOne.settings.gyroRange = 2000;   //Max deg/s.  Can be: 125, 245, 500, 1000, 2000
  SensorOne.settings.gyroSampleRate = 1666;   //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666
  SensorOne.settings.gyroBandWidth = 400;  //Hz.  Can be: 50, 100, 200, 400;
  SensorOne.settings.accelEnabled = 1;
  SensorOne.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  SensorOne.settings.accelSampleRate = 13;  //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664, 13330
  SensorOne.settings.accelBandWidth = 400;  //Hz.  Can be: 50, 100, 200, 400;
  SensorOne.settings.fifoModeWord = 0;  //FIFO mode.
  //FIFO mode.  Can be:
  //  0 (Bypass mode, FIFO off)
  //  1 (Stop when full)
  //  3 (Continuous during trigger)
  //  4 (Bypass until trigger)
  //  6 (Continous mode)


  delay(1000);

  if( SensorOne.begin() != 0 )
    {
      Serial.println("Problem starting the sensor with CS @ Pin 10.");
    }
  else
    {
    Serial.println("Sensor with CS @ Pin 10 started.");

  }
}

void loop(){
  OSCMessage message;
  int size = udp.parsePacket();

  if (size) {
    while (size--) {
      message.fill(udp.read());
    }
    if (!message.hasError()) {
      message.dispatch("/led/0", osc_set_led);
      /* message.dispatch("/set_register", osc_set_register); */
    } else {
      error = message.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }


  OSCMessage msg1("/accel");
  msg1.add(int(SensorOne.readRawAccelX()));
  msg1.add(int(SensorOne.readRawAccelY()));
  msg1.add(int(SensorOne.readRawAccelZ()));
  msg1.add(int(SensorOne.readRawGyroX()));
  msg1.add(int(SensorOne.readRawGyroY()));
  msg1.add(int(SensorOne.readRawGyroZ()));
  // msg.add(anal0);
  udp.beginPacket(outIp, outPort);
  msg1.send(udp);
  udp.endPacket();
  msg1.empty();

  OSCMessage msg2("/fsr");
  msg2.add(int(analogRead(0)));
  // msg.add(anal0);
  udp.beginPacket(outIp, outPort);
  msg2.send(udp);
  udp.endPacket();
  msg2.empty();

  // wait a bit.
  delay(1);
}

void osc_set_led(OSCMessage &msg){
	//do something with the OSCMessage...
  if (msg.isInt(0)){
    int brightness = msg.getInt(0);
    analogWrite(11, brightness);
  }
}
