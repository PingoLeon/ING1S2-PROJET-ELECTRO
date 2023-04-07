#include <Arduino.h>
const int inputPin = 2; // Entrée du signal de fréquence

void setup() {
  Serial.begin(9600); // Initialisation de la communication série
}

void loop() {
  float freqSum = 0;
  int count = 0;
  for (int i = 0; i < 10; i++) {
    int pulseWidth = pulseIn(inputPin, HIGH, 1000000); // Mesure de la largeur d'impulsion du signal
    float frequency = 1000000.0 / pulseWidth; // Calcul de la fréquence en Hz
    if (frequency != INFINITY) { // Vérification que la fréquence mesurée n'est pas "inf"
      freqSum += frequency;
      count++;
    }
    delay(10); // Attente entre chaque mesure
  }
  if (count > 0) {
    float averageFreq = freqSum / count;
    Serial.print("Fréquence moyenne : ");
    Serial.print(averageFreq);
    Serial.println(" Hz");
  }
}



