#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources.h"
#include "mouse.h"

#include "resources/menu2.xpm"
#include "resources/menu_start.xpm"
#include "resources/menu_instructions.xpm"
#include "resources/menu_hallOfFame_1.xpm"
#include "resources/menu_exit.xpm"

/*
typedef struct{
    uint16_t xi;
    uint16_t yi;
    uint16_t xf;
    uint16_t yf;
} StartButton;

typedef struct{
    uint16_t xi;
    uint16_t yi;
    uint16_t xf;
    uint16_t yf;
} InstructionsButton;

typedef struct{
    uint16_t xi;
    uint16_t yi;
    uint16_t xf;
    uint16_t yf;
} HallOfFameButton;

typedef struct{
    uint16_t xi;
    uint16_t yi;
    uint16_t xf;
    uint16_t yf;
} ExitButton;
*/

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
