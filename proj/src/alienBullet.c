#include "alienBullet.h"

AlienBullet *alienBullet;

xpm_image_t loadAlienBulletXpm() {
    xpm_image_t alienBullet_xpm;
    xpm_load(bullet, XPM_INDEXED, &alienBullet_xpm);
    return alienBullet_xpm;
}

void initAlienBullet() {
    alienBullet = (AlienBullet *) malloc(sizeof(AlienBullet));
}

void createAlienBullet(int x, int y, int speed, xpm_image_t img) {
    alienBullet->x = x;
    alienBullet->y = y;
    alienBullet->speed = speed;
    alienBullet->img = img;
    alienBullet->active = true;
}

void updateAlienBulletPosition() {
    if (alienBullet->y + alienBullet->speed + alienBullet->img.height >= Y_RES) {
        alienBullet->active = false;
        return;
    }
    alienBullet->y = alienBullet->y + alienBullet->speed;
}

void drawAlienBullet() {
    if (!alienBullet->active) return;
    drawXpm(alienBullet->x, alienBullet->y, alienBullet->img);
}

int generateAlienBullet(Alien aliens[]) {
    int randNum;
    srand(time(NULL));
    do {
        randNum = rand() % sizeOfAliens;
    } while (!aliens[randNum].alive);
    return randNum;
}

bool verifyShipAndBulletCollision(Ship *ship) {
    if (ship->x < alienBullet->x && ((ship->x + ship->img.width) > alienBullet->x) &&
        ship->y <= alienBullet->y && ship->y >= (alienBullet->y + alienBullet->img.height)) {
        ship->isShot = true;
        ship->lives --;
        alienBullet->active = false;
        printf("colisao\n");
        return true;
    }
    return false;
}

void removeAlienBullet() {
    if (alienBullet == NULL)
        return;
    free(alienBullet);
    alienBullet = NULL;
}


