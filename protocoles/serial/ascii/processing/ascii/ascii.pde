import processing.serial.*; //import the Serial library
Serial myPort;  //the Serial port object
String val;

void setup() {
  size(200, 200);
  myPort = new Serial(this, Serial.list()[3], 115200);
  myPort.bufferUntil('\n'); 
}

void draw() {
  //we can leave the draw method empty, 
  //because all our programming happens in the serialEvent (see below)
}

void serialEvent( Serial myPort) {
//put the incoming data into a String - 
//the '\n' is our end delimiter indicating the end of a complete packet
val = myPort.readStringUntil('\n');
//make sure our data isn't empty before continuing
 if (val != null) {
  //trim whitespace and formatting characters (like carriage return)
  val = trim(val);
  println(val);
  
  }
}