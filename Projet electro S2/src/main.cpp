#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <toneAC.h>


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
int inf_counter = 0;

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
  int listFreq[] = {200,600, 700, 800, 900, 1000, 1100, 1500};
  if(freq <= listFreq[1] && freq >= listFreq[0]){
    freq =  262;
  }else if(freq <= listFreq[2] && freq >= listFreq[1]){
    freq =  294;
  }else if(freq <= listFreq[3] && freq >= listFreq[2]){
    freq =  330;
  }else if(freq <= listFreq[4] && freq >= listFreq[3]){
    freq =  349;
  }else if(freq <= listFreq[5] && freq >= listFreq[4]){
    freq =  392;
  }else if(freq <= listFreq[6] && freq >= listFreq[5]){
    freq =  440;
  }else if(freq <= listFreq[7] && freq >= listFreq[6]){
    freq =  494;
  }


  tone(BuzzerPin, freq + ultrason);
  Serial.print(ultrason);
  Serial.print(',');
  Serial.println(freq);
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
  
  //if button clicked, pause the system, until the button is clicked again
  buttonState = digitalRead(ButtonPin);
  if(buttonState == HIGH){
    if(block == false){
      block = true;
      noTone(BuzzerPin);
    }
  }

  while(block == true){
    delay(1000);
    buttonState = digitalRead(ButtonPin);
    if(buttonState == HIGH){
      block = false;
    }

    //?##################################  Ecran  ##################################
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Pause");
    display.display();
  }

    
  //!################################## Mesure de la fréquence du VCO et envoi sur le moniteur série
  int pulseWidth;
  float frequency = 0;
  pulseWidth = pulseIn(inputPin, HIGH, 4000); // Durée de mesure plus longue pour les fréquences basses
  frequency = 1000000.0 / pulseWidth; // Calcul de la fréquence en Hz
    

if (frequency == INFINITY) {
    inf_counter++;
    if (inf_counter > 3) {
      noTone(BuzzerPin);
    }
  } else {
    inf_counter = 0; // Réinitialiser le compteur

    // Vérification que la fréquence mesurée n'est pas "inf"
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
    toneif(frequency, distance);
  }

  //!####################################  Metronome
  // Lecture de la valeur des potentiomètres
  int pot1 = analogRead(PotPin1);
  int pot2 = analogRead(PotPin2);
  
  // Conversion de la valeur de pot1 en intervalle entre les tics
  ticInterval = map(pot1, 0, 1023, 1500, 60) ;
  int testTicInterval = map(pot1, 0, 1023, 1, 210) ;
  
  // Conversion de la valeur de pot2 en fréquence du son
  int frequencyBuzzer = map(pot2, 0, 1023, 20, 20000);

  // Production du son avec la fréquence et la durée déterminées par les potentiomètres
  if (millis() - lastTicTime >= ticInterval - 200 && testTicInterval <= 200)  {
    toneAC(frequencyBuzzer, 10, 10);  
    lastTicTime = millis();
  }
  //!############################################################

  //! Affichage de la valeur du BPM sur l'écran OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("BPM: ");
  display.print(map(pot1, 0, 1023, 1, 200));

  //! Affichage de la valeur de la fréquence sur l'écran OLED
  display.setCursor(0,10);
  display.display();
}