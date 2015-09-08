/******************************************
Moving Head Steuerung

Dieses Skript soll meinen selbstentworfenen Movinghead steuern.
Dazu werden zunächst einmal beide Schrittmotoren angesteuert.
Der untere Motor fährt die Position der über serielle Schnittstelle eingegebenen Wert an.
Dieser Wert darf nicht Null sein, sonst passiert nichts.
  modified 7 September 2015
  by Daniel Friedrich
*******************************************/

#define MAX_SPEED 2 // je kleiner Desto Schneller Maximal
#define MIN_SPEED 50 // je höher Desto Langsamer beim Anfahren und Bremsen
#define Bremszyklen 40
#define RICHTUNGSPIN 8
#define MOTOR1PIN 13
#define STOPPOSITION 1500

unsigned long time;
unsigned long last_move;
unsigned int Wartezeit = 3000;
int i = 0; //Schrittzähler
boolean Richtung = 1; //1 for CCW
int Ziel = 0;
int Bewegung = 0;
int Status = 0;
int Position = 0;

void Setup() {
  //Grundpositionen finden
  Status = 2;
}

void setup() {
  pinMode(RICHTUNGSPIN, OUTPUT);
  pinMode(MOTOR1PIN, OUTPUT);
  Serial.begin(38400);
}

unsigned long Drehungschritt() {
  digitalWrite(MOTOR1PIN, 0);
  delay(1);
  digitalWrite(MOTOR1PIN, 1);
  (Richtung) ? Position++ : Position--;
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

void Move() {
  if ((time - last_move) > Wartezeit) {
    if (i < (Bewegung / 2)) {
      Wartezeit = Beschleunigung(i, Wartezeit);
    }
    else if (i >= (Bewegung / 2) && (Bewegung - i) < Bremszyklen) {
      Wartezeit = Abbremsen(i, Wartezeit);
    }
    last_move = Drehungschritt();
    i++; // Schritt gemacht
    if (i >= Bewegung) {
      Status = 2;
      i = 0;
    }
  }
}

void Pause() {
  Wartezeit = 5000;
  if ((time - last_move) > Wartezeit) {
    last_move = Drehungschritt();
    Richtung = !Richtung;
    digitalWrite(RICHTUNGSPIN, Richtung);
  }
}

void Einlesen() {
  Ziel = Serial.parseInt();
  if (Ziel) {
    Bewegung = Ziel - Position;
    if (Bewegung != 0 && Bewegung != 1 && Bewegung != -1) {
      Status = 1;
      Wartezeit = MIN_SPEED;
      if (Bewegung < 0) {
        Bewegung = Bewegung * (-1);
        Richtung = 0;
        digitalWrite(RICHTUNGSPIN, Richtung);
      }
      else {
        Richtung = 1;
        digitalWrite(RICHTUNGSPIN, Richtung);
      }
    }
  }
}

void loop() {
  while (1) {
    time = millis();
    //    delay(20);
    Serial.println(Position);
    //        Serial.print("\t");
    //        Serial.print(i);
    //        Serial.print("\t");

    switch (Status) {
      case 0: {
          Setup();
          break;
        }
      case 1: {
          Move();
          break;
        }
      case 2: {
          Pause();
          //          Serial.println("\t Status 2");
          Einlesen();
          break;
        }
      default: {
          //          Pause();
        }
    }
  }
}

