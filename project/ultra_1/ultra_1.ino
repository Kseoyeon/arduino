#define DIST_S 200*58.2 // 200cm로 제한
#define PING_INTERVAL 33 // Milliseconds // 3개의 경우 100ms

int trig[3]={30,32,34};
int echo[3]={22,24,26};
uint8_t currentSensor = 0;  
unsigned long pingTimer[3];
long dist_r[3];
int count;
void setup() {
  Serial.begin (38400);
  for (int i=0; i<=2; i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
  }
  for (uint8_t i = 1; i < 3; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}
 
void loop() {
  for(int i=0; i<=2; i++){
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * 3;
        if (i == 0 && currentSensor == 2)        
           oneSensorCycle();      
        currentSensor = i;           
        dist_r[i] = trig_ultra(trig[i],echo[i]);
    }
  }
// 원하는 코드 추가
}

long trig_ultra(int TRIG,int ECHO)
{
  long dist;
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  dist = pulseIn(ECHO, HIGH,DIST_S)/58.2;
  return(dist);
}
void oneSensorCycle() { 
  for (uint8_t i = 0; i < 3; i++) {
      Serial.print(dist_r[i]);
      Serial.print("cm");
      Serial.write(9); //tab
  }
  Serial.println();
}
