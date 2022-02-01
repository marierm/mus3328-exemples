void setup() {
  Serial.begin(115200); // 9600
}


void loop() {
  int x, y;
  x = analogRead(A0); // 10 bits 0-1023
  y = analogRead(A1); // 10 bits 0-1023
  Serial.write(192);
  SLIPSerialWrite(x >> 8); // MSB
  SLIPSerialWrite(x & 255); // LSB
  SLIPSerialWrite(y >> 8); // MSB
  SLIPSerialWrite(y & 255); // LSB
  delay(10);
}


void SLIPSerialWrite(int val) {
  switch (val) {
    case 192:
      Serial.write(219);
      Serial.write(220);
      break;
    case 219:
      Serial.write(219);
      Serial.write(221);
      break;
    default:
      Serial.write(val);
  }
}
