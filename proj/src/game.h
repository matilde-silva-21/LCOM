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
#include "mouse.h"
#include "timer.h"
#include "utils.h"
#include "resources/pixil-frame-0.xpm"
#include "resources.h"

#define x_left_border 20
#define x_right_border 1004

#define territory 668


int(game_loop)();

int(alien_movement)(Alien aliens[], int sizeOfAliens);


#endif // PROJ_GAME_H
