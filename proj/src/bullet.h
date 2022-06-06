#ifndef PROJ_BULLET_H
#define PROJ_BULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources.h"
#include "macros.h"

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    bool moving;
    int id;
} ShipBullet;

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
} AlienBullet;

xpm_image_t loadShipBulletXpm();

xpm_image_t loadAlienBulletXpm();

//ShipBullet *createShipBullet(int x, int y, int speed, xpm_image_t img);
void createShipBullet(ShipBullet *shipBullets[], int x, int y, int speed, xpm_image_t img);

AlienBullet *createAlienBullet(int x, int y, int speed, xpm_image_t img);

void updateAlienBulletPosition(AlienBullet *alienBullet);

void updateShipBulletPosition(ShipBullet *shipBullet[]);

void initShipBullets(ShipBullet *shipBullets[]);

void shipShoot(ShipBullet *shipBullets[], ShipBullet *shipBullet);

void drawShipBullets(ShipBullet *shipBullets[]);

void destroyShipBullet(ShipBullet* shipBullet);

void destroyAlienBullet(AlienBullet* alienBullet);

//void checkCollisions

#endif //PROJ_BULLET_H
