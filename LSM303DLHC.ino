#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>


Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float xmax = 0;
float x = 0;

void setup (void) {
  Serial.begin(9600);
  Serial.println("Test");
  if (!mag.begin()) { Serial.println("w: mag"); }
  if (!accel.begin()) { Serial.println("w: accel"); }
  if (!accel.begin() || !mag.begin()) {
    Serial.println("din't work out, did it?");
    while(1);
  }
}

void loop(void) {
  sensors_event_t event;
  accel.getEvent(&event);

  x = fabs(event.acceleration.x);
  if (x > xmax) { xmax = x; }
  int fl = *(int*)&x;

  //Serial.print("x- max"); Serial.print(xmax); Serial.print(" ");
  //Serial.print("xflint: "); Serial.print(fl); Serial.print(" ");
  Serial.print("sizeof(x): "); Serial.print(sizeof(x)); Serial.print(" ");
  Serial.print("accx: "); Serial.print(event.acceleration.x); Serial.print(" ");
  Serial.print("accy: "); Serial.print(event.acceleration.y); Serial.print(" ");
  Serial.print("accz: "); Serial.print(event.acceleration.z); Serial.print(" ");
  mag.getEvent(&event);
  Serial.print("magx: "); Serial.print(event.magnetic.x); Serial.print(" ");
  Serial.print("magy: "); Serial.print(event.magnetic.y); Serial.print(" ");
  Serial.print("magz: "); Serial.print(event.magnetic.z); Serial.print(" ");
  Serial.println();
  delay(50);
}