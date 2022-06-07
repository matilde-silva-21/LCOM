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
    //bool moving;
    //int id;
} ShipBullet;

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
} AlienBullet;

xpm_image_t loadShipBulletXpm();

//ShipBullet *createShipBullet(int x, int y, int speed, xpm_image_t img);
void createShipBullet(int x, int y, int speed, xpm_image_t img);

int drawShipBullet(ShipBullet* shipBullet);

void drawShipBullets();

void updateShipBulletPosition();

void initShipBullets();

void shipShoot(ShipBullet *shipBullet);

void removeShipBulletbyIndex(int ind);

//void destroyShipBullet(ShipBullet* shipBullet);

xpm_image_t loadAlienBulletXpm();

AlienBullet *createAlienBullet(int x, int y, int speed, xpm_image_t img);

void updateAlienBulletPosition(AlienBullet *alienBullet);

void destroyAlienBullet(AlienBullet* alienBullet);

//void checkShipBulletCollision(Alien *aliens[]);

//void checkCollisions

void verifyAlienAndBulletCollision(Alien *a1);

#endif //PROJ_BULLET_H
