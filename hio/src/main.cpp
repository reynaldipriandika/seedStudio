// #include <Arduino.h>
// #include <LIS3DHTR.h>
#include "Imu.h"
#include <TFT_eSPI.h> 
#include <Free_Fonts.h> // include the header file 
#include <Ultrasonic.h>

TFT_eSPI tft;
// LIS3DHTR<TwoWire> lis;
Imu imu;
Ultrasonic ultrasonic(0); // pin D0

// Constant won't change.
const int pitchReadingNumber = 10; // the size of the readings  

float pitch[pitchReadingNumber]; // analog input
int pitchIndex = 0; // the index of the current reading 
float totalPitch = 0; // the running total
float averagePitch = 0; // the average 
float previousPitch; // The last reading of the pitch

const long pitchInterval = 50; // Interval of the reading
unsigned long previousPitchMillis = 0;

/*
Generally should use "unsigned long" for hadling time.
The value will quickly become too large.
*/
unsigned long previousRange1Millis = 0;
const long range1Interval = 250;
long rangeInCentimeters;
long previousRange1;  // The last reading of the distance

unsigned long previousRollMillis = 0;
const long rollInterval = 50;

void setup() {
  Serial.begin(115200);
  imu.begin();
  /*
  lis.begin(Wire1);
  if (!lis){
    Serial.println("ERROR");
    while(1);
  }
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); // scale range
  */
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK); // flicker prevention
  // initialize all the readings to 0
  for (int i=0; i<pitchReadingNumber; i++){
    pitch[i] = 0;
  }
}

void loop() {

  unsigned long currentMillis = millis();
  /*
  *Read the pitch value
  Check to see if it's time to read the pitch value.
  */
  if (currentMillis - previousPitchMillis >= pitchInterval){
    // Save the last time you read the pitch value. 
    previousPitchMillis = currentMillis;
    previousPitch = pitch[pitchIndex]; // the last reading value 
    totalPitch = totalPitch - previousPitch; // subtract the last reading
    pitch[pitchIndex] = imu.pitch();
    // pitch[pitchIndex] = lis.getAccelerationX(); // Get the pitch values
    
    // Prevent the display of too many values
    if (previousPitch != pitch[pitchIndex]) { // smoothing the readings
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString("x:",60,80,1);
      tft.drawFloat(averagePitch,1,73,80,1); // Print the pitch value
      previousPitch = pitch[pitchIndex]; 
    }
    totalPitch += pitch[pitchIndex]; // add readings to the total
    pitchIndex++;
    // if we're at the end of the array
    if (pitchIndex >= pitchReadingNumber) {
      pitchIndex = 0; //wrap around to the beginning
    }
    // Calculate the averange
    averagePitch = totalPitch / pitchReadingNumber;
  }
  // *Read the distance value
  if (currentMillis - previousRange1Millis >= range1Interval){
    previousRange1Millis = currentMillis;
    previousRange1 = rangeInCentimeters;
    rangeInCentimeters = ultrasonic.MeasureInCentimeters();
    
    // Prevent the display of too many values
    if (previousRange1 != rangeInCentimeters) {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString("d1:",60,95,1);
      tft.drawFloat(rangeInCentimeters,1,78,95,1);
      
      previousRange1 = rangeInCentimeters;
    }
  }
}