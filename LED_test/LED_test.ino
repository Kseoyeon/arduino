int LED[5] = {23,22,19,18,21};
int Sensor[5] = {36,39,34,35,32};

void setup() {
   for(int i = 0; i <= 4 ;i++){
    pinMode(LED[i], OUTPUT);
  }
}

void loop() {
  for(int j = 0; j <= 4; j++){
    digitalWrite(LED[j], LOW);
    delay(1000);
  }
  for(int k = 0; k <= 4; k++){
    digitalWrite(LED[k], HIGH);
    delay(1000);
  }
}
