#pragma once
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include "alien.h"

int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t* info);

int vg_set_mode(uint16_t *mode);

void drawPixel(uint16_t x, uint16_t y, uint32_t color);

xpm_image_t (loadXpm)(xpm_map_t xpm);

void drawBackground(xpm_image_t img);

int drawXpm(uint16_t x, uint16_t y, xpm_image_t img);

int drawAlien(Alien *a1, bool mov);


