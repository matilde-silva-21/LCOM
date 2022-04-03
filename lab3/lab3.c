#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <kbc.c>

extern int cnt;
extern int hook_id;
extern uint8_t scancode, statuscode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  if(keyboard_subscribe(&hook_id))
    return 1;

  int ipc_status, r;
  message msg;


  while(scancode != ESC_BREAK_CODE){
    /* Get request message. */
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if(msg.m_notify.interrupts & BIT(irq_set)){
            //esta linha de codigo nao devia ser exclusiva ao IH?
            if(util_sys_inb(STRATEG, &statuscode))
              return 1;
            cnt++;
          }
          break;
        default:
          break;
      }
    }
    else {}

    kbc_ih();
  }

  //aqui nao era suposto verificar se o buffer esta full antes de poder escrever (slide 18)
  if(sys_outb(OUT_BUF, 0x01))
    return 1;

  if(keyboard_unsubscribe())
    return 1;

  uint8_t make = (scancode & MAKE_BIT) >> 7;

  if(make == 0)
    kbd_print_scancode(true, 1, scancode);
  if(make == 1)
    kbd_print_scancode(false, 1, scancode);

  if(kbd_print_no_scancode(cnt))
    return 1;

  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
