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
const int numberOfReadings = 9;
int index = 0;
//                              acc x, y z, mag x, y, z:
float minReadings[numberOfAxis] = 
  {-13.45, -13.18, -10.83, -43.27, -50.0, -56.12};
float maxReadings[numberOfAxis] = 
  {13.61, 11.96, 11.34, 57.09, 49.73, 52.04};
float readings[numberOfAxis];
float sreadings[numberOfAxis][numberOfReadings];
unsigned long lastprint = 0;

void setup (void) {
  Serial.begin(115200);
  Serial.println("Test");
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


  //Serial.print("x- max"); Serial.print(xmax); Serial.print(" ");
  //Serial.print("xflint: "); Serial.print(fl); Serial.print(" ");
  /*Serial.print("xbit: "); */
  /*Serial.print((int)lround(event.acceleration.x*100),BIN); Serial.print(" ");*/
  /*Serial.print("xint: "); Serial.print((int)(event.acceleration.x*100)); Serial.print(" ");*/
  //Serial.println(index); 
  if (millis() - lastprint > 200) {
    lastprint = millis();
    readings[0] = accevent.acceleration.x;
    readings[1] = accevent.acceleration.y;
    readings[2] = accevent.acceleration.z;
    readings[3] = magevent.magnetic.x;
    readings[4] = magevent.magnetic.y;
    readings[5] = magevent.magnetic.z;

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
  if (index++ >= numberOfReadings - 1) { 
    index = 0; 
  }
}


float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float reading(int index) {
  return readings[index];
}