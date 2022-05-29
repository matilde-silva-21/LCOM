// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
#include "timer.h"

extern int mouse_hookid, hookid, timer_counter;
extern uint8_t scancode, statuscode;
extern int ih_error;

extern bool existEvent;

extern state_t pattern_state;
extern struct packet previousPacket;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
  uint32_t mouse_bit_no = BIT(mouse_hookid);

  if (mouse_subscribe_int(&mouse_hookid)) {
    return 1;
  }

  if(enable_mouse(ENABLE_MOUSE)) {
    return 1;
  }


  int ipc_status, r;
  message msg;
  struct packet pp;
  int currentByte = 1;
  uint8_t bytes[3];

  while (cnt > 0) {
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
              bytes[0] = scancode;
            }
            else if(currentByte == 2) {
              currentByte = 3;
              bytes[1] = scancode;
            }
            else if(currentByte == 3) {
              currentByte = 1;
              cnt--;
              bytes[2] = scancode;
              getMousePacket(&pp, bytes);
              mouse_print_packet(&pp);
            }
          }
          break;
        default:
          break;
      }
    } else {
    }
  }

  if(enable_mouse(DISABLE_MOUSE)) {
    return 1;
  }

  if(mouse_unsubscribe_int())
    return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {

uint8_t timer_bit_no = BIT(hookid);
uint32_t mouse_bit_no = BIT(mouse_hookid);

if (mouse_subscribe_int(&mouse_hookid)) {
  return 1;
}

if(enable_mouse(ENABLE_MOUSE)) {
  return 1;
}

if(timer_subscribe_int(&timer_bit_no))
  return 1;

int ipc_status, r;
message msg;
struct packet pp;
int currentByte = 1;
uint8_t bytes[3];
uint32_t aux_counter = 0;

while (aux_counter <= idle_time) { /* You may want to use a different condition */
  /* Get a request message. */
  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
    continue;
  }
  if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
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
            bytes[0] = scancode;
          }
          else if(currentByte == 2) {
            currentByte = 3;
            bytes[1] = scancode;
          }
          else if(currentByte == 3) {
            currentByte = 1;
            //cnt--;
            bytes[2] = scancode;
            getMousePacket(&pp, bytes);
            mouse_print_packet(&pp);
          }
          timer_counter = 0;
          aux_counter = 0;
        }
        if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
          if (timer_counter % 60 == 0) //se passar um segundo, incrementamos o aux_counter
          {
            aux_counter ++;
          }

          timer_int_handler();
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
    }
  } else {
  }
}

if (timer_unsubscribe_int())
  return 1;

if(enable_mouse(DISABLE_MOUSE)) {
  return 1;
}

if(mouse_unsubscribe_int())
  return 1;

return 0;
}

int (mouse_test_gesture)(uint8_t lineXLen, uint8_t tolerance) {

    uint32_t mouse_bit_no = BIT(mouse_hookid);

    if (mouse_subscribe_int(&mouse_hookid)) {
      return 1;
    }

    if(enable_mouse(ENABLE_MOUSE)) {
      return 1;
    }

    int ipc_status, r;
    message msg;
    struct packet pp;
    int currentByte = 1;
    uint8_t bytes[3];

    struct mouse_ev event;

    previousPacket.lb = false;
    previousPacket.rb = false;
    previousPacket.mb = false;

    while (pattern_state != DETECTED) {
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
                bytes[0] = scancode;
              }
              else if(currentByte == 2) {
                currentByte = 3;
                bytes[1] = scancode;
              }
              else if(currentByte == 3) {
                currentByte = 1;
                bytes[2] = scancode;
                getMousePacket(&pp, bytes);
                mouse_print_packet(&pp);

                event = mouse_get_event(&pp);
              }
            }
            break;
          default:
            break;
        }
      } else {
      }
      if(existEvent)
        mouse_check_pattern(event, tolerance, lineXLen);
    }

    if(enable_mouse(DISABLE_MOUSE)) {
      return 1;
    }

    if(mouse_unsubscribe_int())
      return 1;

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
