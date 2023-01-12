#include <MsTimer2.h>

int Switch1_Pin = A5;  // 스위치1
int Switch2_Pin = A4;  // 스위치2
int Switch3_Pin = A3;  // 스위치3

int Fnd_a = 7;    // Fnd "a" 자리 핀(Pin)
int Fnd_b = 6;    // Fnd "b" 자리 핀(Pin)
int Fnd_c = 5;    // Fnd "c" 자리 핀(Pin)
int Fnd_d = 4;    // Fnd "d" 자리 핀(Pin)
int Fnd_e = 3;    // Fnd "e" 자리 핀(Pin)
int Fnd_f = 2;    // Fnd "f" 자리 핀(Pin)
int Fnd_g = 13;    // Fnd "g" 자리 핀(Pin)
int Fnd_dot = 12;  // Fnd "dot" 자리 핀(Pin)
/* 전역 변수 선언 */
int place_1 = 11;    // Fnd 1의 자리
int place_10 = 10;   // Fnd 10의 자리
int place_100 = 9;   // Fnd 100의 자리
int place_1000 = 8;  // Fnd 1000의 자리

unsigned int FndCnt = 0;          // Fnd 숫자 디스플레이 변수
unsigned int FndPlaceTickCnt = 0;  // Fnd 자리 시프트 변수
boolean flag = 0;                 // Fnd 값 상승 or 정지 플래그 변수

byte seven_seg_digits[10][8] = {  // 0~9까지
// a, b, c, d, e, f, g, h
  {1, 1, 1, 1, 1, 1, 0, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1, 0},  // 2
  {1, 1, 1, 1, 0, 0, 1, 0},  // 3
  {0, 1, 1, 0, 0, 1, 1, 0},  // 4
  {1, 0, 1, 1, 0, 1, 1, 0},  // 5
  {1, 0, 1, 1, 1, 1, 1, 0},  // 6
  {1, 1, 1, 0, 0, 1, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1, 0},  // 8
  {1, 1, 1, 1, 0, 1, 1, 0}  // 9
};
void NumberDisplay(byte num)  // Fnd에 num값에 해당하는 숫자를 표현하는 함수
{
  int j;

  for (j = 7; j >= 0; j--) // a ~ g
  {
    // Fnd에 표현되는 숫자는 seven_seg_digits에 선언
    if (j == 0) 
      digitalWrite(12, seven_seg_digits[num][7 - j]);
    else if (j == 1) 
      digitalWrite(13, seven_seg_digits[num][7 - j]);
    else
      digitalWrite(j, seven_seg_digits[num][7 - j]);
  }
}
// 4열 Fnd에 4개의 공통애노드 핀(Pin)에 신호를 번갈아 가면서 선택
void Fnd_4place_Display_()
{
  int j, k, l, m = 0;
  
  //FndCnt = 1234;
  
  j = (FndCnt % 10000) / 1000; // 1000의 자리
  k = (FndCnt % 1000) / 100; // 100의 자리
  l = (FndCnt % 100) / 10; // 10의 자리
  m = (FndCnt % 10);   // 1의 자리

  switch (FndPlaceTickCnt) // 1ms마다 Fnd 공통 애노드 핀(Pin) 선택
  {
    case 0:
      digitalWrite(place_1, LOW);
      digitalWrite(place_10, HIGH);
      digitalWrite(place_100, HIGH);
      digitalWrite(place_1000, HIGH);
      NumberDisplay(m);
      break;
    case 1:
      digitalWrite(place_1, HIGH);
      digitalWrite(place_10, LOW);
      digitalWrite(place_100, HIGH);
      digitalWrite(place_1000, HIGH);
      NumberDisplay(l);
      break;
    case 2:
      digitalWrite(place_1, HIGH);
      digitalWrite(place_10, HIGH);
      digitalWrite(place_100, LOW);
      digitalWrite(place_1000, HIGH);
      NumberDisplay(k);
      break;
    case 3:
      digitalWrite(place_1, HIGH);
      digitalWrite(place_10, HIGH);
      digitalWrite(place_100, HIGH);
      digitalWrite(place_1000, LOW);
      NumberDisplay(j);
      break;
  /*     
    case 0:
      digitalWrite(place_1, HIGH);
      digitalWrite(place_10, LOW);
      digitalWrite(place_100, LOW);
      digitalWrite(place_1000, LOW);
      NumberDisplay(m);
      break;
    case 1:
      digitalWrite(place_1, LOW);
      digitalWrite(place_10, HIGH);
      digitalWrite(place_100, LOW);
      digitalWrite(place_1000, LOW);
      NumberDisplay(l);
      break;
    case 2:
      digitalWrite(place_1, LOW);
      digitalWrite(place_10, LOW);
      digitalWrite(place_100, HIGH);
      digitalWrite(place_1000, LOW);
      NumberDisplay(k);
      break;
    case 3:
      digitalWrite(place_1, LOW);
      digitalWrite(place_10, LOW);
      digitalWrite(place_100, LOW);
      digitalWrite(place_1000, HIGH);
      NumberDisplay(j);
      break;
      */
  }
}
void ISR_Timer()
{
  static unsigned int cnt, cnt2 = 0;
  /* Timer에서 flag 변수 값의 상태 여부를 1ms 마다 체크하여, flag값이 참이라면 FndCnt값을 증가 시킨다. 만일 flag값이 거짓이라면 FndCnt값을 증가를 정지한다.  */
  
  if (flag)
  {
    if (cnt >= 20)
    {
      if (FndCnt >= 9999)
      {
        FndCnt = 0;
      }
      else
      {
        FndCnt++;
      }
      cnt = 0;
    }
    else
    {
      cnt++;
    }
  }
 
/* cnt2값이 1ms마다 증가한다. 이는 Fnd에 공통캐소드 핀(Pin)에 번갈아 가면서 신호를 주기 위한 카운트 변수이다. 4ms마다 FndPlaceTickCnt 변수 값이 증가하며, 이 변수 값에 의해서 Fnd에 공통캐소드 핀(Pin)에 자리가 변경되면서 Fnd에 숫자가 표현 된다. 여러 자리의 숫자를 출력할 때는 한 개씩 빠르게 켰다 껐다를 반복하여 동시에 켠 것처럼 보이게 한다. 이를 다이내믹 디스플레이 제어 방식이라 한다. */
  if (cnt2 > 3)
  {
    if (FndPlaceTickCnt > 3)
    {
      FndPlaceTickCnt = 0;
    }
    else
    {
      FndPlaceTickCnt++;
    }
    cnt2 = 0;
  }
  else
  {
    cnt2++;
  }
}
void setup()  // 초기화
{
  pinMode(Fnd_a, OUTPUT);    // Fnd 'a' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_b, OUTPUT);    // Fnd 'b' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_c, OUTPUT);    // Fnd 'c' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_d, OUTPUT);    // Fnd 'd' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_e, OUTPUT);    // Fnd 'e' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_f, OUTPUT);     // Fnd 'f' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_g, OUTPUT);    // Fnd 'g' 위치의 핀(Pin) 출력 포트 설정
  pinMode(Fnd_dot, OUTPUT);  // Fnd 'dot' 위치의 핀(Pin) 출력 포트 설정

  pinMode(place_1, OUTPUT);  // Fnd1
  pinMode(place_10, OUTPUT);  // Fnd10
  pinMode(place_100, OUTPUT);   // Fnd100
  pinMode(place_1000, OUTPUT);   // Fnd1000

  pinMode(Switch1_Pin, INPUT);  // 스위치 1번 핀(Pin) 입력 포트 설정
  pinMode(Switch2_Pin, INPUT);  // 스위치 2번 핀(Pin) 입력 포트 설정
  pinMode(Switch3_Pin, INPUT);  // 스위치 3번 핀(Pin) 입력 포트 설정

  MsTimer2::set(1, ISR_Timer);  // 타이머 인터럽트 설정, 1ms마다, ISR_Timer를 불러옴
  MsTimer2::start();           // 타이머 인터럽트 On
}
/* 각 스위치를 이용하여 Fnd(7-세그먼트)에 숫자 값을 증가 혹은 정지 시키거나 값을
  리셋 하도록 한다. */
void loop()  // 무한 루프
{
  Fnd_4place_Display_();  // Fnd값 출력 디스플레이 함수

  if (digitalRead(Switch1_Pin) == LOW)      // 첫 번째 스위치 누르면
  {
    flag = 1;                              // Fnd 값 카운트 시작
  }
  else if (digitalRead(Switch2_Pin) == LOW)  // 두 번째 스위치 누르면
  {
    flag = 0;                              // Fnd 값 카운트 종료
  }
  else if (digitalRead(Switch3_Pin) == LOW)  // 세 번째 스위치 누르면
  {
    FndCnt = 0;                           // Fnd 값 클리어
  }
}

