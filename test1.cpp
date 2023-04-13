
//exemple de code avec un capteur à ultrason HC-SR04

#include <Arduino.h>

const int trigPin = 10;
const int echoPin = 11;

const int minSensor = 5;
const int maxSensor = 25;


long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if (distance == 1191){
    distance = 0;

  }

  if (distance >= minSensor && distance <= maxSensor){
    Serial.print(distance);
    Serial.println(" cm");
  }else{
    Serial.println("Out of range");
  }
  delay(100);
}