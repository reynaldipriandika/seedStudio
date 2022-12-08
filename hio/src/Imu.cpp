#include "Imu.h"

LIS3DHTR<TwoWire> lis;
Imu::Imu(){
}

void Imu::begin(){
   lis.begin(Wire1);
   if(!lis){
      Serial.println("ERROR");
      while(1);
   }
   lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
   lis.setFullScaleRange(LIS3DHTR_RANGE_2G);
}

float Imu::pitch(){
   float currentPitch;
   currentPitch = lis.getAccelerationX();
   return currentPitch;
}