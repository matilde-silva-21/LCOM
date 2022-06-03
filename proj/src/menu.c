#include "menu.h"

extern Mouse * mouse;

//chamar quando o botao esquerdo e carregado no menu
Button getButton(uint16_t x, uint16_t y){
    if(x > 392 && x < 632 && y > 288 && y < 345){
        return StartButton;
    }
    else if(x > 208 && x < 817 && y > 400 && y < 457){
        return InstructionsButton;
    }
    else if(x > 256 && x < 777 && y > 512 && y < 569){
        return HallOfFameButton;
    }
    else if(x > 408 && x < 609 && y > 624 && y < 681){
        return ExitButton;
    }
    else{
        return Initial;
    }
}

//draws menu according to the mouse's position
int drawMenu(Button button){
    xpm_image_t menu;
    switch(button){
        case StartButton:
            menu = loadXpm(menu_start);
            break;
        case InstructionsButton:
            menu = loadXpm(menu_instructions);
            break;
        case HallOfFameButton:
            menu = loadXpm(menu_hallOfFame_1);
            break;
        case ExitButton:
            menu = loadXpm(menu_exit);
            break;
        default:
            menu = loadXpm(menu2);
            break;
    }
    if(drawXpm(0, 0, menu)){
        return 1;
    }
    return 0;
}
