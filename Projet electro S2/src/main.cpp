#include <Arduino.h>
//get analogic signal from pin AO
int pinAO = A0;
int value = 0;
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (value == 0){
    analogRead(pinAO);
  }
  Serial.println(value);
  delay(100);
}