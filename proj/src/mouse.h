#ifndef PROJ_MOUSE_H
#define PROJ_MOUSE_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"

typedef struct{
    uint16_t x, y;
} Mouse;

int(mouse_subscribe_int)(int *bit_no);

int(mouse_unsubscribe_int)();

void(mouse_ih)();

void(getMousePacket)(struct packet *pp, uint8_t bytes[3]);

int (send_mouse_command)(uint8_t cmd);

void (updateMouseCoordinates)(struct packet* pp);
#endif //PROJ_MOUSE_H
