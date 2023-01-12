#define DIST_S 200*58.2 // 200cm로 제한
#define PING_INTERVAL 20 // Milliseconds // 3개의 경우 100ms

int trig[5]={33,26,14,17,0};
int echo[5]={25,27,12,16,2};
uint8_t currentSensor = 0;  
unsigned long pingTimer;
long dist_r[5];
int num;
void setup() {
  Serial.begin(115200);
  for (int i=0; i<=4; i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
  }
  pingTimer= millis();
}
 
void loop() {

  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
  
    pingTimer += PING_INTERVAL;      // Set the next ping time.
    dist_r[num] = trig_ultra(trig[num],echo[num]);
    num++;      
  }
  if(num>4) { num=0; oneSensorCycle();   }
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
  for (uint8_t i = 0; i < 5; i++) {
      Serial.print(dist_r[i]);
      Serial.print("cm");
      Serial.write(9); //tab
  }
  Serial.println();
}
