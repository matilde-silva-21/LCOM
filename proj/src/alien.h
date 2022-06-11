#pragma once

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "macros.h"
#include "resources/alien1.xpm"
#include "resources/alien1_m.xpm"
#include "resources/alien2.xpm"
#include "resources/alien2_m.xpm"
#include "resources/alien3.xpm"
#include "resources/alien3_m.xpm"

typedef struct{
    int x;
    int y;
    int width;
    int height;
    xpm_image_t img;
    xpm_image_t img_mov;
    bool alive;
    int value;
}Alien;


void change_alien_x_coordinates(Alien *a1, const int xf);

void change_alien_y_coordinates(Alien *a1, const int yf);

Alien createAlien(int xi, int yi, xpm_map_t img, xpm_map_t img_mov, int value);

void decrement_alien_x_coordinates(Alien *a1);

void decrement_alien_y_coordinates(Alien *a1);

void increment_alien_x_coordinates(Alien *a1);

int alien_get_x(Alien *a1);

int alien_get_y(Alien *a1);

int alien_get_width(Alien *a1);

int alien_get_height(Alien* a1);

void change_all_y(Alien aliens[], int yf, int size_array);
