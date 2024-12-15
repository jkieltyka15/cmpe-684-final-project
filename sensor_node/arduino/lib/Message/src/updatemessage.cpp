/**
* @brief: Contains the implementation of the UpdateMessage class.
* @file: updatemessage.cpp
*
* @author: jkieltyka15
*/

// standard libraries
#include <Arduino.h>

// local dependencies
#include "message.hpp"
#include "updatemessage.hpp"


UpdateMessage::UpdateMessage(uint8_t rx_id,
                             uint8_t tx_id,
                             uint8_t node_id,
                             bool is_vacant) : Message(rx_id, tx_id, UPDATE) {

    this->node_id = node_id;
    this->is_vacant = is_vacant;
}


uint8_t UpdateMessage::get_node_id() {

    return this->node_id;
}


bool UpdateMessage::get_is_vacant() {

    return this->is_vacant;
}
