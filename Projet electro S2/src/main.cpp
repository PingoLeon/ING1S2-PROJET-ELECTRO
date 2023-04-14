#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <toneAC.h>

//!##################################################  DECLARATION DES PINS ##################################################
//oled screen
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

//buzzer
const int BuzzerPin = 2;

//bouton
const int ButtonPin = 4;
bool buttonState = false;

//VCO
const int inputPin = 5; // Entrée du signal de fréquence lue
const int range = 100;

//ultrason
const int TriggerPin = 11;
const int EchoPin = 12;
long duration;
int distance;

//Metronome : 
const int PotPin1 = A1;
const int PotPin2 = A0;
unsigned long ticInterval; // Intervalle en millisecondes entre les tics
unsigned long lastTicTime = 0; // Temps depuis le dernier tic


//!############################################################################################################################

void toneif(float freq){
  if(freq <= 700){
      tone(BuzzerPin, 261.63);
      Serial.println("1");
  }else if(freq <= 800 && freq > 700){
      tone(BuzzerPin, 293.66);
      Serial.println("2");
  }else if(freq <= 800 && freq > 700){
      tone(BuzzerPin, 329.63);
      Serial.println("3");
  }else if(freq <= 900 && freq > 800){
      tone(BuzzerPin, 349.23);
      Serial.println("4");
  }else if(freq <= 1015 && freq > 900){
      tone(BuzzerPin, 392.00);
      Serial.println("5");
  }else if(freq <= 1120 && freq > 1015){
      tone(BuzzerPin, 440.00);
      Serial.println("6");
  }else if(freq <= 1300 && freq > 1120){
      tone(BuzzerPin, 493.88);
      Serial.println("7");
  }
}


/*
void toneif(float freq){
  if(freq <= 650){
      toneAC(261.63);
      Serial.println("1");
  }else if(freq <= 750 && freq > 650){
      toneAC(293.66);
      Serial.println("2");
  }else if(freq <= 850 && freq > 750){
      toneAC(329.63);
      Serial.println("3");
  }else if(freq <= 950 && freq > 850){
      toneAC(349.23);
      Serial.println("4");
  }else if(freq <= 1050 && freq > 950){
      toneAC(392.00);
      Serial.println("5");
  }else if(freq <= 1150 && freq > 1050){  
      toneAC(440.00);
      Serial.println("6");
  }else if(freq <= 3000 && freq > 1150){
      toneAC(493.88);
      Serial.println("7");
  }
}
*/

//?##################################################  SETUP  ##################################################
void setup() {
  //? Initialisation de la communication série
  Serial.begin(9600);

  //? Initialisation des pins
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);
  pinMode(ButtonPin, INPUT);
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(PotPin1, INPUT);
  pinMode(PotPin2, INPUT);

  //? Initialisation des paramètres de l'ecran
  display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
}
//?############################################################################################################


//*##################################################  LOOP  ##################################################
void loop() {
  //!################################## Mesure de la fréquence du VCO et envoi sur le moniteur série
  //moyenne de 10 fréquences pour le VCO
  float freqSum = 0;
  int count = 0;
  for (int i = 0; i < 1; i++) {
    int pulseWidth;
    float frequency = 0;
        pulseWidth = pulseIn(inputPin, HIGH, 4000); // Durée de mesure plus longue pour les fréquences basses
    frequency = 1000000.0 / pulseWidth; // Calcul de la fréquence en Hz
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

    //send the frequency to the buzzer
    toneif(averageFreq);
  }
  //!############################################################


  //!####################################  Metronome
  // Lecture de la valeur des potentiomètres
  int pot1 = analogRead(PotPin1);
  int pot2 = analogRead(PotPin2);
  
  // Conversion de la valeur de pot1 en intervalle entre les tics
  ticInterval = map(pot1, 0, 1023, 1500, 60) ;
  
  // Conversion de la valeur de pot2 en fréquence du son
  int frequency = map(pot2, 0, 1023, 20, 20000);

  // Production du son avec la fréquence et la durée déterminées par les potentiomètres
  if (millis() - lastTicTime >= ticInterval - 200) {
    toneAC(frequency, 10, 10);
    lastTicTime = millis();
  }
  //!############################################################

  //! Affichage de la valeur du BPM sur l'écran OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("BPM: ");
  display.print(map(pot1, 0, 1023, 1, 200));
  display.display();
}


