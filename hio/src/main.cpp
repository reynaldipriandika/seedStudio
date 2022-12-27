#include "Imu.h"
#include <Ultrasonic.h>

extern TFT_eSPI tft;
// LIS3DHTR<TwoWire> lis;
Imu imu;
Ultrasonic ultrasonic(0); // pin D0

// Constant won't change
const int pitchReadingNumber = 10; // the size of the readings  

const long pitchIntervalTime = 50; // Interval of the reading
unsigned long previousPitchTime = 0;

// The value will quickly become too large.
unsigned long previousRange1Time = 0;
const long range1IntervalTime = 250;

long rangeInCentimeters;
long previousRange1;  // The last reading of the distance

void setup() {
  Serial.begin(115200);
  imu.begin();
}

void loop() {

  unsigned long currentTime = millis();
  if (currentTime - previousPitchTime >= pitchIntervalTime){
    previousPitchTime = currentTime;
    // Serial.println(imu.pitch(),2);
    imu.displayPitch(60,80);
  }
  
  // Read the distance value
  if (currentTime - previousRange1Time >= range1IntervalTime){
    previousRange1Time = currentTime;
    previousRange1 = rangeInCentimeters;
    rangeInCentimeters = ultrasonic.MeasureInCentimeters();
    // rangeInCentimeters = imu.pitch();

    // Prevent the display of too many values
    if (previousRange1 != rangeInCentimeters) {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString("d1:",60,95,1);
      tft.drawFloat(rangeInCentimeters,1,78,95,1);
      
      previousRange1 = rangeInCentimeters;
    }
  }
}