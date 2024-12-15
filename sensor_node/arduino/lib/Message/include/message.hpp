/**
* @brief: Contains the prototype of the Message class.
* @file: message.hpp
*
* @author: jkieltyka15
*/

#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

// standard libraries
#include <Arduino.h>


// message types
enum message_t {
    UNKNOWN = 0,
    UPDATE = 1
};


class Message {

    private:

        uint8_t rx_id = 0;              // receiving node's ID
        uint8_t tx_id = 0;              // transmitting node's ID
        message_t msg_type = UNKNOWN;   // message type
        

    public:

        /**
         * @brief Constructs a Message object
         * 
         * @param rx_id: ID of receiving node
         * @param tx_id: ID of transmitting node
         * @param msg_type: type of message
         */
        Message(uint8_t rx_id, uint8_t tx_id, message_t msg_type);

        /**
         * @brief Gets the receiving node's ID
         * 
         * @return Receiving node's ID
         */
        uint8_t get_rx_id();

        /**
         * @brief Gets the transmitting node's ID
         * 
         * @return Transmitting node's ID
         */
        uint8_t get_tx_id();

        /**
         * @brief Gets the type of message
         * 
         * @return Type of message
         */
        message_t get_type();
};

#endif // _MESSAGE_HPP_
