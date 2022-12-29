#ifndef Imu_h
#define Imu_h

#include "LIS3DHTR.h"
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
extern TFT_eSPI tft;
class Imu {
   public:
    Imu();
    void begin(),displayImu(const char text[],float data,int poX,int poY);
    float pitch();
};
#endif