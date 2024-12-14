/**
* @brief: Contains the implementation of the SensorNode class.
* @file: sensornode.cpp
*
* @author: jkieltyka15
*/

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_VL6180X.h"

#include "sensornode.hpp"
#include "log.hpp"


SensorNode::SensorNode() {

}


bool SensorNode::init() {

    // boot ToF sensor
    if (false == sensor.begin()) {

        ERROR("Failed to boot ToF sensor")
        return false;
    }

    return true;
}


tof_sensor_status_t SensorNode::get_sensor_status() {

    return this->sensor_status;
}


bool SensorNode::is_sensor_status_changed() {

    // get range from sensor
    (void) this->sensor.readRange();
    uint8_t status = this->sensor.readRangeStatus();

    // sensor read occupied
    if (VL6180X_ERROR_NONE == status) {

        // status of parking spot did not change
        if (OCCUPIED == this->sensor_status) {
            return false;
        }

        // status of parking space changed
        INFO("parking space is now occupied")
        this->sensor_status = OCCUPIED;
    }

    // sensor read vacant
    else if (VL6180X_ERROR_NOCONVERGE == status) {
        
        // status of parking spot did not change
        if (VACANT == this->sensor_status) {
            return false;
        }

        // status of parking space changed
        INFO("parking space is now vacant")
        this->sensor_status = VACANT;
    }

    // sensor error occured
    else {
        WARN("ToF sensor read error");
        return false;
    }

    return true;
}