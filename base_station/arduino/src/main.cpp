/**
* @brief: Contains the main run loop for the parking lot base station.
* @file: main.cpp
*
* @author: jkieltyka15
*/

// standard libraries
#include <Arduino.h>
#include <stdlib.h>
#include <Wire.h>

// local libraries
#include <Log.h>

// local dependencies
#include "basestation.hpp"


// unique ID for base station
#define BASE_STATION 0

// baud rate for serial connection
#define SERIAL_BAUD 9600

// delay in main loop in milliseconds
#define MAIN_LOOP_DELAY_MS 100

// size of message buffer
#define MSG_BUFFER_SIZE 32


// base station of WSN
BaseStation node = BaseStation(BASE_STATION);


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
 * Continuously listens for messages from sensor nodes and updates
 * the parking space status accordingly on the display.
 */
void loop() {

    if(true == node.is_message()) {

        uint8_t buffer[MSG_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        if (false == node.read_message((uint8_t**)(&buffer), (uint8_t)sizeof(buffer))) {
            ERROR("Failed to read message");
        }

        else {
            // convert buffer to Message
            Message msg = Message();
            memcpy(&msg, buffer, sizeof(msg));

            // verify message is for base station
            if (node.get_id() != msg.get_rx_id()) {
                WARN("Messaged intended for Node " + msg.get_rx_id() + " not Node " + node.get_id());
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
