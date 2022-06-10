#include "menu.h"

//extern Mouse * mouse;

extern xpm_image_t menu;

xpm_image_t menu_start_xpm;
xpm_image_t menu_instructions_xpm;
xpm_image_t menu_exit_xpm ;
xpm_image_t menu_xpm;

xpm_image_t instructions_xpm;
xpm_image_t instructions_back_xpm;

void initMenuXpm(){
    xpm_load(menu_start, XPM_INDEXED, &menu_start_xpm);
    xpm_load(menu_instructions, XPM_INDEXED, &menu_instructions_xpm);
    xpm_load(menu_exit, XPM_INDEXED, &menu_exit_xpm);
    xpm_load(menu2, XPM_INDEXED, &menu_xpm);
    xpm_load(instructions, XPM_INDEXED, &instructions_xpm);
    xpm_load(instructions_back, XPM_INDEXED, &instructions_back_xpm);
}

//verificar a posiçao do rato no ecra do menu
Button getButton(uint16_t x, uint16_t y) {
    if (x > 392 && x < 632 && y > 288 && y < 345) {
        return StartButton;
    } else if (x > 208 && x < 817 && y > 400 && y < 457) {
        return InstructionsButton;
    } else if (x > 408 && x < 609 && y > 512 && y < 569) {
        return ExitButton;
    } else {
        return Initial;
    }
}

bool getInstructionButton(uint16_t x, uint16_t y) {
    if(x > 912 && x < 992 && y > 672 && y < 736)
        return true;
    return false;
}

//draws menu according to the mouse's position
int drawMenu(Button button) {
    xpm_image_t menu;

    switch (button) {
        case StartButton:
            menu = menu_start_xpm;
            break;
        case InstructionsButton:
            menu = menu_instructions_xpm;
            break;
        case ExitButton:
            menu = menu_exit_xpm;
            break;
        default:
            menu = menu_xpm;
            break;
    }
    drawBackground(menu);
    return 0;
}

int drawInstructions(bool button) {
    xpm_image_t menu;

    if(button){
        menu = instructions_back_xpm;
    }
    else menu = instructions_xpm;

    drawBackground(menu);
    return 0;
}

void displayInstructions(){
    xpm_image_t menu;
    drawBackground(menu);
}
