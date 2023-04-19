import processing.serial.*; // importe la bibliothèque pour la communication série

Serial port; // variable pour stocker le port série

int keyCount = 7; // nombre de touches du clavier
int[] keyPos = new int[keyCount]; // tableau pour stocker les positions des touches
int activeKey = -1; // touche active (initialement aucune)

void setup() {
  size(400, 400); // taille de la fenêtre
  port = new Serial(this, "COM5", 9600); // initialise la communication série
  for (int i = 0; i < keyCount; i++) {
    keyPos[i] = width/(keyCount+1)*(i+1); // positionne les touches
  }
}

void draw() {
  background(255); // fond blanc
  stroke(0); // couleur des bordures noire
  strokeWeight(2); // largeur des bordures
  for (int i = 0; i < keyCount; i++) {
    if (i == activeKey-1) {
      fill(200, 0, 0); // couleur de la touche active rouge
    } else {
      fill(200); // couleur des autres touches grise
    }
    rect(keyPos[i]-20, height/2-50, 40, 100, 10); // dessine les touches
  }
}

void serialEvent(Serial port) { // fonction appelée à chaque réception de données série
  if (port.available() > 0) {
    int data = port.read(); // lit l'octet reçu
    if (data >= 49 && data <= 55) { // vérifie que la donnée est entre 1 et 7 inclus
      activeKey = data - 48; // active la touche correspondante (ASCII 49-55 -> int 1-7)
    }
  }
}