#include <Adafruit_SSD1306.h>
#include <Arduino.h>

//!##################################################  DECLARATION DES PINS ET VARIABLES GLOBALES ##################################################
//oled screen
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

//bouton
const int ButtonPin = 4;
bool buttonState = false;

//ultrason
const int TriggerPin = 11;
const int EchoPin = 12;

//Potentiometre
const int PotPin1 = A0;
const int PotPin2 = A1;

//buzzer
const int BuzzerPin = 2;
//!############################################################################################################################


//?##################################################  SETUP  ##################################################
void setup() {
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
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(0.5);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
}
//?############################################################################################################


//*##################################################  LOOP  ##################################################
void loop() {
  //!print text and flip the screen with setRotation(2) every 2 seconds
  display.clearDisplay();
  display.setRotation(0);
  display.setCursor(0,0);
  display.println("Hello World!");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setRotation(2);
  display.setCursor(0,0);
  display.println("Hello World!");
  display.display();
  delay(2000);
}
//*############################################################################################################
