#ifndef PROJ_BULLET_H
#define PROJ_BULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "graphic.h"
#include "resources/ship_bullet.xpm"
#include "macros.h"
#include "ship.h"

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    bool hitAlien;
} ShipBullet;

xpm_image_t loadShipBulletXpm();

void createShipBullet(int x, int y, int speed, xpm_image_t img);

void initShipBullets();

void drawShipBullets();

void updateShipBulletPosition();

void removeShipBullet(ShipBullet *shipBullet);

void verifyAlienAndBulletCollision(Alien *a1, int *killCount, Ship *ship);

void removeAllShipBullets();

#endif //PROJ_BULLET_H
