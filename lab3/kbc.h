#ifndef LAB3_KBC_H
#define LAB3_KBC_H

int(keyboard_subscribe)(int *bit_no);

int(keyboard_unsubscribe)();

int(util_sys_inb)(int port, uint8_t *value);

void(kbc_ih)(void);

#endif // LAB3_KBC_H
