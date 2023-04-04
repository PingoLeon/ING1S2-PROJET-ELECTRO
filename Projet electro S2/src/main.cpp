#include <Arduino.h>
//get analogic signal from pin AO
//send it to buzzer on pin 3

int pinAO = A0;
void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(pinAO);
  Serial.println(value);
  delay(100);
}