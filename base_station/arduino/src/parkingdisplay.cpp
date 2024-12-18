/**
* @brief: Contains the prototypes for the GUI
* @file: parkingdisplay.hpp
*
* @author: jkieltyka15
*/

// standard libraries
#include <Arduino.h>
#include <TVout.h>


#define SCREEN_REGION NTSC // region of the display
#define SCREEN_W      64   // width in pixels of the display
#define SCREEN_H      48   // height in pixels of the display

// thickness of all lines drawn
#define LINE_PIXEL_THICKNESS 2

#define NUM_OF_CARS 10  // max number of cars that can be parked
#define CAR_PIXEL_W 6   // width of car icon in pixels
#define CAR_PIXEL_H 5   // height of car icon in pixels


// 2D coordinate
struct position_t {
    uint8_t x;
    uint8_t y;
};

position_t space_locations[NUM_OF_CARS] = {
    {4, 15},    // parking space 1
    {4, 4},     // parking space 2
    {21, 17},   // parking space 3
    {21, 28},   // parking space 4
    {21, 39},   // parking space 5
    {33, 17},   // parking space 6
    {33, 28},   // parking space 7
    {33, 39},   // parking space 8
    {50, 4},    // parking space 9
    {50, 15}    // parking space 10
};


// screen for displaying parking space status
TVout screen = TVout();


/**
 * @brief Draws a black or white rectangle line by line.
 * 
 * @param color: color of rectangle which must be either BLACK or WHITE
 * @param x: horizontal position of upper left corner of rectangle on screen
 * @param y: vertical position of upper left corner of rectangle on screen
 * @param width: width of rectangle in pixels
 * @param height: height of rectangle in pixels
 * 
 * @note value of color is not checked, nor is a valid screen position nor rectangle
 *      being drawn off the screen
 */
static void draw_rectangle(uint8_t color, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {

    // draw rectangle line by line
    for (uint8_t dy = 0; dy < height; dy++) {
        for (uint8_t dx = 0; dx < width; dx++) {
            screen.set_pixel(x + dx, y + dy, color);
        }
    }
}


/**
 * @brief Draws a black or white rectangle line by line.
 * 
 * @param color: color of rectangle which must be either BLACK or WHITE
 * @param position: position of upper left corner of rectangle on screen
 * @param width: width of rectangle in pixels
 * @param height: height of rectangle in pixels
 * 
 * @note value of color is not checked, nor is a valid screen position nor rectangle
 *      being drawn off the screen
 */
static void draw_rectangle(uint8_t color, position_t position, uint8_t width, uint8_t height) {

    draw_rectangle(color, position.x, position.y, width, height);
}


bool init_parking_display() {

    // intialize the screen
    if (0 != screen.begin(SCREEN_REGION, SCREEN_W, SCREEN_H)) {
        return false;
    }

    // clear the screen
    screen.clear_screen();

    return true;
}


void draw_parking_map() {

    // draw boarders
    draw_rectangle(WHITE, 0, 0, LINE_PIXEL_THICKNESS, 24);  // left boarder
    draw_rectangle(WHITE, 2, 0, 56, LINE_PIXEL_THICKNESS);  // top boarder
    draw_rectangle(WHITE, 58, 0, LINE_PIXEL_THICKNESS, 24); // right boarder
    draw_rectangle(WHITE, 0, 46, 60, LINE_PIXEL_THICKNESS); // bottom boarder

    // draw parking space seperators
    draw_rectangle(WHITE, 2, 11, 8, LINE_PIXEL_THICKNESS);      // top left
    draw_rectangle(WHITE, 2, 22, 8, LINE_PIXEL_THICKNESS);      // bottom left
    draw_rectangle(WHITE, 50, 11, 8, LINE_PIXEL_THICKNESS);     // top right
    draw_rectangle(WHITE, 50, 22, 8, LINE_PIXEL_THICKNESS);     // bottom left
    draw_rectangle(WHITE, 21, 13, 18, LINE_PIXEL_THICKNESS);    // top middle
    draw_rectangle(WHITE, 21, 24, 18, LINE_PIXEL_THICKNESS);    // middle middle
    draw_rectangle(WHITE, 21, 35, 18, LINE_PIXEL_THICKNESS);    // bottom middle
    draw_rectangle(WHITE, 29, 15, LINE_PIXEL_THICKNESS, 31);    // middle vertical divider
}


void update_parking_space(uint8_t space_id, bool is_vacant) {

    // check to ensure space ID is valid
    if ((0 == space_id) || ((space_id - 1) > NUM_OF_CARS)) {
        return;
    }

    // determine if car should be drawn or erased
    uint8_t color = (true == is_vacant) ? BLACK : WHITE;

    // draw or erase car
    draw_rectangle(color, space_locations[space_id - 1], CAR_PIXEL_W, CAR_PIXEL_H);
}
