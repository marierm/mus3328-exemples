// Variables globales
const int END=192;
const int ESC=219; 
const int ESC_END=220;
const int ESC_ESC=221;

void setup() {
  Serial.begin(115200); // On va le plus vite possible.
}

void loop() {
  int sensorA;
  int sensorB;
  // Lire les entrées analogiques et les envoyer sur le port série.
  sensorA = analogRead(0);
  sensorB = analogRead(1);

  SLIPSerialWrite(sensorA >> 8 ); // envoyer le MSB de l'octet 1.
  SLIPSerialWrite(sensorA & 255 ); // envoyer le LSB de l'octet 1.
  SLIPSerialWrite(sensorB >> 8 ); // envoyer le MSB de l'octet 2.
  SLIPSerialWrite(sensorB & 255 ); // envoyer le LSB de l'octet 2.

  Serial.write(END);   // On termine le paquet.

  // On attend un peu avant de faire une nouvelle lecture des entrées analogiques.
  delay(3);
}

// Fonction 
void SLIPSerialWrite(int value){
  if(value == END){ 
    Serial.write(ESC);
    Serial.write(ESC_END);
    return;
  } else if(value == ESC) {  
    Serial.write(ESC);
    Serial.write(ESC_ESC);
    return;
  } else {
    Serial.write(value);
    return;
  }
}

