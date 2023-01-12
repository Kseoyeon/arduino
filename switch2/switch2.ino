const int led = 9;
const int button = 2;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  int swInput = digitalRead(button);

  if(swInput == LOW) digitalWrite(led, HIGH);
  else digitalWrite(led, LOW);
}
