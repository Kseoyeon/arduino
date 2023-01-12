#include <Adafruit_LiquidCrystal.h> //LCD i2c 라이브러리
#include <Servo.h> //서보모터 라이브러리
#include <Wire.h> //

Adafruit_LiquidCrystal lcd(0);
Servo motor;
int motorpin = 2;
const int button1 = 3; 
const int button2 = 4;
const int button3 = 5;
const int button4 = 6;
const int resetbutton = A1;
const int led1 = 10;
const int led2 = 11;
const int led3 = 12;
const int led4 = 13;
const int redled = 7;
const int greenled = 8;
const int blueled = 9;

int buttonState[]={0,0,0,0}; //버튼의 상태 저장할 배열

int game_on = 0;
int wait = 0;
int currentlevel = 1; //현재 진행 레벨
long rand_num = 0; //난수 담는 변수
int rando = 0; //배열에 넣을 난수의 변수
int n_levels = 10; //총 레벨
int right = 0; //정답인지 아닌지 판단
int pin = 0; //버튼 눌렀을 때 점등될 핀
int score = 0; //점수

void setup() {
  lcd.begin(16, 2); 
  lcd.setCursor(0,0);
  lcd.print("LEVEL : ");
  lcd.setCursor(0,1);
  lcd.print("SCORE : ");
  lcd.setCursor(9,1);
  lcd.print("0");
  randomSeed(analogRead(0)); //난수 생성

  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(greenled,OUTPUT);
  pinMode(blueled,OUTPUT);

  motor.attach(motorpin);  
}

void loop() {

  int n_array[n_levels]; //난수배열
  int u_array[n_levels]; //버튼배열
  int i;
  int reset = digitalRead(resetbutton); //게임이 끝나고 리셋버튼 누르기 전까지 정지하기 위한 변수
  motor.write(0);

  lcd.setCursor(9,0);
  lcd.print("  ");
  lcd.setCursor(9,0);
  lcd.print(currentlevel);
  
  if(game_on == 0){
    for(i=0;i<n_levels;i++){ //총 레벨까지 반복
      n_array[i]=0;
      u_array[i]=0;
      rand_num=random(1,200); //1~200 난수 생성해서 rand_num 변수에 저장
      if(rand_num<=50)
      rando = 0;
      else if(rand_num>50&&rand_num<=100)
      rando = 1;
      else if(rand_num>100&&rand_num<=150)
      rando = 2;
      else if(rand_num>150&&rand_num<=200)
      rando = 3;

      n_array[i]=rando; //켜질 LED 순서가 배열에 저장됨
    }

    game_on = 1; //난수생성 정지
  }

  if(wait == 0){
    delay(200);
    i=0;
    for(i=0;i<currentlevel;i++){ //현재레벨까지 반복
      pin = n_array[i];
      digitalWrite(pin + 10,HIGH); //순서에 맞는 LED 점등
      delay(500);
      digitalWrite(pin + 10,LOW);
      delay(50);
    }
    wait = 1;
  }

  i = 0;
  int buttonchange = 0;
  int j = 0;
  while(j<currentlevel){//j가 현재레벨보다 작을 때까지
    while(buttonchange == 0){
      for(i=0;i<4;i++){
        buttonState[i]=digitalRead(i+3); //현재 버튼 상태를 배열에 저장
        buttonchange=buttonchange+buttonState[i];
      }
    }
    for(i=0;i<4;i++){
      if(buttonState[i]==HIGH){ //버튼이 눌리면
        delay(300);
        digitalWrite(i+10,HIGH); //순서에 맞는 LED 점등
        digitalWrite(i+10,LOW);
        wait=0;
        u_array[j]=i; //LED와 비교할 버튼 배열에 저장
        buttonState[i]=LOW;
        buttonchange=0;
      }
    }

    if(u_array[j]==n_array[j]){ //LED와 버튼 순서 비교해서 같으면
      j++;
      right=1;
    }

    else{ //LED와 버튼 순서가 같지 않으면
      right=0;
      i=4; //더이상 반복 X
      j=currentlevel; //while문 정지
      wait=0;
    }
  }

  if(right==0){ //LED와 버튼 순서가 같지 않으면
    delay(300);
    i=0;
    
    digitalWrite(redled,HIGH); //RGB 빨간 LED 점등
    delay(1000);
    digitalWrite(redled,LOW);
    delay(500);
    
    currentlevel++; //레벨 상승
    
    wait=0; //게임 진행
  }

  if(right==1){ //LED 순서와 버튼 순서가 같으면
    
    score=score+currentlevel; //점수 증가
    currentlevel++; //현재 레벨 증가

    lcd.setCursor(9,1);
    lcd.print("  ");
    lcd.setCursor(9,1);
    lcd.print(score); //LCD에 점수 표시

    digitalWrite(blueled,HIGH); //파란 LED 점등
    delay(1000);
    digitalWrite(blueled,LOW);
    delay(500);
    
    wait=0; //게임진행
  }

    if(currentlevel == (n_levels + 1)){ //현재레벨이 최고단계를 넘으면
    
    //LED 4개 다 점등
    delay(500);
    for(i=0;i<4;i++){
      digitalWrite(i+10,HIGH);
    }
    delay(500);
    for(i=0;i<4;i++){
      digitalWrite(i+10,LOW);
    }
    delay(200);
    for(i=0;i<4;i++){
      digitalWrite(i+10,HIGH);
    }
    delay(500);
    for(i=0;i<4;i++){
      digitalWrite(i+10,LOW);
    }
    delay(500);

    if(score>=40){ //점수가 40점이 넘으면
      motor.write(90); //서보모터 작동
      delay(10000);
    }

    while(reset == LOW){} //리셋버튼 누르기 전까지 게임 정지
    }
}
