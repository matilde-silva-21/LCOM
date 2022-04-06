#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

extern int cnt;
extern int hook_id;
extern uint8_t scancode, statuscode;
extern bool kbc_iherr;

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

  uint8_t size; // nº de bytes no scancode
  uint8_t bytes[2]; // 1 byte por posição (pode ter 1 ou 2)
  bool twoBytes = false;
  bool make;
  uint8_t irq_set = 0;

  while(scancode != ESC_BREAK_CODE){
    size = 0;
    /* Get request message. */
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0){
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          //if(msg.m_notify.interrupts & BIT(irq_set)){
          if(msg.m_notify.interrupts & irq_set){
            //esta linha de codigo nao devia ser exclusiva ao IH?-acho que sim
            /*
            if(util_sys_inb(STATREG, &statuscode))
              return 1;
              */
            kbc_ih();
            if(kbc_iherr)
              continue;
            if(twoBytes){
              bytes[0] = 0xE0;
              bytes[1] = scancode;
              size = 2;
              twoBytes = false; // para a próxima iteração
              if((scancode & MAKE_BIT) >> 7)
                make = false;
              else
                make = true;
            }
            else {
              if (scancode == 0xE0) {
                twoBytes = true;
                bytes[1] = 0xE0;
                size = 2;
                continue; // como se lê o scancode byte a byte, fazemos continue para ir buscar o LSB
              }
              else{
                bytes[0] = scancode;
                size = 1;
                twoBytes = false;
                if((scancode & MAKE_BIT) >> 7)
                  make = false;
                else
                  make = true;
              }
              kbd_print_scancode(make, size, bytes);
            }
          }
          break;
        default:
          break;
      }
    }
    else {}
  }

  //aqui nao era suposto verificar se o buffer esta full antes de poder escrever (slide 18)
  //acho que não é preciso porque estás a dar flush e já não interessa o que possa estar lá(not sure tho)
  if(sys_outb(OUT_BUF, 0x01))
    return 1;

  if(keyboard_unsubscribe())
    return 1;

  if(kbd_print_no_sysinb(cnt))
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
