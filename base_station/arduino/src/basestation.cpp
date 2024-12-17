/**
* @brief: Contains the implementation of the BaseStation class.
* @file: basestation.cpp
*
* @author: jkieltyka15
*/

// standard libraries
#include <Arduino.h>
#include <stdlib.h>
#include <Wire.h>

// local libraries
#include <Log.h>
#include <Message.h>

// local dependencies
#include "basestation.hpp"


// base station's node ID
#define BASE_STATION_ID 0

// special base station address since 0x00000000 is not a valid address
#define BASE_STATION_ADDRESS 0xBAD1DEA5

#define RF24_CHANNEL_SPACING 5  // number of channels between a valid node channel
#define RF24_READING_PIPE 1     // reading pipe for the NRF24L01

#define MAX_SEND_ATTEMPTS 15    // maximum number of attempts to send a message
#define FAILED_SEND_DELAY 15    // minimum delay between sending message attempts


BaseStation::BaseStation(uint8_t node_id) {

    this->node_id = node_id;

    this->radio_address = this->calculate_radio_address(node_id);
    this->radio_channel = this->calculate_radio_channel(node_id);
}


uint32_t BaseStation::calculate_radio_address(uint8_t node_id) {

    // base station has special non-calculated address
    if (BASE_STATION_ID == node_id) {
        return BASE_STATION_ADDRESS;
    }

    // generate address as byte array
    const byte address_bytes[RF24_ADDRESS_WIDTH] = { node_id, node_id, node_id, node_id };

    // convert byte array to 32 bit integer
    uint32_t address = 0;
    memcpy(&address, address_bytes, sizeof(address));
    
    return address; 
}


uint8_t BaseStation::calculate_radio_channel(uint8_t node_id) {

    return node_id * RF24_CHANNEL_SPACING;
}


bool BaseStation::init() {

    // start radio
    if (false == radio.begin()) {

        ERROR("Failed to start radio")
        return false;
    }

    // configure radio
    radio.enableDynamicPayloads();
    radio.setAutoAck(true);
    radio.setRetries(FAILED_SEND_DELAY, MAX_SEND_ATTEMPTS);
    radio.setAddressWidth(RF24_ADDRESS_WIDTH);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(this->radio_channel);
    radio.openReadingPipe(RF24_READING_PIPE, this->radio_address);

    // start listening on radio
    radio.startListening();

    // assuming status of all sensor nodes are vacant on initialization
    for (uint8_t i; i < sizeof(this->node_status); i++) {
        this->node_status[i] = true;
    }

    return true;
}


bool BaseStation::is_message() {

    return this->radio.available();
}


bool BaseStation::read_message(uint8_t** buffer, uint8_t len) {

    if (false == this->radio.available()) {
        return false;
    }

    this->radio.read(buffer, len);
    return true;
}


uint8_t BaseStation::get_id() {

    return this->node_id;
}


bool BaseStation::is_valid_sensor_node(uint8_t node_id) {

    return node_id <= SENSOR_NODE_NUM && node_id != this->node_id;
}


bool BaseStation::update_node_status(uint8_t node_id, bool is_vacant) {

    // provided node id is not valid
    if (false == this->is_valid_sensor_node(node_id)) {
        return false;
    }

    this->node_status[node_id - 1] = is_vacant;

    return true;
}

bool BaseStation::get_node_status(uint8_t node_id) {

    // provided node id is not valid
    if (false == this->is_valid_sensor_node(node_id)) {
        return false;
    }

    return this->node_status[node_id - 1];
}


uint8_t BaseStation::num_vacant() {

    uint8_t num_vacant = 0;

    // count number of vacant statuses
    for (uint8_t i = 0; i < sizeof(this->node_status); i++) {

        // status is vacant
        if (true == node_status[i]) {
            num_vacant++;
        }
    }

    return num_vacant;
}