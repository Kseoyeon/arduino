int echoPin[] = {0,23,25,27,29,31,33,35,37,39,41,0};
int trigPin[] = {0,22,24,26,28,30,32,34,36,38,40,0};
int LED[] = {0,44,46,48,50,52,45,47,49,51,53,0};

void setup() {
  Serial.begin(9600);
  for(int i = 1; i <= 10 ;i++){
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
    pinMode(LED[i], OUTPUT);
  }
}

void sensor(int j) {
  long duration[10];
  long distance[10];
  digitalWrite(trigPin[j], LOW);
  digitalWrite(echoPin[j], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin[j], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin[j], LOW);
  
  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  duration[j] = pulseIn(echoPin[j], HIGH); 
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  distance[j] = duration[j] * 17 / 1000;  
  if (distance[j] >= 200 || distance[j] <= 0)       // 거리가 200cm가 넘거나 0보다 작으면
  {
    Serial.println("거리를 측정할 수 없음");   // 에러를 출력합니다.
  }
  else                                                    // 거리가 200cm가 넘지 않거나 0보다 작지 않으면
  {
    Serial.print(j);
    Serial.print(" : ");
    Serial.print(distance[j]);                         // distance를 시리얼 모니터에 출력합니다.
    Serial.println(" cm");                           // cm를 출력하고 줄을 넘깁니다.
  }

  if(distance[j] <= 20){
    digitalWrite(LED[j-1], HIGH);
    digitalWrite(LED[j], HIGH);
    digitalWrite(LED[j+1], HIGH);
    delay(100);
  }
}

void loop() {

  digitalWrite(LED[1], LOW);
  digitalWrite(LED[2], LOW);
  digitalWrite(LED[3], LOW);
  digitalWrite(LED[4], LOW);
  digitalWrite(LED[5], LOW);
//  digitalWrite(LED[6], HIGH);
//  digitalWrite(LED[7], LOW);
//  digitalWrite(LED[8], LOW);
//  digitalWrite(LED[9], HIGH);
//  digitalWrite(LED[10], LOW);
  

  sensor(1);
  sensor(2);
  sensor(3);
  sensor(4);
  sensor(5);
//  sensor(6);
//  sensor(7);
//  sensor(8);
//  sensor(9);
//  sensor(10);
}
