const int LM35Pin = A0;

int DI1Pin = 5;
int DI2Pin = 3;

void setup() {
  Serial.begin(9600);

  pinMode(DI1Pin,OUTPUT);
  pinMode(DI2Pin,OUTPUT);
}

void loop() {
  int adcValue;
  long temp;

  adcValue = analogRead(LM35Pin);
  temp = (adcValue * 500L) / 1024;

  Serial.print(temp);
  Serial.println("℃");

   if(temp >= 24)
  {
    analogWrite(DI1Pin, HIGH);
    digitalWrite(DI2Pin, LOW);
  }
  else{

    digitalWrite(DI1Pin, LOW);
    digitalWrite(DI2Pin, LOW);
  }
  
  delay(1000);
}
