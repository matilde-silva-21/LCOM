#include "alien.h"

/*
Alien *aliens[sizeOfAliens];

void change_alien_x_coordinates(Alien *a1, const int xf) {
    a1->x = a1->x + xf;
}

void change_alien_y_coordinates(Alien *a1, const int yf) {
    a1->y = a1->y + yf;
}

Alien *createAlien(int xi, int yi, xpm_map_t img, xpm_map_t img_mov) {
    xpm_image_t alien_xpm_mov;
    xpm_load(img_mov, XPM_INDEXED, &alien_xpm_mov);
    xpm_image_t alien_xpm;
    xpm_load(img, XPM_INDEXED, &alien_xpm);

    Alien *alien = (Alien* )malloc(sizeof(Alien));

    alien->x = xi; alien->y = yi;
    alien->img = alien_xpm; alien->img_mov = alien_xpm_mov;
    alien->width = 60; alien->height = 40;

    //return (Alien) {.x = xi, .y = yi, .img = alien_xpm, .img_mov = alien_xpm_mov, .width=60, .height=40};
    return alien;
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

void change_all_y(int yf, int size_array) {
    for (int i = 0; i < size_array; i++) {
        Alien *a1 = aliens[i];
        change_alien_y_coordinates(a1, yf);
    }
}
 */

/*
void createAliens() {
    int xi = 24, yi = 20;

    int indice = 0;

    for (int column = 0; column < COL_ALIENS; column++) {
        for (int row = 0; row < ROW_ALIENS; row++) {
            if (row == 0) {
                printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n", row, column, indice, xi, yi);
                aliens[indice] = createAlien(xi, yi, alien3, alien3_m);
            } else if (row == 1) {
                printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n", row, column, indice, xi, yi);
                aliens[indice] = createAlien(xi, yi, alien1, alien1_m);
            } else {
                printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n", row, column, indice, xi, yi);
                aliens[indice] = createAlien(xi, yi, alien2, alien2_m);
            }
            yi += 60;
            indice++;
        }
        indice++;
        xi = 24;
        xi += 80;
    }
}
 */
/*
int column = 0;
    for (int i = 0; i < COL_ALIENS; i++)
    {
        column++;
            for (int row = 0; row < ROW_ALIENS; row++)
            {
                if (row == 0) {
                    printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n",row, column, column + (COL_ALIENS * row), xi, yi);
                    aliens[column + (COL_ALIENS * row)] = createAlien(xi, yi, alien1, alien1_m);
                } else if (row == 1) {
                    printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n",row, column, column + (COL_ALIENS * row), xi, yi);
                    aliens[column + (COL_ALIENS * row)] = createAlien(xi, yi, alien2, alien2_m);
                } else {
                    printf("row = %d; col = %d; ind = %d; x = %d; y = %d\n",row, column, column + (COL_ALIENS * row), xi, yi);
                    aliens[column + (COL_ALIENS * row)] = createAlien(xi, yi, alien3, alien3_m);
                }
                xi += 80;
            }
        xi = 24;
        yi += 60;
    }
*/
/*
void createAliens() {
    aliens[0] = createAlien(24, 20, alien1, alien1_m);
    aliens[1] = createAlien(300, 20, alien2, alien2_m);
    aliens[2] = createAlien(600, 20, alien3, alien3_m);
}
 */


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
    return (Alien) {.x = xi, .y = yi, .img = alien_xpm, .img_mov = alien_xpm_mov, .width=60, .height=40, .alive = true};
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

