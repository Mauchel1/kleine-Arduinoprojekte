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
#define RICHTUNG1PIN 8
#define RICHTUNG2PIN 9
#define MOTOR1PIN 13
#define MOTOR2PIN 12
#define STOPPOSITION 1500

unsigned long time;
unsigned long last_move;
unsigned long last_move2;
unsigned int Wartezeit = 3000;
unsigned int Wartezeit2 = 3000;
int i = 0; //Schrittzähler
int i2 = 0; //Schrittzähler
boolean Richtung = 1; //1 for CCW
boolean Richtung2 = 1; //1 for CCW
int Ziel = 0;
int Ziel2 = 0;
int Bewegung = 0;
int Bewegung2 = 200;
int Status = 0;
int Status2 = 0;
int Position = 0;
int Position2 = 0;

void Setup() {
  //Grundpositionen finden
  Status = 2;
  Status2 = 2;
}

void setup() {
  pinMode(RICHTUNG1PIN, OUTPUT);
  pinMode(MOTOR1PIN, OUTPUT);
  pinMode(RICHTUNG2PIN, OUTPUT);
  pinMode(MOTOR2PIN, OUTPUT);
  Serial.begin(38400);
}

unsigned long Drehungschritt() {
  digitalWrite(MOTOR1PIN, 0);
  delay(1);
  digitalWrite(MOTOR1PIN, 1);
  (Richtung) ? Position++ : Position--;
  return (millis());
}

unsigned long Nickschritt() {
  digitalWrite(MOTOR2PIN, 0);
  delay(1);
  digitalWrite(MOTOR2PIN, 1);
  (Richtung2) ? Position2++ : Position2--;
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

void Move_Nicken() {
  if ((time - last_move2) > Wartezeit2) {
    if (i2 < (Bewegung2 / 2)) {
      Wartezeit2 = Beschleunigung(i2, Wartezeit2);
    }
    else if (i2 >= (Bewegung2 / 2) && (Bewegung2 - i2) < Bremszyklen) {
      Wartezeit2 = Abbremsen(i2, Wartezeit2);
    }
    last_move2 = Nickschritt();
    i2++; // Schritt gemacht
    if (i2 >= Bewegung2) {
      Status2 = 2;
      i2 = 0;
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

void Pause_Nicken() {
  Wartezeit2 = 5000;
  if ((time - last_move2) > Wartezeit) {
    last_move2 = Nickschritt();
    Richtung2 = !Richtung2;
    digitalWrite(RICHTUNG2PIN, Richtung2);
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
        digitalWrite(RICHTUNG1PIN, Richtung);
      }
      else {
        Richtung = 1;
        digitalWrite(RICHTUNG1PIN, Richtung);
      }
    }
  }
}

void Einlesen2() {
  Ziel2 = Serial.parseInt();
  if (Ziel2) {
    Bewegung2 = Ziel2 - Position2;
    if (Bewegung2 != 0 && Bewegung2 != 1 && Bewegung2 != -1) {
      Status2 = 1;
      Wartezeit2 = MIN_SPEED;
      if (Bewegung2 < 0) {
        Bewegung2 = Bewegung2 * (-1);
        Richtung2 = 0;
        digitalWrite(RICHTUNG2PIN, Richtung2);
      }
      else {
        Richtung2 = 1;
        digitalWrite(RICHTUNG2PIN, Richtung2);
      }
    }
  }
}

void loop() {
  while (1) {
    time = millis();
    //    delay(20);
    Serial.print(Position);
    Serial.print("\t");
    Serial.println(Position2);
    //        Serial.print("\t");
if (Status != 1 && Status2 != 1) {
    Einlesen();
    Einlesen2();
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
    
    switch (Status2) {
      case 0: {
          Setup();
          break;
        }
      case 1: {
          Move_Nicken();
          break;
        }
      case 2: {
          Pause_Nicken();
          break;
        }
      default: {;
        }
    }
  }
}

