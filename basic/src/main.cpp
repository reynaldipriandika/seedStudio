#include <Arduino.h>

#define LED1 D3
#define LED2 LED_BUILTIN
#define BUTTON WIO_KEY_A
#define POTENSIO D4

unsigned long previousTimeLed1 = 0; // last time LED was updated
const long intervalLed1 = 1000; // interval at which to blink
unsigned long previousTimePot = 0;
const long intervalPot = 2000;
int ledState1 = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
}

void loop() {
  unsigned long currentMilis = millis();
  // task 1
  if (currentMilis - previousTimeLed1 >= intervalLed1){
    
    /*
    Serial.print("p:");Serial.print(previousTimeLed1);
    Serial.print("c:");Serial.print(currentMilis);
    Serial.println();
    */
    // save the last time blinked the LED
    previousTimeLed1 = currentMilis; 
    if (ledState1 == HIGH) {
      ledState1 = LOW;
    }
    else {
      ledState1 = HIGH;
    }
    digitalWrite(LED1, ledState1);
  }
  // task 2
  if (digitalRead(BUTTON) == HIGH){
    digitalWrite(LED2, HIGH);
  }
  else{
    digitalWrite(LED2, LOW);
  }
  // *task 3
  if (currentMilis - previousTimePot >= intervalPot) {
    previousTimePot = currentMilis;
    Serial.println(POTENSIO);
  }
}
