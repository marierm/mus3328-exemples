// Il y a des problèmes dans le code ci-dessous. Vous pouvez vous amuser à déboguer!

// Variables globales
// Ces variables servent à l'encodage en SLIP.
const byte END=192;
const byte ESC=219; 
const byte ESC_END=220;
const byte ESC_ESC=221;

const int echoPin = 5;  // pin connected to Echo Pin in the ultrasonic distance sensor
const int trigPin  = 6; // pin connected to trig Pin in the ultrasonic distance sensor

void setup() {
  Serial.begin(115200);
  pinMode(echoPin, INPUT); //Set the connection pin output mode Echo pin
  pinMode(trigPin, OUTPUT);//Set the connection pin output mode trig pin
}

void loop() {
  int mm = ping();
  Serial.write(END);   // On commence le paquet.
  SLIPSerialWrite( byte(mm >> 8) );  // On envoie le MSB
  SLIPSerialWrite( byte(mm & 255) ); // On envoie le LSB 
  Serial.write(END);   // On termine le paquet.
  delay(5);
}

//Fonction pour encoder les paquets SLIP.
void SLIPSerialWrite(byte value){
  if(value == END) { // Si c'est la valeur 192, on remplace par 219 et 220.
    Serial.write(ESC);
    Serial.write(ESC_END);
    return;
  } else if (value == ESC) {  // Si c'est la valeur 219, on fait suivre un 221.
    Serial.write(ESC);
    Serial.write(ESC_ESC);
    return;
  } else { // On envoie toutes les autres valeurs normalement.
    Serial.write(value);
    return;
  }
}

int ping() 
{ 
   // establish variables for duration of the ping, 
   // and the distance result in inches and centimeters: 
   long duration, mm; // long int (64 bits?)
   // The PING))) is triggered by a HIGH pulse of 2 or more microseconds. 
   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse: 
   // pinMode(trigPin, OUTPUT); 
   digitalWrite(trigPin, LOW); 
   delayMicroseconds(2); 
   digitalWrite(trigPin, HIGH); 
   delayMicroseconds(5); 
   digitalWrite(trigPin, LOW); 

   //pinMode(echoPin, INPUT); 
   duration = pulseIn(echoPin, HIGH); 

   // convert the time into a distance 
   mm = microsecondsToMm(duration); 
   return mm ; 
} 

long microsecondsToMm(long microseconds) 
{ 
   // The speed of sound is 340 m/s or 29 microseconds per centimeter. 
   // The ping travels out and back, so to find the distance of the 
   // object we take half of the distance travelled. 
   return microseconds * 0.17; 
} 
