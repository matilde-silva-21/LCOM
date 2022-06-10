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
#endif //PROJ_SHIP_H
