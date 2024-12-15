/**
* @brief: Contains the prototype of the SensorNode class.
* @file: sensornode.hpp
*
* @author: jkieltyka15
*/

#ifndef _SENSOR_NODE_HPP_
#define _SENSOR_NODE_HPP_

// standard libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL6180X.h>
#include <nRF24L01.h>
#include <RF24.h>

// local libraries
#include <Message.h>


#define RF24_CE_PIN 7   // NRF24L01 CE pin assignment
#define RF24_CSN_PIN 8  // NRF24L01 CSN pin assignment

// width in bytes of the radio's address
#define RF24_ADDRESS_WIDTH 4 


// different states of the ToF sensor
enum tof_sensor_status_t {
    NOT_INITIALIZED = 0,
    VACANT = 1,
    OCCUPIED = 2
};


class SensorNode {

    private:

        // unique id of the sensor node
        uint8_t node_id = 0;

        // most recently read status of the sensor
        tof_sensor_status_t sensor_status = NOT_INITIALIZED;

        // VL6180X ToF sensor
        Adafruit_VL6180X sensor = Adafruit_VL6180X();

        // NRF24L01 transciever radio
        RF24 radio = RF24(RF24_CE_PIN, RF24_CSN_PIN);
        uint32_t radio_address = 0;
        uint8_t radio_channel = 0;

        /**
         * @brief Calculates a given sensor node's radio address based on the node ID
         * 
         * @param node_id: ID of node to calculate address for
         * @return The calculated radio address for the node
         */
        uint32_t calculate_radio_address(uint8_t node_id);

        /**
         * @brief Calculates a given sensor node's radio channel based on the node ID
         * 
         * @param node_id: ID of node to calculate channel for
         * @return The calculated radio channel for the node (0-125)
         */
        uint8_t calculate_radio_channel(uint8_t node_id);


    public:

        /**
         * @brief Constructs a SensorNode object
         */
        SensorNode(uint8_t node_id);

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

        /**
         * @brief Transmit update to sensor node or base station.
         * 
         * @param msg: Update message to be transmitted
         * @return True if successfully sent. Otherwise false
         */
        bool transmit_update(UpdateMessage* msg);

        /**
         * @brief Transmit update to sensor node or base station.
         * 
         * @param rx_node_id: ID of receiving node
         * @return True if successfully sent. Otherwise false
         */
        bool transmit_update(uint8_t rx_node_id);

        /**
         * @brief Determine if there is a message available to read
         * 
         * @return True if a message is available. Otherwise false
         */
        bool is_message();

        /**
         * @brief Gets a message from the message queue
         * 
         * @param buffer: buffer to hold message
         * @param size: size of buffer
         * @return Number of bytes read
         */
        bool read_message(uint8_t* buffer, uint8_t size);

        /**
         * @brief Get the ID of the node
         * 
         * @return ID of the node
         */
        uint8_t get_id();
};

#endif /* _SENSOR_NODE_HPP_ */
