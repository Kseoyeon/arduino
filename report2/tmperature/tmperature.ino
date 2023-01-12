const int LM35Pin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int adcValue;
  long temp;

  adcValue = analogRead(LM35Pin);
  temp = (adcValue * 500L) / 1024;

  Serial.print(temp);
  Serial.println("℃");

  delay(1000);
}
