// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

// Variables globales
// Ces variables servent à l'encodage en SLIP.
const byte END=192;
const byte ESC=219; 
const byte ESC_END=220;
const byte ESC_ESC=221;

byte slipPacket[256]; // Cet array contiendra le paquet SLIP reçu.

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 10 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW);


void setup() {
  Serial.begin(115200);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  // Réception des paquets SLIP.  
  int packetSize = 0;
  packetSize = SLIPSerialRead( slipPacket ); // [ r, g, b, w, r, g, b, w, ..... ]
  // On attend 4 x NUMPIXELS items dans le paquet:
  // R, G, B, W pour chaque pixel.
  if (packetSize > 0) {
    for (int  i=0; i < packetSize ; i=i+4){ // i=0, i=4, i=8, ....
      pixels.setPixelColor(i/4, pixels.Color(slipPacket[i], slipPacket[i+1], slipPacket[i+2], slipPacket[i+3]) );
    };
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(20);
  };
}

// Fonction pour recevoir et décoder les paquets SLIP.
// Retourne la taille du paquet SLIP.
// Prend un paramètre: le tableau (array) d'octets (bytes) qui contiendra le paquet SLIP.
int SLIPSerialRead(byte * slipPacket) { 
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

  // S'il n'y a rien sur le port série, on retourne zéro immédiatement.
  if ( Serial.available() == 0 ) {
    return 0;
  }

  // S'il y a quelque chose, on attend un paquet complet.
  while( !packetComplete ) {
    if( Serial.available() > 0) {
      byte b = Serial.read();
      if (escape) { // Si l'octet précédent était ESC (219)
        if (b == ESC_END) {  // On ajoute END (192)...
          slipPacket[packetIndex] = END;
        } else if (b == ESC_ESC) { // ...ou ESC (219)
          slipPacket[packetIndex] = ESC;
        }
        packetIndex++;  
        escape = 0; // On remet le témoin escape à faux.
      } else if (b == END) { // Le paquet est terminé.
        packetComplete = true;  // Le témoin packetComplete est maintenant vrai.
      } else if (b == ESC) { // Si l'octet est ESC (219)
        escape = 1; // Le témoin escape est maintenant vrai. (On n'incrémente pas packetIndex.)
      } else { // Cas normal.
        slipPacket[packetIndex] = b; // On ajoute l'octet au paquet et
        packetIndex ++; // on incrémente packetIndex.
      }
    }
  }
  // On retourne la taille du paquet.
  return packetIndex;
}
