#ifndef PROJ_MOUSE_H
#define PROJ_MOUSE_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"
//#include "graphic.h"
#include "macros.h"

#include "resources/mouse.xpm"

typedef struct {
    int x, y;
    xpm_image_t img;
    bool lb_pressed;
} Mouse;

int (mouse_subscribe_int)(int *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (getMousePacket)(struct packet *pp, uint8_t bytes[3]);

int (send_mouse_command)(uint8_t cmd);

Mouse *(createMouse)(int x, int y);

void (updateMouse)(struct packet *pp, Mouse *mouse);

//void (drawMouse)(Mouse *mouse);

void (removeMouse)(Mouse *mouse);

#endif //PROJ_MOUSE_H
