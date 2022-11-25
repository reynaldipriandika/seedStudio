#include <Arduino.h>
#include <LIS3DHTR.h>
#include <TFT_eSPI.h> 
#include <Free_Fonts.h> // include the header file 
#include <Ultrasonic.h>
TFT_eSPI tft;
LIS3DHTR<TwoWire> lis;
Ultrasonic ultrasonic(0); // pin D0

const int readingNumber = 10; // the size of the readings  

float pitch[readingNumber]; // analog input
int pitchIndex = 0; // the index of the current reading 
float totalPitch = 0; // the running total
float averagePitch = 0; // the average 
float lastPitch; // The last reading of the pitch

unsigned long previousPitchMillis = 0; 
const long pitchInterval = 50; // Interval of the reading 

unsigned long previousDistance1Millis = 0;
long distance1Interval = 200;

void setup() {
  Serial.begin(115200);
  lis.begin(Wire1);
  if (!lis){
    Serial.println("ERROR");
    while(1);
  }
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); // data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); // scale range
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK); // flicker prevention
  // initialize all the readings to 0
  for (int i=0; i<readingNumber; i++){
    pitch[i] = 0;
  }
}

void loop() {
  unsigned long currentMillis = millis();
  long RangeInCentimeters;
  // *Read the pitch value
  if (currentMillis - previousPitchMillis >= pitchInterval){
    previousPitchMillis = currentMillis;
    lastPitch = pitch[pitchIndex]; // the last reading value 
    // Serial.print("l:");Serial.print(lastPitch,2);
    totalPitch = totalPitch - lastPitch; // subtract the last reading
    pitch[pitchIndex] = lis.getAccelerationX(); // Get the pitch values
    // Serial.print("p:"); Serial.print(pitch[pitchIndex],2);
    if (lastPitch != pitch[pitchIndex]) { // smoothing the readings
      /*
      tft.setCursor(70,80);
      tft.fillScreen(TFT_BLACK);
      */
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      // tft.setFreeFont(FF1);
      tft.drawString("x:",60,80,1);
      tft.drawFloat(averagePitch,1,70,80,1);
      lastPitch = pitch[pitchIndex]; 
    }
    totalPitch += pitch[pitchIndex]; // add readings to the total
    pitchIndex++;
    // if we're at the end of the array
    if (pitchIndex >= readingNumber) {
      pitchIndex = 0; //wrap around to the beginning
    }
    // Calculate the averange
    averagePitch = totalPitch / readingNumber;
    // Serial.println(averagePitch,2);
    // Serial.println(RangeInCentimeters); 
  }
  // *Read the distance value
  if (currentMillis - previousDistance1Millis >= distance1Interval){
    previousDistance1Millis = currentMillis;
    RangeInCentimeters = ultrasonic.MeasureInCentimeters();
  }
}