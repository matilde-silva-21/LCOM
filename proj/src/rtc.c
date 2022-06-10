#include "rtc.h"

int rtc_hook_id = 8;

int (rtc_subscribe_int)(uint8_t *bit_no){

    if(sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id))
        return 1;
    
    
}

int(rtc_unsubscribe_int)();

int(rtc_ih)();

int rtc_read(uint8_t *data, uint8_t reg);

int rtc_write(uint8_t data, uint8_t reg);











