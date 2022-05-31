#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "utils.h"
#include "mousemacros.h"

typedef enum{INIT, DRAW_UP, DRAW_DOWN, END_UPLINE, DETECTED}state_t;
/*
typedef enum { RBDOWN, RBUP, LBDOWN, LBUP, MBDOWN,
               MBUP, MOVE } event_type_t;

typedef struct {
  event_type_t type;
  uint8_t moveX;
  uint8_t moveY;
  bool lbdown, rbdown, mbdown;
} mouse_event_t
*/
int (mouse_subscribe_int)(int* bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (getMousePacket)(struct packet* pp, uint8_t bytes[3]);

int (mouseStatus)(uint8_t* st);

int (send_mouse_command)(uint8_t cmd);

void mouse_check_pattern(struct mouse_ev evt, int tolerance, int lineXLen);

struct mouse_ev mouse_get_event(struct packet* pp);

#endif // LAB4_MOUSE_H
