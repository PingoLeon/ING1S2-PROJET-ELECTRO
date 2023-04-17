#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <toneAC.h>
#

//!##################################################  DECLARATION DES PINS ET VARIABLES GLOBALES ##################################################

//oled screen
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

//buzzer
const int BuzzerPin = 2;

//bouton
const int ButtonPin = 4;
bool buttonState = false;
bool block;

//VCO
const int inputPin = 5; // Entrée du signal de fréquence lue
const int range = 100;

//ultrason
const int TriggerPin = 11;
const int EchoPin = 12;
long duration;
int distance;

//paramètres du capteur à ultrason
const int minSensor = 5;
const int pas = 5;

//Metronome 
const int PotPin1 = A1;
const int PotPin2 = A0;
unsigned long ticInterval; // Intervalle en millisecondes entre les tics
unsigned long lastTicTime = 0; // Temps depuis le dernier tic


//!############################################################################################################################

void toneif(float freq, int ultrason){
  if(freq <= 700){
      freq = 261.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
  else if(freq <= 800 && freq > 700){
      freq = 293.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
  else if(freq <= 800 && freq > 700){
      freq = 329.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
  else if(freq <= 900 && freq > 800){
      freq = 349.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
  else if(freq <= 1015 && freq > 900){
      freq = 392.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
  else if(freq <= 1120 && freq > 1015){
      freq = 440.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
  else if(freq <= 1300 && freq > 1120){
      freq = 493.0;
      tone(BuzzerPin, freq + ultrason);
      Serial.write((byte*)&freq, sizeof(float));
      Serial.write((byte*)&ultrason, sizeof(int));
  }
}

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


//*##################################################  LOOP  ##################################################
void loop() {

  //?##################################  Bouton  ##################################
  //block all the program while the button hasn't been pressed another time
  if (digitalRead(ButtonPin) == HIGH && buttonState == false) {
    buttonState = true;
    block = true;
  }
  while(block) {
    if(digitalRead(ButtonPin) == HIGH && buttonState == true) {
      buttonState = false;
      block = false;
    }

    //display a message on the screen
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Le système est bloqué");
    display.println("Appuyez sur le bouton");
    display.println("pour le débloquer");
    display.display(); 
  }
    
  //!################################## Mesure de la fréquence du VCO et envoi sur le moniteur série
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

    //mesure de l'ultrason
    digitalWrite(TriggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TriggerPin, LOW);
    duration = pulseIn(EchoPin, HIGH);
    distance = duration * 0.034 / 2;
    
    //comparer cette distance : 
    if(distance == 1191){
      distance = 0;
    }else if(distance >= minSensor && distance < (minSensor + pas)){
      distance = 20;
    }else if(distance >= (minSensor + pas) && distance < (minSensor + 2*pas)){
      distance = 40;
    }else if(distance >= (minSensor + 2*pas) && distance < (minSensor + 3*pas)){
      distance = 60;
    }else if(distance >= (minSensor + 3*pas) && distance <= (minSensor + 4*pas)){
      distance = 80;
    }

    //send the frequency to the buzzer
    toneif(averageFreq, distance);
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