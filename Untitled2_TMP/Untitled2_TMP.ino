#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>

Adafruit_LiquidCrystal lcd(0);
Servo motor;
int motorpin = 2;
const int button1 = 3; 
const int button2 = 4;
const int button3 = 5;
const int button4 = 6;
const int led1 = 10;
const int led2 = 11;
const int led3 = 12;
const int led4 = 13;
const int redled = 7;
const int greenled = 8;
const int blueled = 9;

int buttonState[]={0,0,0,0};
int lastButtonstate[]={0,0,0,0};
int buttonPushCounter[]={0,0,0,0};

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("LEVEL : ");
  lcd.setCursor(0,1);
  lcd.print("SCORE : ");
  randomSeed(analogRead(0));

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

int game_on = 0;
int wait = 0;
int currentlevel = 1;
long rand_num = 0;
int rando = 0;
int n_levels = 10;
int right = 0;
int pin = 0;
int score = 0;

void loop() {

  int n_array[n_levels];
  int u_array[n_levels];
  int i;


  lcd.setCursor(9,0);
  lcd.print("  ");
  lcd.setCursor(9,0);
  lcd.print(currentlevel);

  lcd.setCursor(9,1);
  lcd.print("  ");
  lcd.setCursor(9,1);
  lcd.print(score);

  if(game_on == 0){
    for(i=0;i<n_levels;i++){
      n_array[i]=0;
      u_array[i]=0;
      rand_num=random(1,200);
      if(rand_num<=50)
      rando = 0;
      else if(rand_num>50&&rand_num<=100)
      rando = 1;
      else if(rand_num>100&&rand_num<=150)
      rando = 2;
      else if(rand_num>150&&rand_num<=200)
      rando = 3;

      n_array[i]=rando;
    }

    game_on = 1;
  }

  if(wait == 0){
    delay(200);
    i=0;
    for(i=0;i<currentlevel;i++){
      pin = n_array[i];
      digitalWrite(pin + 10,HIGH);
      delay(500);
      digitalWrite(pin + 10,LOW);
      delay(50);
    }
    wait = 1;
  }

  i = 0;
  int buttonchange = 0;
  int j = 0;
  while(j<currentlevel){
    while(buttonchange == 0){
      for(i=0;i<4;i++){
        buttonState[i]=digitalRead(i+3);
        buttonchange=buttonchange+buttonState[i];
      }
    }
    for(i=0;i<4;i++){
      if(buttonState[i]==HIGH){
        digitalWrite(i+10,HIGH);
        digitalWrite(i+10,LOW);
        wait=0;
        u_array[j]=i;
        buttonState[i]=LOW;
        buttonchange=0;
      }
    }

    if(u_array[j]==n_array[j]){
      j++;
      right=1;
    }

    else{
      right=0;
      i=4;
      j=currentlevel;
    }
  }

  if(right==0){
    
    currentlevel++;

    digitalWrite(redled,HIGH);
    delay(1000);
    digitalWrite(redled,LOW);
    delay(500);
    
    wait=0;
  }

  if(right==1){
    
    score=score+currentlevel;
    currentlevel++;

    digitalWrite(blueled,HIGH);
    delay(1000);
    digitalWrite(blueled,LOW);
    delay(500);
    
    wait=0;
  }

  if(currentlevel==n_levels){
    delay(500);
    for(i=0;i<4;i++){
      digitalWrite(i+10,HIGH);
    }
    for(i=0;i<4;i++){
      digitalWrite(i+10,LOW);
    }
    delay(200);
    for(i=0;i<4;i++){
      digitalWrite(i+10,HIGH);
    }
    for(i=0;i<4;i++){
      digitalWrite(i+10,LOW);
    }
    delay(500);

    if(score>=40){
      motor.write(90);
    }

    while(digitalRead(button1) == HIGH){}

    game_on=0;
    currentlevel=1;
  }

}