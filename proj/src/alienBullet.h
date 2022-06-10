#ifndef PROJ_ALIENBULLET_H
#define PROJ_ALIENBULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "graphic.h"
#include "resources.h"
#include "macros.h"
#include "ship.h"

typedef struct {
    int x, y;
    int speed;
    xpm_image_t img;
    bool active;
} AlienBullet;

xpm_image_t loadAlienBulletXpm();

void initAlienBullet();

void createAlienBullet(int x, int y, int speed, xpm_image_t img);

void updateAlienBulletPosition();

void drawAlienBullet();

int generateAlienBullet(Alien aliens[]);

bool verifyShipAndBulletCollision(Ship *ship);

void removeAlienBullet();

#endif //PROJ_ALIENBULLET_H
