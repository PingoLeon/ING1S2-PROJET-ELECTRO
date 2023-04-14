#include <Arduino.h>

//initialisation du VCO et du buzzer
const int inputPin = 12; // Entrée du signal de fréquence lue
const int BuzzerPin = 2; // Entrée du signal de fréquence envoyée

//ultrason
const int TriggerPin = 11;
const int EchoPin = 12;
long duration;
int distance;


//!REREGLER LES PLAGES DE FREQUENCE POUR LE BUZZER (pq sinon c'est moche)
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

  //initialisation du VCO et du buzzer
  pinMode(BuzzerPin, OUTPUT); // Initialisation de la broche de sortie du signal de fréquence
  pinMode(inputPin, INPUT); // Initialisation de la broche d'entrée du signal de fréquence

  //initialisation du capteur à ultrason
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

void loop() {

  //moyenne de 100 fréquences pour le VCO
  float freqSum = 0;
  int count = 0;
  for (int i = 0; i < 100; i++) {
    int pulseWidth = pulseIn(inputPin, HIGH, 1000000); // Mesure de la largeur d'impulsion du signal
    float frequency = 1000000.0 / pulseWidth; // Calcul de la fréquence en Hz
    if (frequency != INFINITY) { 

      // Vérification que la fréquence mesurée n'est pas "inf"
      freqSum += frequency;
      count++;

    }else{

      //Si la fréquence est infinie, on coupe le buzzer car c'est une valeur inexploitable
      noTone(BuzzerPin);

    }
  }
  if (count > 0) {
    // Calcul de la fréquence moyenne et affichage sur le moniteur série
    float averageFreq = freqSum / count;
    Serial.print("Fréquence moyenne : ");
    Serial.print(averageFreq);
    Serial.println(" Hz");

    //send the frequency to the test tone
    toneif(averageFreq);
  }
  delay(8);
}




/*
Code simple sans moyenne pour avoir fréquence 
const int inputPin = 12; // Pin sur laquelle le signal est connecté
unsigned long duration; // Durée du signal en microsecondes
float frequency; // Fréquence du signal en Hz

void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  duration = pulseIn(inputPin, HIGH); // Mesure de la durée du signal carré en microsecondes
  frequency = 1000000.0 / duration; // Calcul de la fréquence en Hz
  tone(2, frequency);
  Serial.print("Fréquence : ");
  Serial.print(frequency);
  Serial.println(" Hz");
  delay(100);
}
*/


