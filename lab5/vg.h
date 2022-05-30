#ifndef LAB5_VG_H
#define LAB5_VG_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "kbc.h"
#include "utils.h"
#include "vbemacros.h"

int vg_set_mode(uint16_t *mode);

int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t* info);

void vg_drawpixel(uint16_t x, uint16_t y, uint32_t color);

int vg_drawline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int vg_drawrectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int wait_esckey();

int vg_drawpattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img);

int vg_initialize(uint16_t mode);

int draw_next_xpm(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf);

#endif // LAB5_VG_H
