#ifndef PROJ_RTC_H
#define PROJ_RTC_H

#include <lcom/lcf.h>
#include "util.h"

#define IRQ_RTC 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

//Control/Status Register A
#define RS3 BIT(3)
#define RS2 BIT(2)
#define RS1 BIT(1)
#define RS0 BIT(0)  

//Control/Status Register B
#define PIE BIT(6)

//Control/Status Register C
#define IRQF BIT(7)
#define PF BIT(6)

#define REG_A 0x10
#define REG_B 0x11
#define REG_C 0x12

int (rtc_subscribe_int)(uint8_t *bit_no);

int(rtc_unsubscribe_int)();

void rtc_ih();

int (rtc_enable)();

int (rtc_disable)();

int rtc_read(uint8_t *data, int reg);

int rtc_write(uint8_t data, int reg);

#endif
