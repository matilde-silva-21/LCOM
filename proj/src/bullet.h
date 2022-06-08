#ifndef PROJ_BULLET_H
#define PROJ_BULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
//#include <stdlib.h>

#include "graphic.h"
#include "resources.h"
#include "macros.h"
//#include "ship.h"

///SHIP BULLETS
typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    bool hitAlien;
} ShipBullet;

xpm_image_t loadShipBulletXpm();

//ShipBullet *createShipBullet(int x, int y, int speed, xpm_image_t img);
void createShipBullet(int x, int y, int speed, xpm_image_t img);

int drawShipBullet(ShipBullet* shipBullet);

void drawShipBullets();

void updateShipBulletPosition();

void initShipBullets();

void shipShoot(ShipBullet *shipBullet);

void removeShipBulletbyIndex(int ind);

void verifyAlienAndBulletCollision(Alien *a1, int *killCount);

///ALIEN BULLETS
/*
typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    bool active;
} AlienBullet;

xpm_image_t loadAlienBulletXpm();

void createAlienBullet(int x, int y, int speed, xpm_image_t img);

void initAlienBullet(xpm_image_t img);

void updateAlienBulletPosition(AlienBullet *alienBullet);

void clearAlienBullet();

bool verifyShipAndBulletCollision(Ship *ship);

int generateALienBullet(Alien aliens[]);

void drawAlienBullet();
*/
#endif //PROJ_BULLET_H
