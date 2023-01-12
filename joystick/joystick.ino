int pin_X= A0;
int pin_Y= A1;
int pin_SW= A2;

void setup()
{
  Serial.begin(115200);
  pinMode(pin_SW, INPUT_PULLUP); // 스위치가 Pullup상태에서 해재 될 떄 인식
}

void  loop()
{

  int x = map(analogRead(pin_X), 0, 1023, -100, 100);
  int y = map(analogRead(pin_Y), 0, 1023, -100, 100);
  // map 함수를 이용하여 입력되는 값을 변형시킴

  Serial.print("X : ");
  Serial.print(x);
  Serial.print("\tY : ");
  Serial.print(y);
  Serial.print(" \tSW : "); // 출력되는 값.

  if(digitalRead(pin_SW))
  {
    Serial.println("OFF");
  }
else 
{
  Serial.println("ON");
 }
delay(100);
}     
