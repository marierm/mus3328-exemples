

void setup() { 
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop(){
  while (Serial.available() > 0) {
    int lf = 13;
    String myString = Serial.readStringUntil(lf);
    if (myString != NULL) {
      int first_number = Serial.parseInt();
      int second_number = Serial.parseInt();
      int third_number = Serial.parseInt();
    
      analogWrite(3, first_number >> 2); 
      analogWrite(5, second_number >> 2);
      analogWrite(6, third_number);
    }
  }

  int sensor = analogRead(A0);
  int sensor2 = analogRead(A1);
  Serial.print(sensor);
  Serial.print(" ");
  Serial.println(sensor2);

  //delay(3);
}
