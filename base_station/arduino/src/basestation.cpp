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


#define RF24_CHANNEL_SPACING 5  // number of channels between a valid node channel
#define RF24_READING_PIPE 0     // default reading pipe for the NRF24L01


#define MAX_SEND_ATTEMPTS 15            // maximum number of attempts to send a message
#define FAILED_SEND_DELAY_MIN_MS 0      // minimum delay between sending message attempts in milliseconds


BaseStation::BaseStation(uint8_t node_id) {

    this->node_id = node_id;

    this->radio_address = this->calculate_radio_address(node_id);
    this->radio_channel = this->calculate_radio_channel(node_id);
}


uint32_t BaseStation::calculate_radio_address(uint8_t node_id) {

    const byte address_bytes[RF24_ADDRESS_WIDTH] = { node_id, node_id, node_id, node_id };

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
    radio.setAutoAck(true);
    radio.enableAckPayload();
    radio.setRetries(FAILED_SEND_DELAY_MIN_MS, MAX_SEND_ATTEMPTS);
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