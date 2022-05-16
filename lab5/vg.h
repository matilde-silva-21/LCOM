#ifndef LAB5_VG_H
#define LAB5_VG_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

int vg_setmode(uint16_t *mode);

int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t* info);

void vg_drawpixel(uint16_t x, uint16_t y, uint32_t color);

int vg_drawline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

#endif // LAB5_VG_H
