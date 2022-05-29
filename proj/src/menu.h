#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources/menu2.xpm"
#include "mouse.h"

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
    Invalid;
} Button;

int drawMenu();

#endif //PROJ_MENU_H
