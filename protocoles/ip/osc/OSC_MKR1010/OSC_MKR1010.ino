
#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>


#define POT 1

int status = WL_IDLE_STATUS;
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
//char ssid[] = "fpTubeRouter-2.4";        // your network SSID (name)
//char pass[] = "=OKI/*$9-W";    // your network password (use for WPA, or use as key for WEP)

const char ssid[] = "WirelessBouc";
const char pass[] = "JuBnu22a";


WiFiUDP udp;
const IPAddress outIp(192,168,1,255); // remote IP of your computer
const unsigned int outPort = 57120; // remote port to receive OSC
const unsigned int localPort = 50505; // local port to listen for OSC packets

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  
  analogReadResolution(12);
  Wire.begin();
  delay(100);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  udp.begin(localPort);
}

void loop() {
  OSCMessage msg("/pot");
  msg.add(int(analogRead(POT)));
  udp.beginPacket(outIp, outPort);
  msg.send(udp);
  udp.endPacket();
  msg.empty();

  // wait a bit.
  delay(10);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
