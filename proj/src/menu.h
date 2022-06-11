#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources.h"
//#include "mouse.h"

typedef enum {
    START_BUTTON,
    INSTRUCTIONS_BUTTON,
    EXIT_BUTTON,
    INITIAL
} Button;

void initMenuXpm();

int drawMenu(Button button);

Button getButton(uint16_t x, uint16_t y);

bool getInstructionButton(uint16_t x, uint16_t y);

void displayInstructions();

int drawInstructions(bool button);

#endif //PROJ_MENU_H
