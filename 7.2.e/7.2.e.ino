#include <Servo.h> 

// 서보모터 이름 설정
Servo motor1; 

// 서보 모터 신호핀 설정
int servoMotorPin = 9;

// 포텐쇼미터 핀 설정
int potentioMeterPin = 0;

// 모터 각도 변수 설정
int motorAngle;
int motorAngleOld;

void setup() {
  motor1.attach(servoMotorPin,600,2400);

  // 시리얼 통신 설정  
  Serial.begin(9600);

}

void loop() {

  motor1.write(90);

  delay(1000);

  motor1.detach();

  delay(1000);

  motor1.attach(servoMotorPin,600,2400);

  motor1.write(120);

  delay(1000);

}
