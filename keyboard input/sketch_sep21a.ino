const int ledA = 11;
const int ledB = 9;
int Num = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop()
{
  if(Serial.available())
  {
    char i = Serial.read();
    if(isDigit(i)){
      Num = ( i - '0');
    }
  }
    digitalWrite(ledA,HIGH);
    digitalWrite(ledB,LOW);
    delay(Num*100);
    digitalWrite(ledA,LOW);
    digitalWrite(ledB,HIGH);
    delay(Num*100);
}
