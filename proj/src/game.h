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

#include "resources/pixil-frame-0.xpm"

int (game_loop)();

#endif //PROJ_GAME_H
