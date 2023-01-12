#define DIST_S 50*58.2 // 200cm로 제한
#define PING_INTERVAL 10 // Milliseconds // 3개의 경우 100ms
#define delaytime 2000

int trig[10] = {32,34,36,38,40,22,24,26,28,30};
int echo[10] = {33,35,37,39,41,23,25,27,29,31};
int LED[10] = {44,46,48,50,52,45,47,49,51,53};
uint8_t currentSensor = 0;  
unsigned long pingTimer[10];
long dist_r[10];
int count;
unsigned long timeCurr;
unsigned long timePrev[10];

void setup() {
  Serial.begin (38400);
  for (int i=0; i<=9; i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
    pinMode(LED[i], OUTPUT);
    timePrev[i] = millis();
  }
  for (uint8_t i = 1; i < 10; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}
 
void loop() {
  digitalWrite(LED[2], HIGH);
  digitalWrite(LED[5], HIGH);
  digitalWrite(LED[8], HIGH);
  for(int i=0; i<=9; i++){
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * 10;
        if (i == 0 && currentSensor == 9)        
           oneSensorCycle();      
        currentSensor = i;           
        dist_r[i] = trig_ultra(trig[i],echo[i]);
    }
  }
  
  for(int j = 0; j <= 9; j++) {
    timeCurr = millis();
    if(1 < dist_r[j] && 25 >= dist_r[j]) {
      digitalWrite(LED[j-1], HIGH);
      digitalWrite(LED[j], HIGH);
      digitalWrite(LED[j+1], HIGH);
    }
//    else {
//      if( ( 1 > dist_r[j-1] || dist_r[j-1] > 25 ) && ( 1 > dist_r[j+1] || dist_r[j+1] > 25 )){
//      digitalWrite(LED[j], LOW);            
//      }
//    }
  }
  for(int i = 0; i < 10; i++){
    if(timeCurr - timePrev[i] > delaytime) {    
      //digitalWrite(LED[i-1], LOW);
      digitalWrite(LED[i], LOW);
      //digitalWrite(LED[i+1], LOW);
      timePrev[i] = timeCurr;
    }
  }
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
  for (uint8_t i = 0; i < 10; i++) {
      Serial.print(dist_r[i]);
      Serial.print("cm");
      Serial.write(9); //tab
  }
  Serial.println();
}
