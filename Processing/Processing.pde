import procseeing.serial.*;

Serial arduinoPort;

float Brut = 0.0;
int Ult = 0;
float Freq = Brut + Ult
int largeur = 900;
int hauteur = 500;
int nbr_touche = 7;
float taille_touche = largeur/nbr_touche;
float[] historique = new float[645];
int compteur = 0;

void setup() {
  size(200, 200);
  arduinoPort = new Serial(this, "COM5", 9600); // Open the first available serial port
  arduinoPort.clear(); // Clear any existing data from the serial buffer
}


void draw() {
  background(255);
  dessin_clavier(Brut);
  
  historique[0] = Freq;
  for (int i = 1; i < historique.length; i++) {
    historique[i] = historique[i-1];
  }
  stroke(255,0,0);
  strokeWeight(2);
  float x1 = 1230;
  float y1 = 125;
  float x2 = 1875;
  float y2 = 400;
  float xStep = (x2 - x1) / (historique.length - 1);
  float yStep = (y2 - y1) / (height - y1);
  for (int i = 0; i < historique.length-1; i++) {
    line(x1 + i * xStep, y2 - historique[i] * yStep, x1 + (i+1) * xStep, y2 - historique[i+1] * yStep);
  }
}

void serialEvent(Serial arduinoPort) {
  String data = arduinoPort.readStringUntil('\n');
  if (data != null) {
    data = data.trim();
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      Ult = int(data.substring(0, commaIndex));
      Freq = float(data.substring(commaIndex + 1));
      
      historique[0] = Freq;
      for (int i = 1; i < historique.length; i++) {
        historique[i] = historique[i-1];
      }
    }
  }
}
void dessin_clavier(int touche){
    fill(0,0,0);
    rect(0, hauteur*0.62, largeur, hauteur);//2 premiers c'est co l/h et les deux dernieres c'est la taille dans chaque directions
    fill(255,255,255);
    for(int i = 0; i < nbr_touche; i++){
      rect(taille_touche*i, hauteur*0.625, taille_touche, hauteur*0.374);}
    
    fill(0,0,0);
    for(int i = 0; i < nbr_touche/7; i++){
      for(int j = 0; j < 2; j++){
        rect(taille_touche*(j + 0.75 + 7*i), hauteur*0.625, taille_touche*0.5, hauteur*0.25);
      }
      for(int j = 0; j < 3; j++){
        rect(taille_touche*(j + 3.75 + 7*i), hauteur*0.625, taille_touche*0.5, hauteur*0.25);
      }
    }
    if (touche<=700){
      touche = 261.0;
      fill(255,0,0);
      rect(0,310,120,200);}
    else if (touche>700 && touche<=800){
      touche = 293.0;
      fill(125,0,0);
      rect(130,310,123,200);}
    else if (touche>800 && touche<=900){
      touche = 329.0; 
      fill(0,255,0);
      rect(258,310,123,200);}
    else if (touche>900 && touche<=1015){
      touche = 349.0;
      fill(0,125,0);
      rect(385,310,123,200);}
    else if (touche>1015 && touche<=1120){
      touche = 392.0;
      fill(0,0,255);
      rect(515,310,123,200);}
    else if (touche>1120 && touche<=1300){
      touche = 440.0
      fill(0,0,125);
      rect(642,310,123,200);}
    else if (touche>1300 && touche<=1400){
      touche = 493.0
      fill(125,125,125);
      rect(770,310,123,200);}
 }
