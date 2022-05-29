#include <lcom/proj.h>
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "mouse.h"
#include "keyboard.h"
#include "i8254.h"
#include "i8042.h"

extern int mouse_hookid;
extern uint8_t scancode, statuscode;
extern int ih_error;
extern int kbd_hookid;//, hookid, timer_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int proj_main_loop(){
    uint32_t mouse_bit_no;
    uint8_t kbd_bit_no;

    if (mouse_subscribe_int(&mouse_hookid)) {
        return 1;
    }

    if(send_mouse_command(ENABLE_MOUSE)) {
        return 1;
    }

    if (kbd_subscribe_int(&kbd_bit_no))
        return 1;

    int ipc_status, r;//, pack_count = 0;
    message msg;
    struct packet pp;
    int currentByte = 1;
    uint8_t mouseBytes[3];
    int size = 1;
    uint8_t kbdBytes[2];

    while (scancode != ESC_BREAK) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & mouse_bit_no) {
                        mouse_ih(); // read 1 byte per interrupt -> a packet has 3 bytes
                        if(ih_error)
                            continue;

                        if(currentByte == 1){
                            //1º byte do packet
                            if((scancode & BIT(3)) == 0){
                                continue;//if the bit(3) is different from 1 wait for the next interrupt
                            }
                            currentByte = 2; // for the next iteration
                            mouseBytes[0] = scancode;
                        }
                        else if(currentByte == 2) {
                            currentByte = 3;
                            mouseBytes[1] = scancode;
                        }
                        else if(currentByte == 3) {
                            currentByte = 1;
                            mouseBytes[2] = scancode;
                            getMousePacket(&pp, mouseBytes);
                            mouse_print_packet(&pp);
                        }
                    }
                    if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
                        kbc_ih();
                        if (twoBytes(scancode)) {
                            size = 2;
                            continue;
                        }
                        if (size == 1) {
                            kbdBytes[0] = scancode;
                        } else {
                            kbdBytes[0] = 0xE0;
                            kbdBytes[1] = scancode;
                        }
                        size = 1;
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    if (send_mouse_command(DISABLE_MOUSE))
        return 1;

    if(mouse_unsubscribe_int())
        return 1;

  return 0;
}
