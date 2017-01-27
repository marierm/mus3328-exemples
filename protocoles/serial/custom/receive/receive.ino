//message end byte (line feed)
int lf = 10;
//byte array expand as desired
byte myByte[3];
 
void setup() {
  //set baud rate
  Serial.begin(9600);
  //set pinModes
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  //check if something is available on the serial port
  while (Serial.available() > 0) {
    Serial.readBytesUntil(lf, (char *)myByte, sizeof(myByte));
    //validate if message is completed
    if(myByte != NULL) {
      //do something
      analogWrite(3, myByte[0]);
      analogWrite(5, myByte[0]);
      analogWrite(6, myByte[0]);
    }
  }
}
