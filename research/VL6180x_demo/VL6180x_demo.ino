/**
* @brief: A simple VL6180x ToF sensor example using an Arduino Nano.
* @file: VL6180x_demo.ino
*
* @author: jkieltyka15
*/


#include <Wire.h>
#include "Adafruit_VL6180X.h"


Adafruit_VL6180X tof_sensor = Adafruit_VL6180X();


void setup() {

  Serial.begin(9600);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  
  // boot ToF sensor
  if (false == tof_sensor.begin()) {

    Serial.println("*ERROR* Failed to boot VL6180x");

    // hang since failure is not recoverable
    while(1);
  }

  Serial.println("setup complete");
}


void loop() {

  // get range from ToF sensor
  uint8_t range = tof_sensor.readRange();
  uint8_t status = tof_sensor.readRangeStatus();

  // sensor value is valid
  if (VL6180X_ERROR_NONE == status) {

      Serial.print("range: ");
      Serial.print(range);
      Serial.println(" mm");
  }

  // out of ToF sensor's range
  else if (VL6180X_ERROR_NOCONVERGE == status) {
    Serial.println("out of range");
  }

  // sensor error occured
  else {
    Serial.println("error");
  }

  delay(500);
}