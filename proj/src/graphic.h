#ifndef PROJ_GRAPHIC_H
#define PROJ_GRAPHIC_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

void drawPixel(uint16_t x, uint16_t y, uint32_t color);

xpm_image_t (loadXpm)(xpm_map_t xpm);

int drawXpm(uint16_t x, uint16_t y, xpm_image_t img);

#endif //PROJ_GRAPHIC_H
