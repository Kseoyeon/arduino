#define DIST_S 30*58.2 // 200cm로 제한
#define PING_INTERVAL 20 // Milliseconds // 3개의 경우 100ms

int trig[5]={33,26,14,17,0};
int echo[5]={25,27,12,16,2};
uint8_t LED[5] = {23,22,19,18,21};
uint8_t currentSensor = 0;  
unsigned long pingTimer;
unsigned long cur_time;
unsigned long pre_time;
long dist_r[5];
int num;
long before[5];
long after[5];

void setup() {
  Serial.begin(115200);
  for (int i=0; i<=4; i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
    pinMode(LED[i],OUTPUT);
    digitalWrite(LED[i], HIGH);
  }
  pingTimer= millis();
  pre_time = millis();
}
 
void loop() {

}

void ultra(){
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += PING_INTERVAL;      // Set the next ping time.
    dist_r[num] = trig_ultra(trig[num],echo[num]);
    num++;      
  }
  if(num>4) { num=0; oneSensorCycle();}
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
