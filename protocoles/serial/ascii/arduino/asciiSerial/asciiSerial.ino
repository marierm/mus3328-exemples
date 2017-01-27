/*
This example shows how to send messages encoded as ASCII characters over the serial port. Each character forming a number or a message
is sent as a single byte according to the ASCII standard. More info on ASCII can be found here --> https://fr.wikipedia.org/wiki/American_Standard_Code_for_Information_Interchange 

This means that if one wants to send the number 12345, that number would be sent as 6 different bytes. 
One byte for each character ('1', '2', '3', '4', '5') and one extra byte to tell the message is completed.
In ASCII standard, this line feed termination character is usually 10 (LF) or 13 (CR). 

You add as many arguments as you want. Use the Serial.print and Serial.println methods to send messages as ASCII characters.
Separate each number with a space or a coma. Use the Serial.println and the end of the message to add the message termination 
character or line feed. 

In this example, messages are sent from openFrameworks to Arduino. Once received,
these messages are decoded from ASCII to integers and send back to openFrameworks as ASCII characters once again. 
These messages are then printed to the console. 

oF -- ASCII -->  arduino -- ASCII --> oF --> console output

Code by Patrick Saint-Denis

*/


int lf = 10;
String myString = "";

void setup() { 
  Serial.begin(9600);
}

void loop(){
  while (Serial.available() > 0) {
    Serial.readStringUntil(lf);
    if (myString != NULL) {
      int first_number = Serial.parseInt();
      int second_number = Serial.parseInt();
      int third_number = Serial.parseInt();
    
      //send it back   
      Serial.print(first_number);
      Serial.print(" ");
      Serial.print(second_number);
      Serial.print(" ");
      Serial.println(third_number);
    }
  }
}
