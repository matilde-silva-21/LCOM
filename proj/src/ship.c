#include "ship.h"

Ship *createShip(int x, int y, int speed) {
    Ship *ship = (Ship *) malloc(sizeof(Ship));

    ship->x = x;
    ship->y = y;
    ship->lives = NUMLIVES;
    ship->isShot = false;
    ship->speed = speed;
    ship->score = 0; 

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

void removeShip(Ship *ship) {
    free(ship);
    ship = NULL;
}

void drawScore(Ship *ship){
    xpm_image_t temp;
    if(ship->score == 0){
        xpm_load(zero, XPM_INDEXED, &temp);
        drawXpm(120, 735, temp);
        return; 
    }
    
    int count = 0; 
    int div = ship->score;
    while(div>0){
        div /= 10; 
        count++;
    }
    
    xpm_image_t* score; 
    score = (xpm_image_t*)malloc(sizeof(xpm_image_t)*count);
    int x = 120;
    int res;
    div = ship->score;
    while(div>0){
        res = div%10; 
        div /= 10; 
        switch(res){
            case 0:
                xpm_load(zero, XPM_INDEXED, &temp);
                break;
            case 1:
                xpm_load(one, XPM_INDEXED, &temp);
                break;
            case 2:
                xpm_load(two, XPM_INDEXED, &temp);
                break;
            case 3:
                xpm_load(three, XPM_INDEXED, &temp);
                break;
            case 4:
                xpm_load(four, XPM_INDEXED, &temp);
                break;
            case 5:
                xpm_load(five, XPM_INDEXED, &temp);
                break;
            case 6:
                xpm_load(six, XPM_INDEXED, &temp);
                break;
            case 7:
                xpm_load(seven, XPM_INDEXED, &temp);
                break;
            case 8:
                xpm_load(eight, XPM_INDEXED, &temp);
                break;
            case 9: 
                xpm_load(nine, XPM_INDEXED, &temp);
                break;
        }
        score[count-1] = temp;
        count--;
    }
    
    for(unsigned int i = 0; i < sizeof(score)-1; i++){
        drawXpm(x, 735, score[i]);
        x+=24;
    }
    
    free(score);
}

void drawLives(Ship *ship){
    xpm_image_t live;
    switch(ship->lives){
        case 1:
            xpm_load(one, XPM_INDEXED, &live);
            drawXpm(1000, 735, live);
            break;
        case 2:
            xpm_load(two, XPM_INDEXED, &live);
            drawXpm(1000, 735, live);
            break;
        case 3:
            xpm_load(three, XPM_INDEXED, &live);
            drawXpm(1000, 735, live);
            break;
    }
}

void increaseScore(int scoreToAdd, Ship* ship){
    ship->score += scoreToAdd;
}
