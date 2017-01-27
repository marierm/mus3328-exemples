int messageBuild();
int messageGetInt();

void setup() { 
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}


void loop(){
  if((messageBuild() > 0)) {
    for(int i=0; i< nPoteau; i++) {
      digitalWrite(i, messageGetInt());
    }
  }
 

}
