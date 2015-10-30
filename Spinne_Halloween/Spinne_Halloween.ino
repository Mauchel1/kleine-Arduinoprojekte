/******************************************
Spinnensteuerung
Dieses Skript soll die Spinne an Halloween hoch und runter bewegen.
Am besten sogar mit kleinen Unterbrechungen.
  modified 24 October 2015
  by Daniel Friedrich
*******************************************/
//#define MOTORAUS //Wenn definiert ist der Motor aus beim runterfahren - Achtung Werte neu anpassen!
#define RUNTERZEIT 1000 // Zeit, die der Motor runter aus ist

int MAX_SPEED = 1; // je kleiner Desto Schneller Maximal
#define MIN_SPEED 100 // je höher Desto Langsamer beim Anfahren und Bremsen
#define Bremszyklen 40

#define RICHTUNG1PIN 9
#define ENABLE 4
#define GESCHWPIN 7
#define MOTOR1PIN 8

#define LONGWAIT 60000 // in der Zeit kühlt der Motor ab
#define HOCHRUNTEROFFSET 00    //in Schritten
#define FAHRT 1000
#define OBENWARTEN 10000
#define UNTENWARTEN 5000
//#define SERIELL  // für seriellen mode

int Anzahl = 0;
unsigned long time;
unsigned long last_move;
unsigned int Wartezeit = 3000;
int i = 0; //Schrittzähler
boolean Richtung = 0; //1 for CCW
boolean Geschwindigkeit = 1; // 1 for HALF
int Ziel = 0;
int Bewegung = 0;
int Status = 0;
int Position = 0;

void setup() {
  pinMode(ENABLE, OUTPUT);
  pinMode(GESCHWPIN, OUTPUT);
  pinMode(RICHTUNG1PIN, OUTPUT);
  pinMode(MOTOR1PIN, OUTPUT);
  Serial.begin(38400);
  digitalWrite(ENABLE, 1);
  digitalWrite(GESCHWPIN, 1);
  digitalWrite(RICHTUNG1PIN, 1);
}

unsigned long Drehungschritt() {
  digitalWrite(MOTOR1PIN, 0);
  delayMicroseconds(300);
  digitalWrite(MOTOR1PIN, 1);
  Position++;
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

void Einlesen() {
#ifdef SERIELL
  Ziel = Serial.parseInt();
#else
  Ziel = FAHRT;
#endif

#ifdef MOTORAUS
  delay(OBENWARTEN);
  digitalWrite(ENABLE, 0);
  delay(RUNTERZEIT);
  digitalWrite(ENABLE, 1);
  delay(UNTENWARTEN);

#else
  if (Richtung) { 
    delay(OBENWARTEN);
    digitalWrite(RICHTUNG1PIN, 0);
    digitalWrite(GESCHWPIN, 0);    
    Richtung = 0;
  }
  else {
    delay(UNTENWARTEN);
    digitalWrite(RICHTUNG1PIN, 1);
    digitalWrite(GESCHWPIN, 1);
    Ziel = Ziel * 2 + HOCHRUNTEROFFSET;
    Richtung = 1;
    Anzahl++;
    if (Anzahl % 5 == 0) {
      digitalWrite(ENABLE, 0);
      delay(LONGWAIT);
      digitalWrite(ENABLE, 1);
    }
  }
  
#endif
  if (Ziel) {
    Bewegung = Ziel - Position;
    Status = 1;
    Wartezeit = MIN_SPEED;
  }
}

void loop() {
  while (1) {
    time = millis();
    //Serial.println(Position);
    Serial.println(Anzahl);
    if (Status != 1) {
      Position = 0;
      Einlesen();
    }
    Move_Drehung();
  }
}
