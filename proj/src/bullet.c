#include "bullet.h"

xpm_image_t loadShipBulletXpm() {
    xpm_image_t shipBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &shipBullet_xpm);
    return shipBullet_xpm;
}

xpm_image_t loadAlienBulletXpm() {
    xpm_image_t alienBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &alienBullet_xpm);
    return alienBullet_xpm;
}

void createShipBullet(ShipBullet *shipBullets[], int x, int y, int speed, xpm_image_t img) {
    bool found = false;
    ShipBullet *shipBullet = (ShipBullet *) malloc(sizeof(ShipBullet));

    shipBullet->x = x;
    shipBullet->y = y;
    shipBullet->speed = speed;
    shipBullet->img = img;

    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] == NULL) {
            printf("created bullet->index = %d, x = %d, y = %d\n", i, shipBullet->x, shipBullet->y);
            shipBullets[i] = shipBullet;
            found = true;
        }
    }

    if(!found)
        destroyShipBullet(shipBullet);
}

AlienBullet *createAlienBullet(int x, int y, int speed, xpm_image_t img) {
    AlienBullet *alienBullet = (AlienBullet *) malloc(sizeof(AlienBullet));

    alienBullet->x = x;
    alienBullet->y = y;
    alienBullet->speed = speed;
    alienBullet->img = img;

    return alienBullet;
}

void updateShipBulletPosition(ShipBullet *shipBullet[]) {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullet[i] != NULL) {
            if(shipBullet[i]->y - shipBullet[i]->speed >= 0) {
                shipBullet[i]->y = shipBullet[i]->y - shipBullet[i]->speed;
                printf("[%d] new y = %d\n", i, shipBullet[i]->y);
            }

            else{
                printf("deleted bullet -> index = %d\n", i);
                //destroyShipBullet(shipBullet[i]);
                shipBullet[i] = NULL;
            }

        }
    }
}

void updateAlienBulletPosition(AlienBullet *alienBullet) {
    alienBullet->y = alienBullet->y - alienBullet->speed;
}

void initShipBullets(ShipBullet *shipBullets[]) {
    for (int i = 0; i <= MAX_SHIP_BULLETS; ++i) {
        shipBullets[i] = NULL;
    }
}

void shipShoot(ShipBullet *shipBullets[], ShipBullet *shipBullet) {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] == NULL) {
            shipBullets[i] = shipBullet;
            return;
        }
    }
    destroyShipBullet(shipBullet);
}

void drawShipBullets(ShipBullet *shipBullets[]) {
    //printf("draw bullet\n");
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] != NULL) {
            drawXpm(shipBullets[i]->x, shipBullets[i]->y, shipBullets[i]->img);
            printf("draw bullet: x = %d, y = %d\n", shipBullets[i]->x, shipBullets[i]->y);
        }
    }
}

void destroyShipBullet(ShipBullet *shipBullet) {
    if (shipBullet == NULL)
        return;
    free(shipBullet);
    shipBullet = NULL;
}

void destroyAlienBullet(AlienBullet *alienBullet) {

    if(alienBullet == NULL)
        return;
    free(alienBullet);

    alienBullet = NULL;
}
