/*
Dieses Programm soll ähnlich einer Lichtschranke Personen zählen
Auch soll die Anzahl von Knopfdrücken gezählt werden. 
Die Anzeige erfolgt mit einer 3fachen 7 Segment Anzeige
*/

    #define SEGMENT1_PIN 8 // Ground von Segment 1
    #define SEGMENT2_PIN 9 // Ground von Segment 2
    #define SEGMENT3_PIN 10 // Ground von Segment 3
    #define CLOCK_PIN 7 // toggeln während der Datenübertragung
    #define LATCH_PIN 6 // vor dem Senden der Daten an den IC auf LOW ziehen, danach wieder HIGH
    #define DATA_PIN 5  // an den seriellen Eingang des IC; jeweils 1 Byte übertragen
    #define LIGHT_PIN 4
   
    uint8_t zahlen[10] = {0x3f,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

    int zahl = 0;
    int einer = 0;
    int zehner = 0;
    int hunderter = 0;
    unsigned long time = 0;
    int licht = 0;
    int lichtalt = 0;
    int flanke = 0;
    int wartezeit = 1200; // blindzeit des Lichtsensors in Millisekunden

void setup() {
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(SEGMENT1_PIN, OUTPUT);
    pinMode(SEGMENT2_PIN, OUTPUT);
    pinMode(SEGMENT3_PIN, OUTPUT);
    pinMode(LIGHT_PIN, INPUT);  

}


void write_to_segments(){
    einer = zahl % 10;
    zehner = (zahl / 10) % 10;
    hunderter = (zahl / 100) % 10;
    digitalWrite(LATCH_PIN,HIGH);
    
    //HUNDERTER
        digitalWrite(SEGMENT1_PIN,LOW);
        digitalWrite(SEGMENT2_PIN,HIGH);
        digitalWrite(SEGMENT3_PIN,HIGH);
        delay(5);
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,zahlen[hunderter]);
    
    //ZEHNER
        digitalWrite(SEGMENT1_PIN,HIGH);
        digitalWrite(SEGMENT2_PIN,LOW);
        digitalWrite(SEGMENT3_PIN,HIGH);
        delay(5);
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,zahlen[zehner]);
        
    //EINER
        digitalWrite(SEGMENT1_PIN,HIGH);
        digitalWrite(SEGMENT2_PIN,HIGH);
        digitalWrite(SEGMENT3_PIN,LOW);
        delay(5);
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,zahlen[einer]);
}

void light_status(){
    licht = digitalRead(LIGHT_PIN);
    if (licht != lichtalt && licht) {
        if (millis() - time > wartezeit){
            zahl++;
            time = millis();
        }
    }
    lichtalt = licht;
}

void loop() {
    light_status();
    write_to_segments();
}
