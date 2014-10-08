//https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout/calibration
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>



Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float xmax = 0;
float x = 0;
const float eGravity = 9.78;
const float compass = 60.0;
const int numberOfAxis = 6;
const int numberOfReadings = 18;
int index = 0;
// readings from the calibration run.
//acc x, y z, mag x, y, z:
float minReadings[numberOfAxis] = 
  {-13.45, -13.18, -10.83, -43.27, -50.0, -56.12};
float maxReadings[numberOfAxis] = 
  {13.61, 11.96, 11.34, 57.09, 49.73, 52.04};
float readings[numberOfAxis];
float readingSums[numberOfAxis];
float sreadings[numberOfAxis][numberOfReadings];
unsigned long lastprint = 0;



void setup (void) {
  Serial.begin(115200);
  if (!mag.begin()) { Serial.println("w: mag"); }
  if (!accel.begin()) { Serial.println("w: accel"); }
  if (!accel.begin() || !mag.begin()) {
    Serial.println("din't work out, did it?");
    while(1);
  }
} 



void loop(void) {
  sensors_event_t accevent;
  sensors_event_t magevent;
  accel.getEvent(&accevent);
  mag.getEvent(&magevent);


  readings[0] = accevent.acceleration.x;
  readings[1] = accevent.acceleration.y;
  readings[2] = accevent.acceleration.z;
  readings[3] = magevent.magnetic.x;
  readings[4] = magevent.magnetic.y;
  readings[5] = magevent.magnetic.z;
  populateReadings();

  Serial.print(map(reading(0), minReadings[0],
        maxReadings[0], -eGravity, eGravity));
  Serial.print(":");
  Serial.print(map(reading(1), minReadings[1],
        maxReadings[1], -eGravity, eGravity)); 
  Serial.print(":");
  Serial.print(map(reading(2), minReadings[2],
        maxReadings[2], -eGravity, eGravity)); 
  Serial.print(":");

  Serial.print(map(reading(3), minReadings[3],
        maxReadings[3], -compass, compass)); 
  Serial.print(":");
  Serial.print(map(reading(4), minReadings[4],
        maxReadings[4], -compass, compass)); 
  Serial.print(":");
  Serial.print(map(reading(5), minReadings[5],
        maxReadings[5], -compass, compass)); 
  Serial.println();
}



float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



float populateReadings() {
  for (int i = 0; i < numberOfAxis; i++) {
    sreadings[i][index] = readings[i];
    if (index == numberOfReadings - 1) {
      readingSums[i] -= sreadings[i][0];
    }
    else {
      readingSums[i] -= sreadings[i][index + 1];
    }
    readingSums[i] += readings[i];
  }
  if (index++ >= numberOfReadings - 1) { 
    index = 0; 
  }
}



float reading(int lindex) {
  return readingSums[lindex]/numberOfReadings;
}