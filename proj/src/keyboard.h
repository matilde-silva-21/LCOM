#ifndef PROJ_KEYBOARD_H
#define PROJ_KEYBOARD_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

int (kbd_subscribe_int) (uint8_t *bit_no);

int (kbd_unsubscribe_int) ();

void (kbc_ih) ();

int (twoBytes) (uint8_t scan_code);


#endif //PROJ_KEYBOARD_H
