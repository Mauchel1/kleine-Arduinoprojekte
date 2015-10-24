/******************************************
Spinnensteuerung

Dieses Skript soll die Spinne an Halloween hoch und runter bewegen.
Am besten sogar mit kleinen Unterbrechungen.
  modified 24 October 2015
  by Daniel Friedrich
*******************************************/

#define MAX_SPEED 2 // je kleiner Desto Schneller Maximal
#define MIN_SPEED 50 // je höher Desto Langsamer beim Anfahren und Bremsen
#define Bremszyklen 40
#define RICHTUNG1PIN 9
#define ENABLE 4
#define GESCHWPIN 7
#define MOTOR1PIN 8
#define STOPPOSITION 1500
#define HOCHFAHRT 600
//#define SERIELL  // für seriellen mode
#define POSITIONENANZAHL 2

unsigned long time;
unsigned long last_move;
unsigned int Wartezeit = 3000;
int i = 0; //Schrittzähler
boolean Richtung = 1; //1 for CCW
boolean Geschwindigkeit = 1; // 1 for HALF
int Ziel = 0;
int Bewegung = 0;
int Status = 0;
int Position = 0;
int Wartezeiten[POSITIONENANZAHL] = {3000, 5000};
int Positionen[POSITIONENANZAHL] = {HOCHFAHRT, 1};
int Positionstatus = 0;

void Setup() {
  //Grundpositionen finden
  Status = 2;
}

void setup() {
  pinMode(ENABLE, OUTPUT);
  pinMode(GESCHWPIN, OUTPUT);
  pinMode(RICHTUNG1PIN, OUTPUT);
  pinMode(MOTOR1PIN, OUTPUT);
  Serial.begin(38400);
  digitalWrite(ENABLE, 1);
  digitalWrite(GESCHWPIN, 1);
}

unsigned long Drehungschritt() {
  digitalWrite(MOTOR1PIN, 0);
  delay(1);
  digitalWrite(MOTOR1PIN, 1);
  if (Richtung){
  Position++;
  }
  else
  Position--;
  //(Richtung) ? Position++ : Position--;
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

void Move_Drehung() {
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

void Pause_Drehung() {
  Wartezeit = 5000;
  if ((time - last_move) > Wartezeit) {
    last_move = Drehungschritt();
    Richtung = !Richtung;
    digitalWrite(RICHTUNG1PIN, Richtung);
  }
}

void Einlesen() {
#ifdef SERIELL
  Ziel = Serial.parseInt();
#else
  Ziel = Vorgabe();
#endif
  if (Ziel) {
    Bewegung = Ziel - Position;
    if (Bewegung != 0 && Bewegung != 1 && Bewegung != -1) {
      Status = 1;
      Wartezeit = MIN_SPEED;
      if (Bewegung < 0) {
        Bewegung = Bewegung * (-1);
        Richtung = 0;
        digitalWrite(RICHTUNG1PIN, Richtung);
      }
      else {
        Richtung = 1;
        digitalWrite(RICHTUNG1PIN, Richtung);
      }
    }
  }
}

int Vorgabe() {
  if (Positionstatus >= POSITIONENANZAHL ) { //aus dem array gelaufen
    Positionstatus = 0;
  }
  Positionstatus++;
  delay(Wartezeiten[Positionstatus - 1]); 
  //Wartezeit = Wartezeiten[Positionstatus - 1]; //geht irgendwie nicht... kp warum
  return (Positionen[Positionstatus - 1]);
}

void loop() {
  while (1) {
    time = millis();
    //    delay(20);
    Serial.println(Position);
    if (Status != 1) {
      Einlesen();
    }

    switch (Status) {
      case 0: {
          Setup();
          break;
        }
      case 1: {
          Move_Drehung();
          break;
        }
      case 2: {
          Pause_Drehung();
          break;
        }
      default: {
          //          Pause();
        }
    }
  }
}

