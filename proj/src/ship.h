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

xpm_image_t zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm, five_xpm, six_xpm, seven_xpm, eight_xpm, nine_xpm;

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    int lives;
    bool isShot;
    int score;
} Ship;

typedef enum {
    A_Pressed,
    A_Released,
    D_Pressed,
    D_Released,
    Invalid_key
} KeyActivity;

Ship *createShip(int x, int y, int speed);

void drawShip(Ship *ship);

Ship *updateShipPosition(Ship *ship, KeyActivity key);

void removeShip(Ship *ship);

void drawScore(Ship *ship);

void drawLives(Ship *ship);

void increaseScore(int scoreToAdd, Ship *ship);

void initNumbers();


#endif //PROJ_SHIP_H
