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


// unique ID for node
#define NODE_ID 1

// baud rate for serial connection
#define SERIAL_BAUD 9600

// delay in main loop in milliseconds
#define MAIN_LOOP_DELAY_MS 100

// size of message buffer
#define MSG_BUFFER_SIZE 32


// parking sensor node
SensorNode node = SensorNode(NODE_ID);


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

    // determine if parking space status has changed
    if (true == node.is_sensor_status_changed()) {

        // determine recepient
        uint8_t rx_id = node.get_id() - 1;

        // transmit update
        if (false == node.transmit_update(rx_id)) {

            ERROR("Failed to transmit update message")
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

            INFO("" + (int)buffer[0] + " " + (int)buffer[1] +  " " + (int)buffer[2] + " " + (int)buffer[3] +  " " + (int)buffer[4])

            // convert buffer to Message
            Message msg = Message();
            memcpy(&msg, buffer, sizeof(msg));

            // verify message is for node
            if (node.get_id() != msg.get_rx_id()) {
                WARN("Messaged intended for Node " + msg.get_rx_id() + " not Node " + node.get_id());
            }

            // react accordingly based on message type
            else {

                INFO("Received UPDATE message from Node " + msg.get_tx_id())

                uint8_t type = msg.get_type();
                switch(type) {

                    case MESSAGE_UPDATE: {

                        INFO("Received UPDATE message from Node " + msg.get_tx_id())
                        
                        // convert buffer to UpdateMessage
                        UpdateMessage update_msg = UpdateMessage();
                        memcpy(&update_msg, buffer, sizeof(update_msg));

                        // determine recepient
                        uint8_t rx_id = node.get_id() - 1;

                        // create new message to forward
                        uint8_t tx_id = node.get_id();
                        uint8_t node_id = update_msg.get_node_id();
                        bool is_vacant = update_msg.get_is_vacant();
                        UpdateMessage new_msg = UpdateMessage(rx_id, tx_id, node_id, is_vacant);

                        // forward the message
                        if (false == node.transmit_update(&new_msg)) {
                            ERROR("Failed to transmit update message")
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
        delay(MAIN_LOOP_DELAY_MS);
    }
}
