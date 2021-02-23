// Variables globales
// Ces variables servent à l'encodage en SLIP.
const byte END=192;
const byte ESC=219; 
const byte ESC_END=220;
const byte ESC_ESC=221;

byte slipPacket[256]; // Cet array contiendra le paquet SLIP reçu.


int Pin0 = 11;//definition digital 11 pins as pin to control the IN1 (ULN24L01)
int Pin1 = 10;//definition digital 10 pins as pin to control the IN2 (ULN24L01)
int Pin2 = 9;//definition digital 9 pins as pin to control the IN3 (ULN24L01)
int Pin3 = 8;//definition digital 8 pins as pin to control the IN4 (ULN24L01)

int steps[8][4] = {{1, 0, 0, 1}, {1, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}};

int _step = 512; 
int _speed = 1; 
void setup()
{
 Serial.begin(115200);
 pinMode(Pin0, OUTPUT);//Set digital 8 port mode, the OUTPUT for the output
 pinMode(Pin1, OUTPUT);//Set digital 9 port mode, the OUTPUT for the output
 pinMode(Pin2, OUTPUT);//Set digital 10 port mode, the OUTPUT for the output
 pinMode(Pin3, OUTPUT);//Set digital 11 port mode, the OUTPUT for the output
}

void loop()
{
    // Réception des paquets SLIP.
  int packetSize = 0;
  packetSize = SLIPSerialRead( slipPacket );
  // On attend deux items dans le paquet: une vitesse (de 1 à 15) et un nombre de pas (0 à 255).

  if (packetSize > 0) {
    Speed( slipPacket[0] );
    Step( slipPacket[1] );
  };
  delay(10);
}
void Speed(int stepperspeed)//set Stepper speed 
{
    _speed = 15 - stepperspeed;
    if( _speed<1){
     _speed = 1;
    }
    if( _speed>15){
     _speed = 15;
    }
}
void Step(int _step)//Stepper motor rotation
{
  if(_step>=0){  // Stepper motor forward
    for(int i=0;i<_step;i++){   
      setStep(1, 0, 0, 1);
      delay(_speed); 
      setStep(1, 0, 0, 0);
      delay(_speed);
      setStep(1, 1, 0, 0);
      delay(_speed);
      setStep(0, 1, 0, 0);
      delay(_speed);
      setStep(0, 1, 1, 0);
      delay(_speed);
      setStep(0, 0, 1, 0);
      delay(_speed);
      setStep(0, 0, 1, 1);
      delay(_speed); 
      setStep(0, 0, 0, 1);
      delay(_speed);
    }
  }else{ // Stepper motor backward
     for(int i=_step;i<0;i++){  
      setStep(0, 0, 0, 1);
      delay(_speed);
      setStep(0, 0, 1, 1);
      delay(_speed);
      setStep(0, 0, 1, 0);
      delay(_speed);
      setStep(0, 1, 1, 0);
      delay(_speed);
      setStep(0, 1, 0, 0);
      delay(_speed);
      setStep(1, 1, 0, 0);
      delay(_speed);
      setStep(1, 0, 0, 0);
      delay(_speed);
      setStep(1, 0, 0, 1);
      delay(_speed);
    }
   }
}
void setStep(int a, int b, int c, int d)  
{  
    digitalWrite(Pin0, a);     
    digitalWrite(Pin1, b);     
    digitalWrite(Pin2, c);     
    digitalWrite(Pin3, d);     
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
