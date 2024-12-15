/**
* @brief: Contains the prototype functions for the parking map.
* @file: parkingmap.hpp
*
* @author: jkieltyka15
*/

// standard libraries
#include <Arduino.h>

// local libraries
#include <Log.h>

// local dependencies
#include "parkingmap.hpp"


#define NUM_ROWS 4  // number of rows in the parking map
#define NUM_COLS 4  // number of columns in the parking map

#define NOT_SPOT -1  // represent space not for 

#define BASE_STATION_ID 0   // ID of base station
#define BASE_STATION_ROW 0  // row base station is located on
#define BASE_STATION_COL 1  // column base station is located on


static int16_t parking_map[NUM_ROWS][NUM_COLS] = {
    {NOT_SPOT, 0, 1, 2},
    {5, 4, 3, NOT_SPOT},
    {8, 7, 6 , NOT_SPOT},
    {NOT_SPOT, NOT_SPOT, 10, 9}
};


int16_t get_next_ingress_node(uint8_t node_id) {

    // zero is not a valid sensor node ID
    if (BASE_STATION_ID == node_id) {
        return NOT_SPOT;
    }

    // find coordinates of current node
    for (uint8_t i = 0; i < NUM_ROWS; i++) {
        for (uint8_t j = 0; j < NUM_COLS; j++) {

            // current coordinate does not have a node
            if (NOT_SPOT == parking_map[i][j]) {
                continue;
            }

            // current node found
            else if (node_id == parking_map[i][j]) {

                // nodes in same row as base station always go down a column
                if (BASE_STATION_ROW == i) {
                    return parking_map[i][j - 1];
                }

                // nodes in same column as base station always go down a row
                if (BASE_STATION_COL == j) {
                    return parking_map[i - 1][j];
                }

                // calculate column direction
                int8_t direction = (BASE_STATION_COL < j) ? -1 : 1;

                // determine if next node is neighboring column
                if (0 == (random() % 2)) {

                    // no neighboring column node so next node is down a row
                    if (NOT_SPOT != parking_map[i][j + direction]) {
                        return parking_map[i][j + direction];
                    }
                }

                // next node is down a row
                if (NOT_SPOT != parking_map[i - 1][j]) {
                    return parking_map[i - 1][j];
                }

                // no neighboring row node so next node is over a column
                if (NOT_SPOT != parking_map[i][j + direction]) {
                    return parking_map[i][j + direction];
                }

                // no next node
                return NOT_SPOT;
            }
        }
    }

    // invalide node ID
    return NOT_SPOT;
}
