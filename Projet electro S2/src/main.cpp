#include <Arduino.h>
const int inputPin = 12; // Entrée du signal de fréquence lue
const int BuzzerPin = 2; // Entrée du signal de fréquence envoyée

void toneif(float freq){
  if(freq <= 650){
    tone(BuzzerPin, 261.63);
  }else if(freq <= 750 && freq > 650){
    tone(BuzzerPin, 293.66);
  }else if(freq <= 850 && freq > 750){
    tone(BuzzerPin, 329.63);
  }else if(freq <= 950 && freq > 850){
    tone(BuzzerPin, 349.23);
  }else if(freq <= 1050 && freq > 950){
    tone(BuzzerPin, 392.00);
  }else if(freq <= 1150 && freq > 1050){
    tone(BuzzerPin, 440.00);
  }else if(freq <= 3000 && freq > 1150){
    tone(BuzzerPin, 493.88);
  }
}

void setup() {
  Serial.begin(9600); // Initialisation de la communication série
  pinMode(BuzzerPin, OUTPUT); // Initialisation de la broche de sortie du signal de fréquence
  pinMode(inputPin, INPUT); // Initialisation de la broche d'entrée du signal de fréquence
}

void loop() {
  float freqSum = 0;
  int count = 0;
  for (int i = 0; i < 100; i++) {
    int pulseWidth = pulseIn(inputPin, HIGH, 1000000); // Mesure de la largeur d'impulsion du signal
    float frequency = 1000000.0 / pulseWidth; // Calcul de la fréquence en Hz
    if (frequency != INFINITY) { // Vérification que la fréquence mesurée n'est pas "inf"
      freqSum += frequency;
      count++;
    }else{
      noTone(BuzzerPin);
    }
  }
  if (count > 0) {
    float averageFreq = freqSum / count;
    Serial.print("Fréquence moyenne : ");
    Serial.print(averageFreq);
    Serial.println(" Hz");
    //send the frequency to the test tone
    toneif(averageFreq);
  }
  delay(8);

  

}



