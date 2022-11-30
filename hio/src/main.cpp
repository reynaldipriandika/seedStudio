#include <Arduino.h>
#include <LIS3DHTR.h>
#include <TFT_eSPI.h> 
#include <Free_Fonts.h> // include the header file 
#include <Ultrasonic.h>
TFT_eSPI tft;
LIS3DHTR<TwoWire> lis;
Ultrasonic ultrasonic(0); // pin D0

// *Variables for the pitch average
// Constant won't change.
const int pitchReadingNumber = 10; // the size of the readings  

float pitch[pitchReadingNumber]; // analog input
int pitchIndex = 0; // the index of the current reading 
float totalPitch = 0; // the running total
float averagePitch = 0; // the average 
float previousPitch; // The last reading of the pitch
// float currentPitch;

// *Variables for the pitch delay
const long pitchInterval = 50; // Interval of the reading
unsigned long previousPitchMillis = 0;

const int rangeReadingNumber = 10; 
long rangeInCentimeters[rangeReadingNumber];
long range;
int rangeIndex = 0;
/*
Generally should use "unsigned long" for hadling time.
The value will quickly become too large.
*/
unsigned long previousRange1Millis = 0;
const long range1Interval = 250;
long previousRange1;  // The last reading of the distance

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
    // Serial.print("l:");Serial.print(previousPitch);
    
    totalPitch = totalPitch - previousPitch; // subtract the last reading
    pitch[pitchIndex] = lis.getAccelerationX(); // Get the pitch values
    /*
    Serial.print(" p:"); Serial.print(pitch[pitchIndex]);
    Serial.println();
    */
    // currentPitch = pitch[pitchIndex];
    if (previousPitch != pitch[pitchIndex]) { // smoothing the readings
      /*
      tft.setCursor(70,80);
      tft.fillScreen(TFT_BLACK);
      */
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      // tft.setFreeFont(FF1);
      tft.drawString("x:",60,80,1);
      tft.drawFloat(averagePitch,1,70,80,1); // Print the pitch value
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
    // Serial.println(averagePitch,2);
  }
  // *Read the distance value
  if (currentMillis - previousRange1Millis >= range1Interval){
    previousRange1Millis = currentMillis;
    // previousRange1 = rangeInCentimeters[rangeIndex];
    previousRange1 = range;
    // Serial.print("l:");Serial.print(previousRange1);
    // rangeInCentimeters[rangeIndex] = ultrasonic.MeasureInCentimeters(); // Measure the distance
    range = ultrasonic.MeasureInCentimeters();
    // Serial.print(" r:");Serial.print(range);
    // Serial.println();
    // rangeIndex ++;
    /*
    if (rangeIndex >= rangeReadingNumber){
      rangeIndex = 0;
    }
    */
  }
}