/**
* @brief: Contains the implementation of the SensorNode class.
* @file: sensornode.cpp
*
* @author: jkieltyka15
*/

// standard libraries
#include <Arduino.h>
#include <stdlib.h>
#include <Wire.h>
#include <Adafruit_VL6180X.h>

// local libraries
#include <Log.h>
#include <Message.h>

// local dependencies
#include "sensornode.hpp"



#define RF24_CHANNEL_SPACING 5  // number of channels between a valid node channel
#define RF24_READING_PIPE 0     // default reading pipe for the NRF24L01


#define MAX_SEND_ATTEMPTS 15            // maximum number of attempts to send a message
#define FAILED_SEND_DELAY_MIN_MS 0      // minimum delay between sending message attempts in milliseconds


SensorNode::SensorNode(uint8_t node_id) {

    this->node_id = node_id;

    this->radio_address = this->calculate_radio_address(node_id);
    this->radio_channel = this->calculate_radio_channel(node_id);
}


uint32_t SensorNode::calculate_radio_address(uint8_t node_id) {

    const byte address_bytes[RF24_ADDRESS_WIDTH] = { node_id, node_id, node_id, node_id };

    uint32_t address = 0;
    memcpy(&address, address_bytes, sizeof(address));
    
    return address; 
}


uint8_t SensorNode::calculate_radio_channel(uint8_t node_id) {

    return node_id * RF24_CHANNEL_SPACING;
}


bool SensorNode::init() {

    // start ToF sensor
    if (false == sensor.begin()) {

        ERROR("Failed to start ToF sensor")
        return false;
    }

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


bool SensorNode::transmit_update(UpdateMessage* msg) {

    // calculate receiver node's radio configuration
    uint8_t rx_id = msg->get_rx_id();
    uint32_t rx_address = this->calculate_radio_address(rx_id);
    uint8_t rx_channel = this->calculate_radio_channel(rx_id);

    // stop listening
    this->radio.stopListening();
    this->radio.closeReadingPipe(RF24_READING_PIPE);

    // configure radio to send to receiver node
    this->radio.setChannel(rx_channel);
    radio.openWritingPipe(rx_address);

    // create a copy of the message to send
    uint8_t buffer[sizeof(*msg)];
    memcpy(buffer, msg, sizeof(buffer));

    // attempt to transmit message
    bool is_sent = this->radio.write(&buffer, sizeof(buffer));

    // switch back to this node's radio configuration
    this->radio.setChannel(this->radio_channel);
    radio.openReadingPipe(RF24_READING_PIPE, this->radio_address);

    // start listening again
    this->radio.startListening();

    return is_sent;
}


bool SensorNode::transmit_update(uint8_t rx_node_id) {

    // create update message
    bool is_vacant = (this->sensor_status == VACANT);
    UpdateMessage msg = UpdateMessage(rx_node_id, this->node_id, this->node_id, is_vacant);

    // attempt to transmit message
    return this->transmit_update(&msg);
}


bool SensorNode::is_message() {

    return this->radio.available();
}


bool SensorNode::read_message(uint8_t** buffer, uint8_t len) {

    if (false == this->radio.available()) {
        return false;
    }

    this->radio.read(buffer, len);
    return true;
}


uint8_t SensorNode::get_id() {

    return this->node_id;
}
