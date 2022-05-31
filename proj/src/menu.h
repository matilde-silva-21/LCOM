#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources.h"
//#include "mouse.h"

typedef enum{
    StartButton,
    InstructionsButton,
    HallOfFameButton,
    ExitButton,
    Initial
} Button;

int drawMenu(Button button);

Button getButton(uint16_t x, uint16_t y);

#endif //PROJ_MENU_H
