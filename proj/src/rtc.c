#include "rtc.h"

int rtc_hook_id = 29;
int speed = 1;
int frames_per_state = 20;
extern bool playing;
extern int roundNum;

int (rtc_subscribe_int)(uint8_t *bit_no) {

    *bit_no = rtc_hook_id;
    if (sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id))
        return 1;

    return 0;
}

int (rtc_unsubscribe_int)() {
    if (sys_irqrmpolicy(&rtc_hook_id))
        return 1;

    return 0;
}

int (rtc_enable)(uint8_t *bit_no) {

    printf("enable");
    *bit_no = rtc_hook_id;
    if (sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id))
        return 1;

    uint8_t data;
    printf("bit: %d\n", *bit_no);

    if (rtc_read(&data, REG_B)) {
        return 1;
    }

    data |= PIE;

    if (rtc_write(data, REG_B)) {
        return 1;
    }

    if (rtc_read(&data, REG_A)) {
        return 1;
    }

    data |= RS3 | RS2 | RS1 | RS0;

    if (rtc_write(data, REG_A)) {
        return 1;
    }

    return 0;
}

int (rtc_disable)() {

    if (rtc_unsubscribe_int())
        return 1;

    uint8_t data;

    if (rtc_read(&data, REG_B)) {
        return 1;
    }

    data &= ~PIE;

    if (rtc_write(data, REG_B)) {
        return 1;
    }

    if (rtc_read(&data, REG_A)) {
        return 1;
    }

    data &= ~(RS3 & RS2 & RS1 & RS0);

    if (rtc_write(data, REG_A)) {
        return 1;
    }

    return 0;
}

void (rtc_ih)() {
    printf("Entered rtc\n");
    uint8_t regC;
    sys_outb(RTC_ADDR_REG, REG_C);
    util_sys_inb(RTC_DATA_REG, &regC);
    if (regC & PF) {
        speed = speed + (roundNum * roundNum);
        frames_per_state--;
        printf("speed: %d\n", speed);
    }
}

int rtc_read(uint8_t *data, uint8_t reg) {

    if (sys_outb(RTC_ADDR_REG, reg))
        return 1;

    if (util_sys_inb(RTC_DATA_REG, data))
        return 1;

    return 0;
}

int rtc_write(uint8_t data, uint8_t reg) {

    if (sys_outb(RTC_ADDR_REG, reg))
        return 1;

    if (sys_outb(RTC_DATA_REG, data))
        return 1;

    return 0;
}











