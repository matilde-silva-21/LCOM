#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if(freq < 19 || freq > TIMER_FREQ)
    return 1;

  if(timer < 0 || timer > 2)
    return 1;

  uint8_t st;

  if(timer_get_conf(timer, &st))
    return 1;

  st = (st & (TIMER_STATUS_MODE | TIMER_STATUS_BASE));

  uint16_t value = TIMER_FREQ / freq;

  uint8_t controlword = TIMER_LSB_MSB | st;

  uint8_t timer_reg = TIMER_0;

  switch(timer){
    case 0:
      controlword |= TIMER_SEL0;
      break;
    case 1:
      controlword |= TIMER_SEL1;
      timer_reg = TIMER_1;
      break;
    case 2:
      controlword |= TIMER_SEL2;
      timer_reg = TIMER_2;
      break;
    default:
      return 1;
  }

  if(sys_outb(TIMER_CTRL, controlword))
    return 1;

  uint8_t lsb, msb;

  if(util_get_LSB(value, &lsb))
    return 1;

  if(util_get_MSB(value, &msb))
    return 1;

  if(sys_outb(timer_reg, lsb))
    return 1;

  if(sys_outb(timer_reg, msb))
    return 1;

  return 0;
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

  if (st == NULL)
    return 1;

  if (timer < 0 || timer > 2)
    return 1;

  uint8_t rb_command = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

  if (sys_outb(TIMER_CTRL, rb_command))
    return 1;

  uint8_t timer_reg = TIMER_0;

  switch(timer){
    case 0:
      timer_reg = TIMER_0;
      break;
    case 1:
      timer_reg = TIMER_1;
      break;
    case 2:
      timer_reg = TIMER_2;
      break;
    default:
      break;
  }

  if (util_sys_inb(timer_reg, st))
    return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  if (timer != 0 && timer != 1 && timer != 2) {
    return 1;
  }

  union timer_status_field_val val;
  uint8_t aux;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      aux = ((st & TIMER_STATUS_INIT) >> 4);
      if (aux >= 1 && aux <= 3)
        val.in_mode = aux;
      else {
        val.in_mode = INVAL_val;
      }
      break;
    case tsf_mode:
      aux = ((st & TIMER_SQR_WAVE) >> 1);
      val.count_mode = aux;
      break;
    case tsf_base:
      aux = (st & TIMER_BCD);
      val.bcd = aux;
      break;
    default:
      break;
  }
  if (timer_print_config(timer, field, val))
    return 1;
  return 0;
}
