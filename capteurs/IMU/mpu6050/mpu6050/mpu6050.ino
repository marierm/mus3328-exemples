#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> // Librairie pour utiliser l'i2c


#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
//void SLIPSerialWrite(int value);
//void sendOSC();

//SLIP for wired serial backup.
const byte END=192;
const byte ESC=219;
const byte ESC_END=220;
const byte ESC_ESC=221;

const char* ssid     = "WirelessBouc";
const char* password = "JuBnu22a";

Adafruit_MPU6050 mpu;


WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,11); // remote IP of your computer
const unsigned int destPort = 55222; // remote port to receive OSC
const unsigned int localPort = 55333; // local port to listen for OSC packets

OSCErrorCode error;

void setup(){
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2); // SPI
  Serial.begin(115200);
  

  delay(100);
  // We start by connecting to a WiFi network
 

   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);

  // WiFi.begin(ssid);
  WiFi.begin(ssid,password);
  // WiFi.beginProvision();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.write(".");
  }
 
   Serial.println("");
   Serial.println("WiFi connected");  
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());

   Serial.print("SSID: ");
   Serial.println(WiFi.SSID());
   udp.beginMulti(outIp, localPort);
}

void loop(){
  OSCMessage msg("/mpu6050");
    /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  /* Print out the values */
  Serial.print("Acceleration : ");
  Serial.print(a.acceleration.x);
  Serial.print(", ");
  Serial.print(a.acceleration.y);
  Serial.print(", ");
  Serial.println(a.acceleration.z);

  Serial.print("Rotation: ");
  Serial.print(g.gyro.x);
  Serial.print(", ");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.println(g.gyro.z);
  
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println("");

  msg.add(a.acceleration.x);
  msg.add(a.acceleration.y);
  msg.add(a.acceleration.z);
  msg.add(g.gyro.x);
  msg.add(g.gyro.y);
  msg.add(g.gyro.z);
  msg.add(temp.temperature);
  udp.beginPacket(outIp, destPort);
  msg.send(udp);
  udp.endPacket();
  msg.empty();
  delay(30);
}

void SLIPSerialWrite(int value){
  if(value == END) {
    Serial.write(ESC);
    Serial.write(ESC_END);
    return;
  } else if (value == ESC) {
    Serial.write(ESC);
    Serial.write(ESC_ESC);
    return;
  } else {
    Serial.write(value);
    return;
  }
}
