
// 포텐쇼미터 핀 설정
int potentioMeterPin = A0;

// in1 핀 설정
int DI1Pin = 5;
// in2 핀 설정
int DI2Pin = 3;

int motorPWM;
int motorVelocity;

void setup() {
  // 시리얼 통신 설정  
  Serial.begin(9600);

  pinMode(DI1Pin,OUTPUT);
  pinMode(DI2Pin,OUTPUT);
}

void loop(){
  // 포텐쇼미터 값을 읽어옴
  int potentioMeter = analogRead(potentioMeterPin);

  // 포텐쇼미터 아날로그 입력값이 0~500 일 때 CW로 동작 
  if((potentioMeter >= 0) && (potentioMeter <=500)){
    motorPWM = map(potentioMeter,500,0,0,255);
    // 모터를 CW로 회전
    analogWrite(DI1Pin, motorPWM);
    digitalWrite(DI2Pin, LOW);

    motorVelocity = map(potentioMeter,500,0,0,100);
    // 모터의 회전 방향을 출력하고 백분율로 속도를 표시한다    
    Serial.print("CW ");
    Serial.print(motorVelocity);
    Serial.println("%");
  }
  // 포텐쇼미터 아날로그 입력값이 524~1023 일 때 CCW로 동작
  else if((potentioMeter >= 524) && (potentioMeter <=1023)){
    motorPWM = map(potentioMeter,524,1023,0,255);
    // 모터를 CCW로 회전
    digitalWrite(DI1Pin, LOW);
    analogWrite(DI2Pin, motorPWM);

    motorVelocity = map(potentioMeter,524,1023,0,100);
    // 모터의 회전 방향을 출력하고 백분율로 속도를 표시한다
    Serial.print("CCW ");
    Serial.print(motorVelocity);
    Serial.println("%");
  }
  else{

    digitalWrite(DI1Pin, LOW);
    digitalWrite(DI2Pin, LOW);
    // 모터 정지 메세지를 출력한다
    Serial.println("STOP");          
  }
  delay(100);
}
