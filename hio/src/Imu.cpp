#include "Imu.h"

LIS3DHTR<TwoWire> lis;
TFT_eSPI tft;

// Constant won't change
const int readingNumber = 10; // The size of the readings

float currentPitch[readingNumber],getData[readingNumber];
float previousPitch,previousData;
float total = 0; // The running total

int readIndex = 0, getIndex = 0; // The index of the current reading
const long intervalTime = 50; // Interval of the reading

unsigned long previousTime = 0;

Imu::Imu()
{
   
}

void Imu::begin()
{
   lis.begin(Wire1);
   if(!lis)
   {
      Serial.println("ERROR");
      while(1);
   }
   lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // data output rate
   lis.setFullScaleRange(LIS3DHTR_RANGE_2G); // scale range
   
   tft.begin();
   tft.setRotation(3);
   tft.fillScreen(TFT_BLACK);
   tft.setTextColor(TFT_WHITE,TFT_BLACK);

   // Initialize all the readings to 0
   for (int i=0; i<readingNumber; i++)
   {
      currentPitch[i] = 0;
      getData[i] = 0;
   }
}

float Imu::pitch() 
{
   // static float pitchAverage;
   static float average;
   
   // Save the last time you read the pitch value
   previousPitch = currentPitch[readIndex]; // The last reading value
   total = total - previousPitch; // Subtract the last reading
   currentPitch[readIndex] = lis.getAccelerationX();

   /*
   if (previousPitch != currentPitch[readIndex]) 
   {
      pitchAverage = average;
      previousPitch = currentPitch[readIndex];
   }
   */
   // !Fail to return values
   // return pitchAverage;
   // else return 0;
   
   total += currentPitch[readIndex];
   readIndex++;
   // If we're at the end of the array
   if (readIndex >= readingNumber) 
   {
      // Wrap around to the beginning
      readIndex = 0; 
   }
   // Smoothing the reading
   average = total/readingNumber;
   return average;
}

void Imu::displayImu(const char text[],float data,int poX,int poY)
{
   unsigned long currentTime = millis();
   // Check to see if it's time to read data
   if (currentTime - previousTime >= intervalTime)
   {
      previousTime = currentTime;

      // Save the last time you read the pitch value
      previousData = getData[getIndex];
      getData[getIndex] = data;
      
      // Prevent the display too many values
      if (previousData != getData[getIndex])
      {
         tft.drawString(text,poX,poY,1);
         tft.drawFloat(data,2,poX+20,poY,1);
         
         previousData = getData[getIndex];
      }
      
      getIndex++;
      // If we're at the end of the array
      if (getIndex >= readingNumber)
      {
         // Wrap around to the beginning
         getIndex = 0;
      }
      Serial.println(data,2);
   }
}