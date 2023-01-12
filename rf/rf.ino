#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <SPI.h>

int led = 10;
RF24 radio(8, 7);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  pinMode(led,OUTPUT);

}

void loop() {
  int ledState = 1;
  radio.write(&ledState, sizeof(ledState));
  digitalWrite(led,ledState);
  delay(50);

}
