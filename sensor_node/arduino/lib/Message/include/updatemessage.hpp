/**
* @brief: Contains the prototype of the UpdateMessage class.
* @file: updatemessage.hpp
*
* @author: jkieltyka15
*/

#ifndef _UPDATE_MESSAGE_HPP_
#define _UPDATE_MESSAGE_HPP_

// standard libraries
#include <Arduino.h>

// local dependencies
#include "message.hpp"


class UpdateMessage : public Message {

    private:

        uint8_t node_id = 0;
        bool is_vacant = true;


    public:

        /**
         * @brief Constructs an UpdateMessage object
         * 
         * @param rx_id: ID of receiving node
         * @param tx_id: ID of transmitting node
         * @param node_id: ID of node reporting its status
         * @param is_vacant: Node's vacancy status
         */
        UpdateMessage(uint8_t rx_id, uint8_t tx_id, uint8_t node_id, bool is_vacant);

        /**
         * @brief Gets the ID of the node who is reporting its status
         * 
         * @return ID of the node
         */
        uint8_t get_node_id();

        /**
         * @brief Gets the vacancy status of the node
         * 
         * @return True if the status is vacant. Otherwise false.
         */
        bool get_is_vacant();
};


#endif // _UPDATE_MESSAGE_HPP_
