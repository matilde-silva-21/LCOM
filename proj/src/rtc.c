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

//mete os peiodic interrupts a suponho eu 0.5s
int (setPeriodicInterrupts)(){

    uint8_t *stat_reg_b  = 0, *stat_reg_a = 0;
    if(rtc_read(stat_reg_b, REG_B) != 0){
        return 1;
    }

    *stat_reg_b = *stat_reg_b | PIE;

    if(rtc_write(*stat_reg_b, REG_B)!=0){
        return 1;
    }

    if(rtc_read(stat_reg_a, REG_A)!=0){
        return 1;
    }

    *stat_reg_a = *stat_reg_a | RS0 | RS1 | RS2 | RS3;

    if(rtc_write(*stat_reg_a, REG_A)!=0){
        return 1;
    }
        return 0;

    
}

int (rtc_unsubscribe_int)() {
    if (sys_irqrmpolicy(&rtc_hook_id))
        return 1;

    return 0;
}

int (rtc_enable)(uint8_t *bit_no) {

    printf("enable");
    
    if(rtc_subscribe_int(bit_no)!=0){
        return 1;
    }

    uint8_t *stat_reg_a = 0;
    bool update_in_progress = true;

    while(update_in_progress){

        if(rtc_read(stat_reg_a, REG_A)!=0){
            return 1;
        }

        update_in_progress = ((*stat_reg_a & BIT(7)) >> 7);

        printf("Status register A: %d\nupdate_in_progress: %d\n", *stat_reg_a, update_in_progress);
    }

    if(setPeriodicInterrupts()!=0){
        return 1;
    }

    return 0;
}

int (rtc_disable)() {

    if (rtc_unsubscribe_int())
        return 1;

    uint8_t *status_reg_b = 0;

    if(rtc_read(status_reg_b, REG_B)!=0){
        return 1;
    }

    //meter a 0 os periodic interrupts porque tinha metido a 1 antes (reset)
    *status_reg_b = *status_reg_b & (BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT(7));


    if(rtc_write(*status_reg_b, REG_B)!=0){
        return 1;
    }

    return 0;

}

int (rtc_ih)() {
    printf("Entered rtc interrupt handler\n");
    uint8_t *status_reg_c = 0;

    if(rtc_read(status_reg_c, REG_C)!=0){
        return 1;
    }

    bool irqf_set = ((*status_reg_c & IRQF) >> 7);

    bool pf_set = ((*status_reg_c & PF) >> 7);

    printf("status_reg_c: %d\nIRQF: %d\nPF: %d\n", status_reg_c, irqf_set, pf_set);

    if(irqf_set && pf_set){
        speed += 20;
        frames_per_state--;
        printf("speed: %d\n", speed);
    }

    return 0;
}

int (rtc_read)(uint8_t *data, uint8_t reg) {

    if (sys_outb(RTC_ADDR_REG, reg))
        return 1;

    if (util_sys_inb(RTC_DATA_REG, data))
        return 1;

    return 0;
}

int (rtc_write)(uint8_t data, int reg) {

    if (sys_outb(RTC_ADDR_REG, reg))
        return 1;

    if (sys_outb(RTC_DATA_REG, data))
        return 1;

    return 0;
}
