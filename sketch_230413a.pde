Serial arduino;

void setup() {
  // Initialize serial communication with Arduino
  arduino = new Serial(this, Serial.list()[0], 9600);
  
  // Wait for connection to be established
  delay(2000);
}

int freq = 0;
int largeur = 900;
int hauteur = 500;
int nbr_touche = 7;
float taille_touche = largeur/nbr_touche;

void draw() {
  // Read incoming data from Arduino
  if (arduino.available() >= 6) { // Check that we have at least 6 bytes (2x sizeof(int) + 2x sizeof(float))
    int val1 = arduino.parseInt();
    float val2 = arduino.parseFloat();
  background(255);
  dessin_clavier(freq);
}
void dessin_clavier(int touche){
    fill(0,0,0);
    rect(0, hauteur*0.62, largeur, hauteur);//2 premiers c'est co l/h et les deux dernieres c'est la taille dans chaque directions
    fill(255,255,255);
    for(int i = 0; i < nbr_touche; i++){
      rect(taille_touche*i, hauteur*0.625, taille_touche, hauteur*0.374);
    }
    if (touche!=0){
      fill(255,0,0);
      rect(0,310,120,200);}
    else if (touche==436){
      fill(125,0,0);
      rect(130,310,123,200);}
    else if (touche==465){
      fill(0,255,0);
      rect(258,310,123,200);}
    else if (touche==495){
      fill(0,125,0);
      rect(385,310,123,200);}
    else if (touche==531){
      fill(0,0,255);
      rect(515,310,123,200);}
    else if (touche==578){
      fill(0,0,125);
      rect(642,310,123,200);}
    else if (touche==628){
      fill(125,125,125);
      rect(770,310,123,200);}
    
    fill(0,0,0);
    for(int i = 0; i < nbr_touche/7; i++){
      for(int j = 0; j < 2; j++){
        rect(taille_touche*(j + 0.75 + 7*i), hauteur*0.625, taille_touche*0.5, hauteur*0.25);
      }
      for(int j = 0; j < 3; j++){
        rect(taille_touche*(j + 3.75 + 7*i), hauteur*0.625, taille_touche*0.5, hauteur*0.25);
      }
    }
 
}
