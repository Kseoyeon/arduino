const int ledA = 3;
const int ledB = 5;
int ledAState = LOW; 
int ledBState = LOW; 

unsigned long pastA = 0;  
unsigned long pastB = 0;     

const long timeA = 300;   
const long timeB = 700;  

void setup()
{
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop()
{
  unsigned long nowA = millis();

  if (nowA - pastA >= timeA) {
    pastA = nowA;
    if (ledAState == LOW) {
      ledAState = HIGH;
    } else {
      ledAState = LOW;
    }
    digitalWrite(ledA, ledAState);
  }

  unsigned long nowB = millis();

  if (nowB - pastB >= timeB) {
    pastB = nowB;
    if (ledBState == LOW) {
      ledBState = HIGH;
    } else {
      ledBState = LOW;
    }
    digitalWrite(ledB, ledBState);
  }
}
