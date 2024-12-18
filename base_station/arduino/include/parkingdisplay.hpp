/**
* @brief: Contains the prototypes for the GUI
* @file: parkingdisplay.hpp
*
* @author: jkieltyka15
*/

#ifndef _PARKING_DISPLAY_HPP_
#define _PARKING_DISPLAY_HPP_

// standard libraries
#include <Arduino.h>

/**
 * @brief Initializes all objects and variables for the parking display
 * 
 * @return True on success. Otherwise false
 */
bool init_parking_display();

/**
 * @brief Draws the parking map on the parking display
 */
void draw_parking_map();

/**
 * @brief Draws or erases a car in a particular parking space
 * 
 * @param space_id: ID of parking space to update
 * @param is_vacant: vacancy status of parking space
 */
void update_parking_space(uint8_t space_id, bool is_vacant);

#endif // _PARKING_DISPLAY_HPP_
