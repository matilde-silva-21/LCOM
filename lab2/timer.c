#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

int (timer_unsubscribe_int)() {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

void (timer_int_handler)() {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

    printf("timer_get_conf\n");


    if (st == NULL) {
        return 1;
    }

    if (timer < 0 || timer > 1)
        return 1;

    uint8_t rb_command = TIMER_RB_CMD  | TIMER_RB_SEL(timer)| TIMER_RB_COUNT_;

    if (sys_outb(TIMER_CTRL, rb_command))
        return 1;

    uint8_t timer_reg = 0x40 + timer;

    if (util_sys_inb(timer_reg, st))
        return 1;

    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                         enum timer_status_field field) {
    printf("timer_display_conf\n");

    if (timer != 0 && timer != 1 && timer != 2) {
        return 1;
    }

    union timer_status_field_val val;
    uint8_t aux;

    switch (field) {
        case tsf_all:
            val.byte = field;
            if (timer_print_config(timer, field, val))
                return 1;
            return 0;
        case tsf_initial:
            aux = ((st & TIMER_STATUS_INIT) >> 4);
            if (aux >= 1 && aux <= 3)
                val.in_mode = aux;
            else {
                val.in_mode = INVAL_val;
            }
            if (timer_print_config(timer, field, val))
                return 1;
            return 0;
        case tsf_mode:
            aux = ((st & TIMER_STATUS_MODE) >> 1);
            val.count_mode = aux;
            if (timer_print_config(timer, field, val))
                return 1;
            return 0;
        case tsf_base:
            aux = (st & TIMER_STATUS_BASE);
            if (aux == 1)
                val.bcd = true;
            else
                val.bcd = false;
            if (timer_print_config(timer, field, val))
                return 1;
            return 0;
        default:
            break;
    }
    return 0;
}
