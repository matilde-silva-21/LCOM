#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "utils.h"
#include "mousemacros.h"

int (mouse_subscribe_int)(int* bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (getMousePacket)(struct packet* pp, uint8_t bytes[3]);
/*
int (write_command)(uint8_t port, uint8_t cmd);

int (sendCommand)(uint8_t cmd);
*/

int (mouseStatus)(uint8_t* st);

int (enable_mouse)(uint8_t cmd);

int (disable_mouse)(uint8_t cmd);

#endif // LAB4_MOUSE_H
