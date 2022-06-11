#ifndef PROJ_SHIP_H
#define PROJ_SHIP_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources.h"

#include "macros.h"
#include "graphic.h"

#include "resources/ship.xpm"
#include "resources/0.xpm"
#include "resources/1.xpm"
#include "resources/2.xpm"
#include "resources/3.xpm"
#include "resources/4.xpm"
#include "resources/5.xpm"
#include "resources/6.xpm"
#include "resources/7.xpm"
#include "resources/8.xpm"
#include "resources/9.xpm"

xpm_image_t zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm, five_xpm, six_xpm, seven_xpm,eight_xpm, nine_xpm; /*!< XPMs of the numbers */

/**
 * @brief Struct with the information of the player's ship
 */
typedef struct {
    int x, y; /*!< Coordinates of the ship*/
    int speed; /*!< Number of pixels the ship moves each interrupt of the keyboard/timer*/
    xpm_image_t img; /*!< xpm_image_t with the loaded xpm of the ship */
    int lives; /*!< Number of lives of the ship i.e. the number of times ship can still be hit by an alien bullet */
    int score; /*!< Current score of the ship */
} Ship;

/**
 * @brief Enumerated type for identifying the key being pressed/released
 * 
 */
typedef enum {
    A_Pressed, /*!< Key A is being pressed*/ 
    A_Released, /*!< Key A is being released*/ 
    D_Pressed, /*!< Key D is being pressed*/ 
    D_Released, /*!< Key A is being released*/ 
    Invalid_key /*!< Invalid key is being used*/ 
} KeyActivity;

/**
 * @brief Allocates memory and creates a Ship object
 * 
 * @param x X coordinate of the ship
 * @param y Y coordinate of the ship
 * @param speed Initial speed of the ship
 * @return Ship* 
 */
Ship *createShip(int x, int y, int speed);

void drawShip(Ship *ship);

Ship *updateShipPosition(Ship *ship, KeyActivity key);

void removeShip(Ship *ship);

void drawScore(Ship *ship);

void drawLives(Ship *ship);

void increaseScore(int scoreToAdd, Ship *ship);

void initNumbers();

#endif //PROJ_SHIP_H
