const int led = 9;
const int button = 2;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  int swInput = digitalRead(button);
  int ledOutput = digitalRead(led);

  if(swInput == LOW)
    {
      if(ledOutput == HIGH) digitalWrite(led, LOW);
      else digitalWrite(led, HIGH);
    }
}
