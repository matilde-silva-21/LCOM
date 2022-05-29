#include "graphic.h"

xpm_image_t (loadXpm)(xpm_map_t xpm) {
    xpm_image_t img;

    xpm_load(xpm, XPM_8_8_8_8, &img);

    return img;
}

void drawPixel(uint16_t x, uint16_t y, uint32_t color){
    uint8_t *aux = video_mem;
    uint32_t n = (bitsPerPixel + 7) >> 3; //arredonda para cima o nº de bytes na cor - tipo o ceil
    aux = aux + n *(y* Xres + x); // calcula onde colocar a cor
    memcpy(aux, &color, n); //copia n bytes da color para aux
}

int drawXpm(uint16_t x, uint16_t y, xpm_image_t img){
    int counter=0;

    for(int i=y; i<img.height+y; i++){
        for(int j=x; j<img.width+x; j++){
            drawPixel(j,i, img.bytes[counter++]);
        }
    }

    return 0;
}
