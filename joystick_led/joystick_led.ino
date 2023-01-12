int joyX = A1;
int joyY = A2;

int button=2;
int buttonState = 0;

void setup() {
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(button,INPUT);
  digitalWrite(button, HIGH);
  Serial.begin(9600);
}
 
void loop() {
 int xValue = analogRead(joyX);
 int yValue = analogRead(joyY);
 
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
  
  buttonState = digitalRead(button);
  Serial.println(buttonState);
  if ((xValue>=0 && xValue <= 1023)&& yValue==1023)
  {
    digitalWrite(10, HIGH);
    delay(100);
  }
  else{digitalWrite(10, LOW);}

  if (xValue==0 && (yValue>=0 && yValue <= 1023))
  {
    digitalWrite(9, HIGH);
    delay(100);
  }
  else{digitalWrite(9, LOW);}

  if ((xValue >= 0 && xValue <= 1023) && yValue==0)
  {
    digitalWrite(5, HIGH);
    delay(100);
  }
  else{digitalWrite(5, LOW);}

  if (xValue==1023 && (yValue<=1023 && yValue >= 0))
  {
    digitalWrite(3, HIGH);
    delay(100);
  }
  else{digitalWrite(3, LOW);}

  if (buttonState == LOW)
  {
    Serial.println("Switch = High");
    digitalWrite(3, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  else{
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
  }
  delay(50);
}
