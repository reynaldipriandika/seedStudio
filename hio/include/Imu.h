#ifndef Imu_h
#define Imu_h

#include "LIS3DHTR.h"
#include "Arduino.h"

class Imu {
   public:
    Imu();
    void begin();
    float pitch();
};
#endif