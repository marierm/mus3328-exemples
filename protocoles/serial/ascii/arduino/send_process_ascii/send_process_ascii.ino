void setup() {
  Serial.begin(9600);

}

void loop() {
  int sensor_0 = analogRead(A0);
  int sensor_1 = analogRead(A1);
  int sensor_2 = analogRead(A2);
  
  Serial.print(sensor_0);
  Serial.print(" ");
  Serial.print(sensor_1);
  Serial.print(" ");
  Serial.println(sensor_2);

}
