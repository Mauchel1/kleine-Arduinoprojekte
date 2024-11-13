#define MOVE1PIN 5
#define MOVE2PIN 6
#define LIGHTPIN 4
#define OUTPIN  2
#define PIN_LED 13
//#define BUTTON_PIN 15

bool out = false, outstatus = false;
bool movementDetected = false;
bool lightDetected = false;
bool night = true;

unsigned long previousMillis = 0, lastLightMillis = 0; 
unsigned long interval = 100000; //reset time after light goes out
const unsigned long lightInterval = 100000; // time after constant dark to get night


void setup() {
  // put your setup code here, to run once:

  pinMode(PIN_LED, OUTPUT);
  pinMode(OUTPIN,  OUTPUT);
  pinMode(MOVE1PIN, INPUT);
  pinMode(MOVE2PIN, INPUT);
  pinMode(LIGHTPIN, INPUT);
  //pinMode(BUTTON_PIN, INPUT_PULLUP);


  digitalWrite(OUTPIN, outstatus);
  digitalWrite(PIN_LED, outstatus);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Brightness change to: " + String(lightDetected));
  unsigned long currentMillis = millis();
  
  // read sensors
  
  lightDetected = getLight();
  movementDetected = checkMovement();
  night = isNight(currentMillis);


  //light on after movement in the dark

  if (night) //night
  {
    if (movementDetected)
    {
      out = true;
      previousMillis = currentMillis;
      Serial.println("Lights on");
    }
  }

  //reset light after time
  if (out)
  {
    if (currentMillis - previousMillis >= interval) 
    {
      out = false;
      previousMillis = currentMillis;
      Serial.println("Lights off");
    }
  }

  writeOutput();
}



bool isNight (unsigned long currentMillis) {

  if (lightDetected) //Tag
  {
    lastLightMillis = currentMillis;
    return false;
  }
  else if (currentMillis - lastLightMillis >= lightInterval) 
  {
    return true;
  }

  return false;
}

bool checkMovement() {
  return getMovementMotionsensor(MOVE1PIN) || getMovementMotionsensor(MOVE2PIN);
}

bool getMovementMotionsensor(int sensorpin) {
  return digitalRead(sensorpin);
}

bool getLight() {
  return digitalRead(LIGHTPIN);
}

void writeOutput () {
  if (out != outstatus)
  {
    outstatus = out;
    digitalWrite(OUTPIN, outstatus);
    digitalWrite(PIN_LED, outstatus);
  }
}
