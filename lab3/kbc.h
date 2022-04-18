#ifndef LAB3_KBC_H
#define LAB3_KBC_H

int(keyboard_subscribe)(uint8_t *bit_no);

int(keyboard_unsubscribe)();

int(read_command)(uint8_t* command);

int(write_command)(uint8_t* command);

int(kbc_reenable_int)();

void(kbc_ih)(void);

bool(kbc_makecode)(uint8_t scancode);

#endif // LAB3_KBC_H
