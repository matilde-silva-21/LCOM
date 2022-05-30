#include "mouse.h"
#include "utils.h"

int mouse_hookid = 0;
uint8_t mouse_scancode = 0, mouse_statuscode = 0;
int ih_error = 0;

Mouse* mouse;

int(mouse_subscribe_int)(int *bit_no) {
    *bit_no = mouse_hookid;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid))
        return 1;

    return 0;
}

int(mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hookid))
        return 1;

    return 0;
}

void(mouse_ih)() {
    util_sys_inb(STAT_REG, &mouse_statuscode);

    if ((mouse_statuscode & (PARITY_BIT | TIMEOUT_BIT)) == 0 && (mouse_statuscode & OBF_BIT)) {
        util_sys_inb(OUT_BUF, &mouse_scancode);
        ih_error = 0;
    }else ih_error = 1;
    util_sys_inb(OUT_BUF, &mouse_scancode);

}

void(getMousePacket)(struct packet *pp, uint8_t bytes[3]) {
    // bytes
    pp->bytes[0] = bytes[0];
    pp->bytes[1] = bytes[1];
    pp->bytes[2] = bytes[2];

    // buttons
    if (bytes[0] & RIGHT)
        pp->rb = true;
    else
        pp->rb = false;
    if (bytes[0] & LEFT)
        pp->lb = true;
    else
        pp->lb = false;
    if (bytes[0] & MIDDLE)
        pp->mb = true;
    else
        pp->mb = false;

    // x and y displacement
    if (bytes[0] & X_SIGN)
        pp->delta_x = bytes[1] | FILLMSB;
    else
        pp->delta_x = (uint16_t) bytes[1];
    if (bytes[0] & Y_SIGN)
        pp->delta_y = bytes[2] | FILLMSB;
    else
        pp->delta_y = (uint16_t) bytes[2];

    // x and y overflow
    if (bytes[0] & X_OVFL)
        pp->x_ov = true;
    else
        pp->x_ov = false;
    if (bytes[0] & Y_OVFL)
        pp->y_ov = true;
    else
        pp->y_ov = false;
}


int (send_mouse_command)(uint8_t cmd){

    if(sys_irqdisable(&mouse_hookid))
        return 1;
     
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

        //tickdelay(micros_to_delay(20000));

        if(util_sys_inb(OUT_BUF, &ack))
            return 1;

        if(ack == ACK_ERROR)
            return 1;

    }while(ack != ACK);
    
    if(sys_irqenable(&mouse_hookid))
        return 1;
        
    return 0;
}

void (updateMouseCoordinates)(struct packet* pp){
    mouse->x += pp->delta_x;
    mouse->y += pp->delta_y;
}
