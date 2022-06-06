#include "menu.h"

//extern Mouse * mouse;

extern xpm_image_t menu;

xpm_image_t menu_start_xpm;
xpm_image_t menu_instructions_xpm;
xpm_image_t menu_hallOfFame_xpm;
xpm_image_t menu_exit_xpm ;
xpm_image_t menu_xpm;

void initMenuXpm(){
    menu_start_xpm = loadXpm(menu_start);
    menu_instructions_xpm = loadXpm(menu_instructions);
    menu_hallOfFame_xpm = loadXpm(menu_hallOfFame_1);
    menu_exit_xpm = loadXpm(menu_exit);
    menu_xpm = loadXpm(menu2);
}

//verificar a posiçao do rato no ecra do menu
Button getButton(uint16_t x, uint16_t y) {
    if (x > 392 && x < 632 && y > 288 && y < 345) {
        return StartButton;
    } else if (x > 208 && x < 817 && y > 400 && y < 457) {
        return InstructionsButton;
    } else if (x > 256 && x < 777 && y > 512 && y < 569) {
        return HallOfFameButton;
    } else if (x > 408 && x < 609 && y > 624 && y < 681) {
        return ExitButton;
    } else {
        return Initial;
    }
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
        case HallOfFameButton:
            menu = menu_hallOfFame_xpm;
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

void displayInstructions(){

}
