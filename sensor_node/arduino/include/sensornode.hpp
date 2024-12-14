/**
* @brief: Contains the prototype of the SensorNode class.
* @file: sensornode.hpp
*
* @author: jkieltyka15
*/

#ifndef _SENSOR_NODE_HPP_
#define _SENSOR_NODE_HPP_

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_VL6180X.h"


// different states of the ToF sensor
enum tof_sensor_status_t {
    NOT_INITIALIZED = 0,
    VACANT = 1,
    OCCUPIED = 2
};


class SensorNode {

    private:

        // most recently read status of the sensor
        tof_sensor_status_t sensor_status = NOT_INITIALIZED;

        // VL6180X ToF sensor
        Adafruit_VL6180X sensor = Adafruit_VL6180X();


    public:

        /**
         * @brief Constructs a SensorNode object
         */
        SensorNode();

        /**
         * @brief Initializes all variables and objects
         * 
         * Initializes all variables and objects for a SensorNode
         * object including the ToF sensor and transciever.
         * 
         * @return true on success. Otherwise false
         */
        bool init();

        /**
         * @brief Gets the most recently read ToF sensor status
         * 
         * @return Most recently read ToF sensor status
         */
        tof_sensor_status_t get_sensor_status();

        /**
         * @brief Determines if the last read sensor value differs from the current
         * 
         * Determines if the last read sensor value differs from the current. If a
         * sensor error occurs, false will be returned regardless of the prevous
         * sensor status.
         * 
         * @return If the ToF sensor status has changed since it was last read
         */
        bool is_sensor_status_changed();
};

#endif /* _SENSOR_NODE_HPP_ */