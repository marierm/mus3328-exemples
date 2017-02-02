import processing.serial.*;

//messenger******************************
String message, messageReady;
//***************************************
//***************************************


Serial myPort;

void setup() {
  size(200, 200);
  myPort = new Serial(this, Serial.list()[3], 115200);
}

void draw() {
  if((messageBuild() > 0)) {
    String mess = messageReady;
    int[] q = int(split(mess, ' '));
    
    String messOut = str(q[0] >> 2) + " " + str(abs(q[0] - 1023) >> 2) + " " + str(q[1] / 4);
    
    //write back to duino
    for(int i=0; i<messOut.length(); i++) {
      myPort.write(int(messOut.charAt(i)));
    } 
    myPort.write(13);
 }
}

//messenger******************************
int messageBuild() {
  int size = 0;
  while (myPort.available() > 0) {
    int incoming = myPort.read();
    switch (incoming) {
      case 10:
        break;
      case 13:
        size = 1;
        messageReady = message;
        message = "";
        break;
    default:
      message = message + char(incoming);
    }
  }
  return size;
}
//***************************************
//***************************************