#include "bullet.h"

ShipBullet *shipBullets[MAX_SHIP_BULLETS];
//AlienBullet *alienBullet;

xpm_image_t loadShipBulletXpm() {
    xpm_image_t shipBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &shipBullet_xpm);
    return shipBullet_xpm;
}

void createShipBullet(int x, int y, int speed, xpm_image_t img) {

    bool found = false;
    ShipBullet *shipBullet = (ShipBullet *) malloc(sizeof(ShipBullet));

    shipBullet->x = x;
    shipBullet->y = y;
    shipBullet->speed = speed;
    shipBullet->img = img;
    shipBullet->hitAlien = false;

    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] == NULL || shipBullets[i]->hitAlien) {
            //printf("created bullet->index = %d, x = %d, y = %d\n", i, shipBullet->x, shipBullet->y);
            shipBullets[i] = shipBullet;
            found = true;
            //shipBullets[i]->id = i;
        }
    }

    if (!found) {
        //destroyShipBullet(shipBullet);
        free(shipBullet);
        shipBullet = NULL;
    }
}

void updateShipBulletPosition() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] != NULL && !shipBullets[i]->hitAlien) {
            if (shipBullets[i]->y - shipBullets[i]->speed >= 0) {
                shipBullets[i]->y = shipBullets[i]->y - shipBullets[i]->speed;
            } else {
                removeShipBulletbyIndex(i);
            }
        }
    }
}

void drawShipBullets() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] != NULL && !shipBullets[i]->hitAlien) {
            if (drawShipBullet(shipBullets[i])) {
                return;
            }
        }
    }
}

int drawShipBullet(ShipBullet *shipBullet) {
    return drawXpm(shipBullet->x, shipBullet->y, shipBullet->img);
}

void removeShipBulletbyIndex(int ind) {
    if (shipBullets[ind] == NULL)
        return;
    free(shipBullets[ind]);
    shipBullets[ind] = NULL;
}

void initShipBullets() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        shipBullets[i] = NULL;
    }
}

void verifyAlienAndBulletCollision(Alien *a1, int *killCount) {
    for (int j = 0; j < MAX_SHIP_BULLETS; j++) {
        if (shipBullets[j] == NULL) { continue; }
        if (a1->x < shipBullets[j]->x && ((a1->x + a1->width) > shipBullets[j]->x && a1->alive) &&
            shipBullets[j]->y > a1->y && shipBullets[j]->y < (a1->y + a1->height) && !shipBullets[j]->hitAlien) {
            a1->alive = false;
            (*killCount)++;
            shipBullets[j]->hitAlien = true;
            removeShipBulletbyIndex(j);
            return;
        }
    }
}

///ALIENS
/*
xpm_image_t loadAlienBulletXpm() {
    xpm_image_t alienBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &alienBullet_xpm);
    return alienBullet_xpm;
}

void initAlienBullet(xpm_image_t img){
    alienBullet = (AlienBullet *) malloc(sizeof (AlienBullet));
    alienBullet->x = 0;
    alienBullet->y = 0;
    alienBullet->speed = 0;
    alienBullet->img = img;
    alienBullet->active = false;
}

void createAlienBullet(int x, int y, int speed, xpm_image_t img) {
    alienBullet->x = x;
    alienBullet->y = y;
    alienBullet->speed = speed;
    alienBullet->img = img;
    alienBullet->active = true;
}

void updateAlienBulletPosition() {
    if(alienBullet->y + alienBullet->speed + alienBullet->img.height >= Y_RES){
        alienBullet->active = false;
    }
    alienBullet->y = alienBullet->y + alienBullet->speed;
}

void drawAlienBullet(){
    drawXpm(alienBullet->x, alienBullet->y, alienBullet->img);
}

void clearAlienBullet() {
    if (alienBullet == NULL)
        return;
    free(alienBullet);
    alienBullet = NULL;
}

bool verifyShipAndBulletCollision(Ship *ship) {
    if (ship->x < alienBullet->x && ((ship->x + ship->img.width) > alienBullet->x) &&
        ship->y <= alienBullet->y && ship->y >= (alienBullet->y + alienBullet->img.height)) {
        ship->isShot = true;
        alienBullet->active = false;
        return true;
    }
    return false;
}

int generateALienBullet(Alien aliens[]){
    int randNum;
    srand(time(NULL));
    do {
        randNum = rand() % sizeOfAliens;
    }while(aliens[randNum].alive);
    return randNum;
}
*/
