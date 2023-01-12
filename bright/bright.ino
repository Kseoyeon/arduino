const int led = 9;
int brightness = 0;
int increment = 1;

void setup() {

}

void loop() {
  analogWrite(led, brightness);

  brightness = brightness + increment;

  if((brightness >= 255)||(brightness <=0)) increment = -increment;
  delay(10);

}
