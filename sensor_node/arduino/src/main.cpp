/**
* @brief: Contains the main run loop for the parking sensor node.
* @file: main.cpp
*
* @author: jkieltyka15
*/

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_VL6180X.h"

#include "log.hpp"
#include "sensornode.hpp"

// baud rate for serial connection
#define SERIAL_BAUD 9600

// delay in main loop
#define MAIN_LOOP_DELAY_MS 500


// parking sensor node
SensorNode node = SensorNode();


/**
 * @brief Initialize all necessary objects and variables.
 */
void setup() {

    Serial.begin(SERIAL_BAUD);
  
    // initialize the sensor node
    if (false == node.init()) {

        ERROR("Failed to initialize sensor node. Check hardware")

        // hang since failure is not recoverable
        while(1);
    }

    INFO("setup complete")
}


/**
 * @brief Main program run loop.
 * 
 * Continuously monitors the status of a parking space and sends a message if
 * it changes or the status is requested. Additionally, relays messages from
 * other nodes.
 */
void loop() {

    (void) node.is_sensor_status_changed();

    delay(MAIN_LOOP_DELAY_MS);
}