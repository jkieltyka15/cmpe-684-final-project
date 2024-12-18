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
#include "parkingdisplay.hpp"


// unique ID for base station
#define BASE_STATION 0

// baud rate for serial connection
#define SERIAL_BAUD 9600

// delay in main loop in milliseconds
#define MAIN_LOOP_DELAY_MS 100

// size of message buffer
#define MSG_BUFFER_SIZE 32


// base station of WSN
BaseStation base_station = BaseStation(BASE_STATION);


/**
 * @brief Initialize all necessary objects and variables.
 */
void setup() {

    Serial.begin(SERIAL_BAUD);
  
    // initialize the base station
    if (false == base_station.init()) {

        ERROR("Failed to initialize base station. Check hardware")

        // hang since failure is not recoverable
        while(1);
    }

    // intialize the screen
    if (false == init_parking_display()) {

        ERROR("Failed to initialize screen. Check hardware")

        // hang since failure is not recoverable
        while(1);
    }

    // update screen to show the parking map
    draw_parking_map();

    INFO("setup complete")
}


/**
 * @brief Main program run loop.
 * 
 * Continuously listens for messages from sensor nodes and updates
 * the parking space status accordingly on a display.
 */
void loop() {

    if(true == base_station.is_message()) {

        uint8_t buffer[MSG_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        if (false == base_station.read_message((uint8_t**)(&buffer), (uint8_t)sizeof(buffer))) {
            ERROR("Failed to read message");
        }

        else {
            // convert buffer to Message
            Message msg = Message();
            memcpy(&msg, buffer, sizeof(msg));

            // verify message is for base station
            if (base_station.get_id() != msg.get_rx_id()) {
                WARN("Messaged intended for Node " + msg.get_rx_id() + " not Node " + base_station.get_id());
            }

            // verify sender has a valid ID
            else if(false == base_station.is_valid_sensor_node(msg.get_tx_id())) {
                WARN("Message was from invalid Node " + msg.get_tx_id());
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

                        uint8_t node_id = update_msg.get_node_id();
                        bool is_vacant = update_msg.get_is_vacant();

                        // verify node to update has a valid ID
                        if(false == base_station.is_valid_sensor_node(node_id)) {
                            WARN("Cannot update status of invalid Node " + node_id);
                        }

                        // only update if vacancy status changed
                        else if (is_vacant != base_station.get_node_status(node_id)) {

                            // update the status of the reporting node
                            (void) base_station.update_node_status(node_id, is_vacant);
                            
                            // node status is vacant
                            if (true == is_vacant) {
                                INFO("Node " + node_id + " is now vacant")
                            }

                            // node status is occupied
                            else {
                                INFO("Node " + node_id + " is now occupied")
                            }

                            // update the status of the parking space
                            update_parking_space(node_id, is_vacant);
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
