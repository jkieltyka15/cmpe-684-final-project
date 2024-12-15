/**
* @brief: Contains the prototype functions for the parking map.
* @file: parkingmap.hpp
*
* @author: jkieltyka15
*/

#ifndef _PARKING_MAP_H_
#define _PARKING_MAP_H_

// standard libraries
#include <Arduino.h>

/**
 * @brief Gets the next node ID for forwarding an ingress message
 * 
 * @param node_id: ID of current node
 * @return Next node ID on success. Otherwise -1
 */
int16_t get_next_ingress_node(uint8_t node_id);

#endif // _PARKING_MAP_H_
