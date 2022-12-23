#include "Imu.h"

LIS3DHTR<TwoWire> lis;
TFT_eSPI tft;

// Constant won't change
const int readingNumber = 10; // The size of the readings

float currentPitch[readingNumber];
float previousPitch;
float total = 0; // The running total

int readIndex = 0; // The index of the current reading
const long intervalTime = 50; // Interval of the reading

Imu::Imu(){
}

void Imu::begin(){
   lis.begin(Wire1);
   if(!lis){
      Serial.println("ERROR");
      while(1);
   }
   lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // data output rate
   lis.setFullScaleRange(LIS3DHTR_RANGE_2G); // scale range
   
   tft.begin();
   tft.setRotation(3);
   tft.fillScreen(TFT_BLACK);
   // tft.setTextColor(TFT_WHITE,TFT_BLACK);

   // Initialize all the readings to 0
   for (int i=0; i<readingNumber; i++) {
      currentPitch[i] = 0;
   }
}

float Imu::pitch() {
   // static float pitchAverage;
   static float average;
   // unsigned long previousTime = 0;
   // unsigned long currentTime = millis();
   
   // Check to see if it's time to read pitch
   // if (currentTime - previousTime >= intervalTime) {
   // previousTime = currentTime;
   // Save the last time you read the pitch value.
   previousPitch = currentPitch[readIndex]; // The last reading value
   total = total - previousPitch; // Subtract the last reading
   currentPitch[readIndex] = lis.getAccelerationX();
   // return currentPitch[readIndex];
  
   // Prevent the display too many values
   if (previousPitch != currentPitch[readIndex]) {
      // pitchAverage = average;
      previousPitch = currentPitch[readIndex];
   }
   // return pitchAverage;
   // else return 0;
   total += currentPitch[readIndex];
   readIndex++;
   // If we're at the end of the array
   if (readIndex >= readingNumber) {
      readIndex = 0; // Wrap around to the beginning
   }
   // Smoothing the reading
   average = total/readingNumber;
   return average;
   // }
   // else return 0;
}