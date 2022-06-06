#include "mouse.h"
#include "utils.h"

int mouse_hookid = 12;
uint8_t mouse_scancode = 0, mouse_statuscode = 0;
int ih_error = 0;

xpm_image_t menu;

Mouse *mouse;

uint16_t Xres;

int (mouse_subscribe_int)(int *bit_no) {
    *bit_no = mouse_hookid;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid))
        return 1;

    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hookid))
        return 1;

    return 0;
}

void (mouse_ih)() {
    util_sys_inb(STAT_REG, &mouse_statuscode);

    if ((mouse_statuscode & (PARITY_BIT | TIMEOUT_BIT)) == 0 && (mouse_statuscode & OBF_BIT)) {
        util_sys_inb(OUT_BUF, &mouse_scancode);
        ih_error = 0;
    }else
        ih_error = 1;

    util_sys_inb(OUT_BUF, &mouse_scancode);
}

void (getMousePacket)(struct packet *pp, uint8_t bytes[3]) {
    // bytes
    pp->bytes[0] = bytes[0];
    pp->bytes[1] = (int16_t)bytes[1];
    pp->bytes[2] = (int16_t)bytes[2];

    // buttons
    pp->lb = bytes[0] & LEFT;
    pp->rb = bytes[0] & RIGHT;
    pp->mb = bytes[0] & MIDDLE;

    // x and y displacement
    if (bytes[0] & X_SIGN)
        pp->delta_x |= FILLMSB;
    if (bytes[0] & Y_SIGN)
        pp->delta_y |= FILLMSB;

    // x and y overflow
    pp->x_ov = bytes[0] & X_OVFL;
    pp->y_ov = bytes[0] & Y_OVFL;
}
/*
int (write_command)(int port, uint8_t cmd){
    int tries = 10;
    do {
        if (util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) { // checks if we can write
            tries--;
            continue;
        }
        if (sys_outb(STAT_REG, MOUSE_COMMAND))
            return 1;
    }while(tries > 0);
}
*/
/*
int (send_mouse_command)(uint8_t cmd) {

    if (sys_irqdisable(&mouse_hookid))
        return 1;

    uint8_t ack;
    do {
        if (util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) // checks if we can write
            continue;
        if (sys_outb(STAT_REG, MOUSE_COMMAND))
            return 1;

        if (util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) // checks if we can write
            continue;
        if (sys_outb(ARGS_REG, cmd))
            return 1;

        tickdelay(micros_to_ticks(20000));

        if (util_sys_inb(OUT_BUF, &ack))
            return 1;

        if (ack == ACK_ERROR)
            return 1;

    } while (ack != ACK);

    if (sys_irqenable(&mouse_hookid))
        return 1;

    return 0;
}
*/

int (send_mouse_command)(uint8_t cmd){
    uint8_t ack;
    do{
        if(util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) // checks if we can write
            continue;
        if(sys_outb(STAT_REG, MOUSE_COMMAND))
            return 1;

        if(util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) // checks if we can write
            continue;
        if(sys_outb(ARGS_REG, cmd))
            return 1;

        tickdelay(micros_to_ticks(20000));

        if(util_sys_inb(OUT_BUF, &ack))
            return 1;

        if(ack == ACK_ERROR)
            return 1;

    }while(ack != ACK);

    return 0;
}

static int clamp(int min, int max, int value) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void (updateMouse)(struct packet *pp, Mouse *mouse) {

    mouse->x = clamp(0, X_RES - mouse->img.width - 1, mouse->x + pp->delta_x/5);
    mouse->y = clamp(0, Y_RES - mouse->img.height - 1, mouse->y - pp->delta_y/5);

    mouse->lb_pressed = pp->lb;
}

int vg_drawrectangle(int x, int y, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            drawPixel(x + i, y + j, 0);
        }
    }
    return 0;
}

Mouse *(createMouse)(int x, int y){
    Mouse *mouse = (Mouse *) malloc(sizeof(Mouse));

    mouse->x = x;
    mouse->y = y;
    mouse->lb_pressed = false;

    xpm_image_t mouse_img;
    xpm_load(mouse_xpm, XPM_INDEXED, &mouse_img);

    mouse->img = mouse_img;

    return mouse;
}

void (drawMouse)(Mouse *mouse) {
    drawXpm(mouse->x, mouse->y, mouse->img);
/*
    uint32_t *map = (uint32_t *) mouse->img.bytes;

    for (int i = 0; i < mouse->img.width; i++) {
        for (int j = 0; j < mouse->img.height; j++) {
            if (*(map + i + j * mouse->img.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(mouse->x + i, mouse->y + j, *(map + i + j * mouse->img.width));
        }
    }
    */
}

void (destroyMouse)(Mouse *mouse){
    if (mouse == NULL)
        return;
    free(mouse);
    mouse = NULL;
}
