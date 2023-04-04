#include <Arduino.h>
//get analogic signal from pin AO
int pinAO = A0;
int value = 0;
int pin = 9;
unsigned long duration;

void setup() {
  Serial.begin(9600);
  pinMode(pin, INPUT);
}

void loop() {
  //mesure the duration of the pulse
  duration = pulseIn(pin, HIGH);

  //make an average of 10 values of the duration of the pulse
  for (int i = 0; i < 10; i++) {
    duration += pulseIn(pin, HIGH);
  }
  duration = duration / 10;

  Serial.println(duration);
  duration = 0;
  delay(100);
}