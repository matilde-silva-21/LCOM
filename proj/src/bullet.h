#ifndef PROJ_BULLET_H
#define PROJ_BULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

//#include "graphic.h"
#include "resources.h"

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    bool moving;
} ShipBullet;

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
} AlienBullet;

ShipBullet *createShipBullet(int x, int y, int speed);

AlienBullet *createAlienBullet(int x, int y, int speed);

#endif //PROJ_BULLET_H
