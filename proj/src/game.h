#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "graphic.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "menu.h"
#include "utils.h"
#include "mouse.h"
#include "ship.h"
#include "timer.h"
#include "shipBullet.h"
#include "alienBullet.h"
#include "alien.h"
#include "rtc.h"

#include "macros.h"

typedef enum {
    MENU_DISPLAY,
    INSTRUCTIONS_DISPLAY,
    PLAYING,
    NEXT_ROUND,
    EXIT
} game_state;

void (initGame)(Ship *ship);

int (game_loop)();

#endif //PROJ_GAME_H
