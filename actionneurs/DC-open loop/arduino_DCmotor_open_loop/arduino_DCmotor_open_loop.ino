// globales et configuration des GPIO
const int switchPin = 2;   
const int motor1Pin = 3; 
const int motor2Pin = 4;
const int enablePin = 9; 
const int potPin = 0; 

void setup() {
    // configuration des GPIO
    pinMode(switchPin, INPUT); 

    pinMode(motor1Pin, OUTPUT); 
    pinMode(motor2Pin, OUTPUT); 
    pinMode(enablePin, OUTPUT);
    pinMode(ledPin, OUTPUT);
}
  
void loop() {
    // decision logique en fonction du statut de la switch (HIGH ou LOW)
    // pour le sens de la rotation
    if (digitalRead(switchPin) == HIGH) {
      digitalWrite(motor1Pin, LOW);   
      digitalWrite(motor2Pin, HIGH);  
    } 
    else {
      digitalWrite(motor1Pin, HIGH);  
      digitalWrite(motor2Pin, LOW);   
    }
    
    // controle de la vitesse du moteur
    int sensorValue = analogRead(potPin);
    sensorValue = map(sensorValue, 0, 1023, 0, 255);
    analogWrite(enablePin, sensorValue); 
}
