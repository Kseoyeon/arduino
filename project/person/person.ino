int LED[5] = {23,22,19,18,21};
int sensor[5] = {36,39,34,35,32};
int motion[5];
unsigned long timeCurr;
unsigned long timePrev[5];

void setup() {
  for(int i = 0; i < 5; i++){
    pinMode(sensor[i], INPUT);
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], HIGH);
  }
  Serial.begin(115200);
  for(int i = 0; i < 5; i++){
    timePrev[i] = millis();
  }
}

void loop() {
  timeCurr = millis();

//  for(int i = 0; i < 5; i++){
//    digitalWrite(LED[i], HIGH);
//  }
  
  for(int i = 0; i < 5; i++){
    motion[i] = digitalRead(sensor[i]);
    if (motion[i] == HIGH) {
      digitalWrite(LED[i], LOW);
    }
    else digitalWrite(LED[i], HIGH);
    Serial.print(motion[i]);
    Serial.write(9);
  }
  Serial.println();
  
//  for(int i = 0; i < 5; i++){
//    if(timeCurr - timePrev[i] > 1000) {
//      digitalWrite(LED[i], HIGH);
//      timePrev[i] = timeCurr;
//    }
//  }
}
