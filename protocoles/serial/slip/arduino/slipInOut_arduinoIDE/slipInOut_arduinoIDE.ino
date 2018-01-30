// Variables globales

// Ces variables servent à l'encodage en SLIP.
const byte END=192;
const byte ESC=219; 
const byte ESC_END=220;
const byte ESC_ESC=221;

const int sensorPin = 0; //L'entrée analogique utilisée.
const int outPins[] = { 5, 10, 11 }; // Les pins utilisées en sortie (PWM).

// La taille maximum d'un paquet SLIP.  Ce nombre doit être plus grande que le
// paquet le plus grand attendu.
byte slipPacket[256]; // Cet array contiendra le paquet SLIP reçu.
int packetIndex = 0;  // L'index du paquet SLIP reçu.  Cette valeur sera
                      // incrémentée à chaque octet reçu.  Elle sera remise à
                      // zéro quand un paquet SLIP sera complet.

// Deux témoins (flags)
boolean escape = false; // Ce témoin devient vrai quand un octet ESC
			// est reçu.  Il redevient faux quand l'octet
			// suivant (ESC_ESC ou ESC_END) est reçu.
boolean packetComplete = false; // Ce témoin devient vrai quand un
				// paquet SLIP est complet,
				// c'est-à-dire quand l'octet END est
				// reçu.

unsigned long previousMillis = 0;
const long interval = 3;

void setup() {
  Serial.begin(115200);
  for( int i=0 ; i < (sizeof(outPins)/sizeof(int)) ; i++) {
    pinMode(outPins[i], OUTPUT); // Les outPins sont initialisées en OUTPUT.
  }
}

void loop() {
  // Réception des paquets SLIP
  int sensor = 0;
  unsigned long currentMillis = millis();
  while (Serial.available()) {
    byte b = Serial.read();  // On lit un premier octet.
    if (escape) { // Si l'octet précédent étant ESC (219)
      if (b == ESC_END) {  // On ajoute END (192)...
	slipPacket[packetIndex] = END;
      } else if (b == ESC_ESC) { // ...ou ESC (219)
	  slipPacket[packetIndex] = ESC;
      }
      packetIndex++;  
      escape = false; // On remet le témoin escape à faux.
    } else if (b == END) { // Le paquet est terminé.
      packetComplete = true;  // Le témoin packetComplete est maintenant vrai.
    } else if (b == ESC) { // Si l'octet est ESC (219)
      escape = true; // Le témoin escape est maintenant vrai. (On n'incrémente pas packetIndex.)
    } else { // Cas normal.
      slipPacket[packetIndex] = b; // On ajoute l'octet au paquet et
      packetIndex ++; // on incrémente packetIndex.
    }
  }

  if (packetComplete) { // On vérifie si le paquet SLIP est complet.
    for (int i=0; i < packetIndex; i++) { 
      analogWrite(outPins[i], slipPacket[i]);
    }
    packetComplete = false;
    packetIndex = 0;
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // On lit le capteur et on envoie un paquet SLIP.
    sensor = analogRead(sensorPin);
    Serial.write(END);   // On commence le paquet.
    SLIPSerialWrite(sensor >> 8);
    SLIPSerialWrite(sensor & 255);
    Serial.write(END);   // On finit le paquet.
  }
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
