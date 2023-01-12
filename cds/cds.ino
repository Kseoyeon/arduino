void setup() {
  Serial.begin(1200);
}
 
void loop() {
  int light = analogRead(A2);
  int ledLight = map(light, 0, 200, 0, 225);
  Serial.print("Light : ");
  Serial.println(ledLight);
  Serial.print("cd : ");
  Serial.println(light);
  analogWrite(9, ledLight);
}
