#include "Imu.h"

LIS3DHTR<TwoWire> lis;
extern TFT_eSPI tft;
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
}

float Imu::pitch(){
   float currentPitch;
   currentPitch = lis.getAccelerationX();
   return currentPitch;
}