#include "ship.h"

Ship *createShip(int x, int y, int speed) {
    Ship *ship = (Ship *) malloc(sizeof(Ship));

    ship->x = x;
    ship->y = y;
    ship->lives = NUMLIVES;
    //ship->isShot = false;
    ship->speed = speed;

    xpm_image_t ship_xpm;
    xpm_load(ship_image, XPM_INDEXED, &ship_xpm);

    ship->img = ship_xpm;

    return ship;
}

void drawShip(Ship *ship) {
    drawXpm(ship->x, ship->y, ship->img);
}

static int clamp(int min, int max, int value) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Ship *updateShipPosition(Ship *ship, KeyActivity key) {
    int xf;
    switch (key) {
        case A_Pressed:
            xf = ship->x - ship->speed;
            break;
        case A_Released:
            return ship;
        case D_Pressed:
            xf = ship->x + ship->speed;
            break;
        case D_Released:
            return ship;
        case Invalid_key:
            return ship;
        default:
            break;
    }

    xf = clamp(X_MIN, X_MAX - ship->img.width, xf);
    ship->x = xf;

    return ship;
}

void destroyShip(Ship *ship) {
    if (ship == NULL)
        return;
    free(ship);
    ship = NULL;
}

void drawScore(Ship *ship){

}

void drawLives(Ship *ship){
    xpm_image_t live;
    switch(ship->lives){
        case 1:
            xpm_load(one, XPM_INDEXED, &live);
            drawXpm(1029, 741, live);
            break;
        case 2:
            xpm_load(two, XPM_INDEXED, &live);
            drawXpm(1029, 741, live);
            break;
        case 3:
            xpm_load(three, XPM_INDEXED, &live);
            drawXpm(1029, 741, live);
            break;
    }
}