#include "bullet.h"

ShipBullet *shipBullets[MAX_SHIP_BULLETS];

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

void createShipBullet(int x, int y, int speed, xpm_image_t img) {

    bool found = false;
    ShipBullet *shipBullet = (ShipBullet *) malloc(sizeof(ShipBullet));

    shipBullet->x = x;
    shipBullet->y = y;
    shipBullet->speed = speed;
    shipBullet->img = img;

    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] == NULL) {
            //printf("created bullet->index = %d, x = %d, y = %d\n", i, shipBullet->x, shipBullet->y);
            shipBullets[i] = shipBullet;
            found = true;
            //shipBullets[i]->id = i;
        }
    }

    if(!found) {
        //destroyShipBullet(shipBullet);
        free(shipBullet);
        shipBullet = NULL;
    }
}

void updateShipBulletPosition() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] != NULL) {
            if(shipBullets[i]->y - shipBullets[i]->speed >= 0) {
                shipBullets[i]->y = shipBullets[i]->y - shipBullets[i]->speed;
                //printf("[%d] new y = %d\n", i, shipBullets[i]->y);
            }

            else{
                //printf("deleted bullet -> index = %d\n", i);
                removeShipBulletbyIndex(i);
            }

        }
    }
}

void drawShipBullets() {
    //printf("draw bullet\n");
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
//        printf("AAAAAAAAAAAAAAAAA\n");
        if (shipBullets[i] != NULL) {
            //printf("Nao null\n");
            if(drawShipBullet(shipBullets[i])) {
                //printf("nao desenhou\n");
                return;
            } else{
//                printf("BURRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRo\n");
            }
            //drawXpm(shipBullets[i]->x, shipBullets[i]->y, shipBullets[i]->img);
            //printf("draw bullet: x = %d, y = %d\n", shipBullets[i]->x, shipBullets[i]->y);
        } else {
//            printf("Can't draw bullet %d\n", i);
        }
    }
}

int drawShipBullet(ShipBullet* shipBullet){
    return drawXpm(shipBullet->x, shipBullet->y, shipBullet->img);
}

void removeShipBulletbyIndex(int ind){
    //free(shipBullets[ind]);
    shipBullets[ind] = NULL;
}

AlienBullet *createAlienBullet(int x, int y, int speed, xpm_image_t img) {
    AlienBullet *alienBullet = (AlienBullet *) malloc(sizeof(AlienBullet));

    alienBullet->x = x;
    alienBullet->y = y;
    alienBullet->speed = speed;
    alienBullet->img = img;

    return alienBullet;
}

void updateAlienBulletPosition(AlienBullet *alienBullet) {
    alienBullet->y = alienBullet->y - alienBullet->speed;
}

void initShipBullets() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        shipBullets[i] = NULL;
    }
}

/*
void shipShoot(ShipBullet *shipBullet) {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] == NULL) {
            shipBullets[i] = shipBullet;
            return;
        }
    }
    free(shipBullet);
    //shipBullet = NULL;
}
*/

/*
void destroyShipBullet(ShipBullet *shipBullet) {
    if (shipBullet == NULL)
        return;
    free(shipBullet);
    shipBullet = NULL;
}
*/

void destroyAlienBullet(AlienBullet *alienBullet) {

    if(alienBullet == NULL)
        return;
    free(alienBullet);

    alienBullet = NULL;
}


void verifyAlienAndBulletCollision(Alien *a1){
   
    for(int j = 0; j < MAX_SHIP_BULLETS; j++){

        if(shipBullets[j] == NULL){continue;}
        if(a1->x < shipBullets[j]->x && ((a1->x + a1->width) > shipBullets[j]->x && a1->alive) && shipBullets[j]->y > a1->y  && shipBullets[j]->y < (a1->y+a1->height)){
            a1->alive = false;
            removeShipBulletbyIndex(j);
            return;
        }
    } 
    
}


