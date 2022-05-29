#include "menu.h"
#include "resources.h"

xpm_image_t menu;

extern Mouse * mouse;
//Button button;

int drawMenu(){

    menu = loadXpm(menu2);
    if(drawXpm(0, 0, menu)){
        return 1;
    }

    return 0;
}

//chamar quando o botao esquerdo e carregado no menu
Button getPressedButton(uint16_t x, uint16_t y){
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
        return Invalid;
    }
}


