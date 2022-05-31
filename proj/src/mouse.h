#ifndef PROJ_MOUSE_H
#define PROJ_MOUSE_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"
#include "graphic.h"

typedef struct{
    int x, y;
    xpm_image_t img;
} Mouse;

int(mouse_subscribe_int)(int *bit_no);

int(mouse_unsubscribe_int)();

int vg_drawrectangle(int x, int y, int width, int height);

void(mouse_ih)();

void(getMousePacket)(struct packet *pp, uint8_t bytes[3]);

int (send_mouse_command)(uint8_t cmd);

void (updateMouseCoordinates)(struct packet* pp, Mouse * mouse);

void (eraseMouse)(Mouse * mouse);

void (drawMouse)(Mouse * mouse);

#endif //PROJ_MOUSE_H
