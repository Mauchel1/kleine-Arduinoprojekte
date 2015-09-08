/*
Moving Head Steuerung

Dieses Skript soll meinen selbstentworfenen Movinghead steuern. 
Dazu werden zunächst einmal beide Schrittmotoren angesteuert. 
  modified 7 September 2015
  by Daniel Friedrich
 */
#define MAX_SPEED 3 // je kleiner Desto Schneller Maximal
#define MIN_SPEED 50 // je höher Desto Langsamer beim Anfahren und Bremsen
#define Bremszyklen 40

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(38400);
}

unsigned long Drehungschritt() {
  digitalWrite(13, LOW);
  delay(1);
  digitalWrite(13, HIGH);
  return (millis());
}

unsigned int Beschleunigung(int i, float k) {
  if (i < 2) { // Zurücksetzen von Speed am Anfang
    return (MIN_SPEED);
  }
  else {
    k = k - (k / 10);
  }
  if (k < MAX_SPEED) {
    k = MAX_SPEED;
  }
  return (k);
}

unsigned int Abbremsen(int i, float k) {
  if (k >= MIN_SPEED) {
    return (MIN_SPEED);
  }
  else {
      float g  = k / 15;
      if (g < 1) g = 1;
    k = k + (g);
  }
  return (k);
}

void loop() {
  unsigned long time;
  unsigned long last_move;
  unsigned int Wartezeit = 3000;
  int interval; //Zeit in millisekunden zwischen
  int i = 0; //Schrittzähler
  boolean Richtung = 1; //1 for CW
  int Bewegung = 500;

  while (1) {
    time = millis();

      if ((time - last_move) > Wartezeit) {
      if (i < (Bewegung / 2)) {
        Wartezeit = Beschleunigung(i, Wartezeit);
      }
      else if (i >= (Bewegung / 2) && (Bewegung-i) < Bremszyklen) {
        Wartezeit = Abbremsen(i, Wartezeit);
      }
      last_move = Drehungschritt();
      interval = time - last_move;
      Serial.print (Wartezeit);
      Serial.print ("\t");
      Serial.println (i);
      i++;
      if (i > Bewegung) i = 0;
    }

  }
}


