#include "alien.h"

void change_alien_x_coordinates(Alien *a1, const int xf) {
    a1->x = a1->x + xf;
}

void change_alien_y_coordinates(Alien *a1, const int yf) {
    a1->y = a1->y + yf;
}

Alien createAlien(int xi, int yi, xpm_map_t img, xpm_map_t img_mov) {
    xpm_image_t alien_xpm_mov;
    xpm_load(img_mov, XPM_INDEXED, &alien_xpm_mov);
    xpm_image_t alien_xpm;
    xpm_load(img, XPM_INDEXED, &alien_xpm);
    return (Alien) {.x = xi, .y = yi, .img = alien_xpm, .img_mov = alien_xpm_mov, .width=60, .height=40};
}

void decrement_alien_x_coordinates(Alien *a1) {
    a1->x = a1->x - 1;
}

void decrement_alien_y_coordinates(Alien *a1) {
    a1->y = a1->y - 1;
}

void increment_alien_x_coordinates(Alien *a1) {
    a1->x = a1->x + 1;
}

int alien_get_x(Alien *a1) {
    return a1->x;
}

int alien_get_y(Alien *a1) {
    return a1->y;
}

int alien_get_width(Alien *a1) {
    return a1->width;
}

int alien_get_height(Alien *a1) {
    return a1->height;
}

void change_all_y(Alien aliens[], int yf, int size_array) {
    for (int i = 0; i < size_array; i++) {
        Alien *a1 = &aliens[i];
        change_alien_y_coordinates(a1, yf);
    }
}

/*
Alien * createAliens() {
    Alien aliens[ROW_ALIENS * COL_ALIENS];
    int xi = 24, yi = 20;
    for (int column = 0; column < COL_ALIENS; column++) {
        for (int row = 0; row < ROW_ALIENS; row++) {
            if (row == 0) {
                aliens[column + (ROW_ALIENS * row)] = createAlien(xi, yi, alien1, alien1_m);
            } else if (row == 1) {
                aliens[column + (ROW_ALIENS * row)] = createAlien(xi, yi, alien2, alien2_m);
            } else {
                aliens[column + (ROW_ALIENS * row)] = createAlien(xi, yi, alien3, alien3_m);//alien3
            }
        }
        xi = 24;
        yi += 50;
    }
    Alien aliens[] = {
            createAlien(24, 20, alien1, alien1_m),
            createAlien(300, 20, alien2, alien2_m),
            createAlien(600, 20, alien3, alien3_m)
    };
}
*/
