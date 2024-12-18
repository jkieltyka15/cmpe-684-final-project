/**
* @brief: Contains the main run loop for the parking sensor node.
* @file: main.cpp
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

// local dependencies
#include "sensornode.hpp"
#include "parkingmap.hpp"


// unique ID for node
#define NODE_ID 10

// baud rate for serial connection
#define SERIAL_BAUD 9600


#define MAIN_LOOP_DELAY_MIN_MS 75   // minimum delay in main loop in milliseconds
#define MAIN_LOOP_DELAY_MAX_MS 150  // maximum delay in main loop in milliseconds

// number of loop iterations without transmitting a message
// before sending out a heartbeat
#define LOOPS_BEFORE_HEARTBEAT 25

// size of message buffer
#define MSG_BUFFER_SIZE 32


// parking sensor node
SensorNode node = SensorNode(NODE_ID);

// counter for tracking loop iterations since the last
// message was transmitted
uint8_t loops_since_last_transmission = 0;

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

    // track iterations since last transmission
    loops_since_last_transmission++;

    // determine if parking space status has changed or time for heartbeat
    if ((true == node.is_sensor_status_changed())
        || LOOPS_BEFORE_HEARTBEAT <= loops_since_last_transmission) {

        // determine recepient
        int16_t rx_id = get_next_ingress_node(node.get_id());

        // no recepient available
        if (0 > rx_id) {
            WARN("Nobody to send update to")
        }

        else {
            // transmit update
            if (false == node.transmit_update((uint8_t)rx_id)) {
                ERROR("Failed to transmit update message to Node " + rx_id)
            }

            // heartbeat message successfully sent
            else if (LOOPS_BEFORE_HEARTBEAT <= loops_since_last_transmission) {
                INFO("heartbeat update message sent to Node " + + rx_id)
            }

            // update message successfully sent
            else {
                INFO("update message sent to Node " + + rx_id)
            }

            // reset heartbeat iteration counter
            loops_since_last_transmission = 0;
        }
    }

    // check if a message has been received
    else if(true == node.is_message()) {

        uint8_t buffer[MSG_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        if (false == node.read_message((uint8_t**)(&buffer), (uint8_t)sizeof(buffer))) {
            ERROR("Failed to read message");
        }

        else {
            // convert buffer to Message
            Message msg = Message();
            memcpy(&msg, buffer, sizeof(msg));

            // verify message is for node
            if (node.get_id() != msg.get_rx_id()) {
                WARN("Message intended for Node " + msg.get_rx_id() + " not Node " + node.get_id());
            }

            // react accordingly based on message type
            else {

                uint8_t type = msg.get_type();
                switch(type) {

                    case MESSAGE_UPDATE: {

                        INFO("Received UPDATE message from Node " + msg.get_tx_id())
                        
                        // convert buffer to UpdateMessage
                        UpdateMessage update_msg = UpdateMessage();
                        memcpy(&update_msg, buffer, sizeof(update_msg));

                        // determine recepient
                        int16_t rx_id = get_next_ingress_node(node.get_id());

                        // no recepient available
                        if (0 > rx_id) {
                            WARN("Nobody to send update to")
                        }

                        else {
                            // create new message to forward
                            uint8_t tx_id = node.get_id();
                            uint8_t node_id = update_msg.get_node_id();
                            bool is_vacant = update_msg.get_is_vacant();
                            UpdateMessage new_msg = UpdateMessage(rx_id, tx_id, node_id, is_vacant);

                            // forward the message
                            if (false == node.transmit_update(&new_msg)) {
                                ERROR("Failed to transmit update message to " + rx_id)
                            }

                            // reset heartbeat iteration counter
                            loops_since_last_transmission = 0;
                        }

                        break;
                    }

                    default:
                        WARN("Unknown message type received")
                        break;
                }
            }
        }
    }

    // nothing to do
    else {
        delay(random(MAIN_LOOP_DELAY_MIN_MS, MAIN_LOOP_DELAY_MAX_MS));
    }
}
