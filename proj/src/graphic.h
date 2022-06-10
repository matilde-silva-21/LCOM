#ifndef PROJ_GRAPHIC_H
#define PROJ_GRAPHIC_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "alien.h"
#include "mouse.h"

#include "resources/background.xpm"

int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t* info);

int vg_set_mode(uint16_t *mode);

void drawPixel(uint16_t x, uint16_t y, uint32_t color);

xpm_image_t (loadXpm)(xpm_map_t xpm);

void drawBackground(xpm_image_t img);

int drawXpm(int x, int y, xpm_image_t img);

xpm_image_t loadBackground();

void drawBackground();

void displayScreen();

int drawAlien(Alien *a1, bool mov);

void drawMouse(Mouse *mouse);

#endif //PROJ_GRAPHIC_H
