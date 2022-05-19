#include <Button.h> //https://github.com/madleech/Button

const int HORN_PIN = 6; // oder 7 oder 8
const int LED_PIN = LED_BUILTIN;
const int BUTTON_PIN_1 = 15;
const int BUTTON_PIN_2 = 16;
const int BUTTON_PIN_3 = 17;
const int BUTTON_PIN_4 = 18;
const int BUTTON_PIN_5 = 19;

Button button1(BUTTON_PIN_1);
Button button2(BUTTON_PIN_2);
Button button3(BUTTON_PIN_3);
Button button4(BUTTON_PIN_4);
Button button5(BUTTON_PIN_5);

int hornState = LOW;                  // used to set the LED / Horn
int lastHornState = LOW;              // used to set the LED / Horn

unsigned long previousMillis = 0;     // will store last time LED was updated

long interval = 1000;           // interval at which to honk (milliseconds)
const long shortInterval = 1000;             // interval at which to blink (milliseconds)
const long longInterval = 4000;             // interval at which to blink (milliseconds)
const long mistOnInterval = 4000;             // interval at which to blink (milliseconds)
const long mistOffInterval = 80000;           // interval at which to blink (milliseconds)
const long sosShortInterval = 1000;          
const long sosLongInterval = 4000;           
const long sosShortOffInterval = 1000;          
const long sosLongOffInterval = 80000;    
int sosState = 1;      

void setup() 
{
    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    button5.begin();

  // Define pin #13 as output, for the LED
  pinMode(LED_PIN, OUTPUT);
  // Define pin #6 as output, for the HORN
  pinMode(HORN_PIN, OUTPUT);

  digitalWrite(LED_PIN, hornState);
  digitalWrite(HORN_PIN, hornState);
}

void loop() 
{

  unsigned long currentMillis = millis();

  if (button1.read() == Button::PRESSED) //manual horn press
  {
    hornState = HIGH;
  } 
  else if (button1.released())
  {
    previousMillis = currentMillis;
    hornState = LOW;
  }
  else if (button2.pressed()) 
  {
     digitalWrite(HORN_PIN, HIGH);
     digitalWrite(LED_PIN, HIGH);
     delay(1000);
     digitalWrite(HORN_PIN, LOW);
     digitalWrite(LED_PIN, LOW);
     hornState = LOW;
  }
  else if (button3.pressed()) 
  {
     digitalWrite(HORN_PIN, HIGH);
     digitalWrite(LED_PIN, HIGH);
     delay(4000);
     digitalWrite(HORN_PIN, LOW);
     digitalWrite(LED_PIN, LOW);
     hornState = LOW;
  }
  else if (button4.read() == Button::PRESSED) //nebelhorn einzelnes tuten
  {
    if (currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;
      
      if (hornState == LOW) 
      {
        hornState = HIGH;
        interval = mistOnInterval;
      } 
      else 
      {
        hornState = LOW;
        interval = mistOffInterval;
      }
    }
  }
  else if (button4.released())
  {
    previousMillis = currentMillis;
    interval = 10;
    hornState = LOW;
  }
  else if (button5.read() == Button::PRESSED) //sos
  {
    if (currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;
      switch (sosState)
      {
        case 1:
        case 2:
        case 3:
        case 7:
        case 8:
        if (hornState == LOW)
          {
            interval = sosShortInterval;
          }
          else
          {
            interval = sosShortOffInterval;
            sosState ++;
          }
          break;
        case 4:
        case 5:
        case 6:
        if (hornState == LOW)
          {
            interval = sosLongInterval;
          }
          else
          {
            interval = sosShortOffInterval;
            sosState ++;
          }
          break;
        case 9:
        if (hornState == LOW)
          {
            interval = sosShortInterval;
          }
          else
          {
            interval = sosLongOffInterval;
            sosState = 1;
          }
          break;
        default: 
          break;
      }
      hornState = !hornState;
    }
  }
  else if (button5.released())
  {
    previousMillis = currentMillis;
    hornState = LOW;
    interval = 10;
    sosState = 1;
  }
  else 
  {
    hornState = LOW;
  }

  if (lastHornState != hornState)
  {
     digitalWrite(HORN_PIN, hornState);
     digitalWrite(LED_PIN, hornState);
  }
  
  lastHornState = hornState;
}
