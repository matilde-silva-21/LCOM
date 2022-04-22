#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "i8254.h"
#include "kbc.h"
#include "timer.h"
#include "utils.h"

extern int cnt;
extern int kbd_hookid, timer0_hookid;
extern uint8_t scancode, statuscode;
extern bool kbc_iherr;
extern int timer_counter;

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

// nesta funcao como é que eu sei que se o scancode tiver de facto 2 bytes, vêm oS MSB antes dos LSB?
  uint8_t kbd_bit_no = kbd_hookid;

  if (keyboard_subscribe(&kbd_bit_no)) {
    return 1;
  }
  uint8_t irq_set = BIT(kbd_bit_no);

  int ipc_status, r;
  message msg;

  uint8_t size;          // nº de bytes no scancode
  uint8_t bytes[2];      // 1 byte por posição (pode ter 1 ou 2)
  bool twoBytes = false; // make;

  while (scancode != ESC_BREAK_CODE) {
    /* Get request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();

            if (kbc_iherr) {
              kbc_iherr = false;
              continue;
            }
            if (twoBytes) { // se o scancode tiver 2 bytes(na iteração anterior scancode = 0xE0)
                          // pomos no array bytes os dois, o size fica a dois e
                          // vemos se é make ou break code.
            
            bytes[1] = 0xE0;
            bytes[0] = scancode;
            size = 2;
            twoBytes = false; // para a próxima iteração
            kbd_print_scancode(kbc_makecode(scancode), size, bytes);
          }
          else { // se for a primeira iteração, verifica se o scancode tem dois ou um byte
            
            if (scancode == 0xE0) {
              twoBytes = true;
              continue; // como se lê o scancode byte a byte, fazemos continue para ir buscar o LSB
            }
            else { // só tem 1 byte, colocamos no array bytes, o size fica a 1 e twoBytes fica a falso
              bytes[0] = scancode; //aqui nao deviamos colocar bytes[1] a 0 em vez de deixar a 0xE0?
              size = 1;
              twoBytes = false;
              kbd_print_scancode(kbc_makecode(scancode), size, bytes);
            }
          }
        }
        break;
        
        default:
          break;
      }
    }
  }

  // aqui nao era suposto verificar se o buffer esta full antes de poder escrever (slide 18)
  // acho que não é preciso porque estás a dar flush e já não interessa o que possa estar lá(not sure tho)
  if (sys_outb(OUT_BUF, 0x01))
    return 1;

  if (keyboard_unsubscribe())
    return 1;

  if (kbd_print_no_sysinb(cnt))
    return 1;

  return 0;
}

int(kbd_test_poll)() {
  uint8_t size;     // nº de bytes no scancode
  uint8_t bytes[2]; // 1 byte por posição (pode ter 1 ou 2)
  bool twoBytes = false;
  statuscode = 0x00;
  kbc_iherr = false;

  while (scancode != ESC_BREAK_CODE) {
    //esta parte aqui é meio inutil porque quando chamo kbc_ih ja verifico estes erros
    util_sys_inb(STATREG, &statuscode);
    if ((statuscode & (OBF | MOUSEDATA)) != 0) {
      kbc_iherr = true;
    }


    kbc_ih();
    if (kbc_iherr)
      continue;
    if (twoBytes) { // se o scancode tiver 2 bytes(na iteração anterior scancode = 0xE0)
                    // pomos no array bytes os dois, o size fica a dois e
                    // vemos se é make ou break code.
      bytes[1] = 0xE0;
      bytes[0] = scancode;
      size = 2;
      twoBytes = false; // para a próxima iteração
    }
    else { // se for a primeira iteração, verifica se o scancode tem dois ou um byte

      if (scancode == 0xE0) {
        twoBytes = true;
        continue; // como se lê o scancode byte a byte, fazemos continue para ir buscar o LSB
      }
      else { // só tem 1 byte, colocamos no array bytes, o size fica a 1 e twoBytes fica a falso
        bytes[0] = scancode;
        size = 1;
        twoBytes = false;
      }
    }
    kbd_print_scancode(kbc_makecode(scancode), size, bytes);
    tickdelay(micros_to_ticks(DELAY_US));
  }
  if (sys_outb(OUT_BUF, 0x01))
    return 1;

  if (kbd_print_no_sysinb(cnt))
    return 1;

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  //acho que estas 2 linhas de codigo sao para escrever depois do subscribe, para ver se foi subscrito corretamente

  uint8_t kbc_bit_no = BIT(kbc_hookid); // nao sei bem o que fazemos aqui ?????????????????????
  uint8_t timer0_bit_no = BIT(timer0_hookid);

  if (kbc_subscribe_int(&kbc_bit_no))
    return 1;

  if(timer_subscribe_int(&timer0_bit_no))
    return 1;

  uint8_t aux_counter = 0; // para quando nao se recebe scancodes
  uint8_t bytes[2];
  int ipc_status, r, size = 1;
  message msg;
  while (scancode != ESC_BREAK_CODE && timer_counter <= n) { // loops until the scancode is the esc break code (release of the esc hey)
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                     /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(kbc_bit_no)) { /* subscribe*/
            kbc_ih();
            if (kbc_iherr)
              continue;
            if (twoBytes) { // se o scancode tiver 2 bytes(na iteração anterior scancode = 0xE0)
                            // pomos no array bytes os dois, o size fica a dois e
                            // vemos se é make ou break code.
              bytes[1] = 0xE0;
              bytes[0] = scancode;
              size = 2;
              twoBytes = false; // para a próxima iteração
            }
            else { // se for a primeira iteração, verifica se o scancode tem dois ou um byte

              if (scancode == 0xE0) {
                twoBytes = true;
                continue; // como se lê o scancode byte a byte, fazemos continue para ir buscar o LSB
              }
              else { // só tem 1 byte, colocamos no array bytes, o size fica a 1 e twoBytes fica a falso
                bytes[0] = scancode;
                size = 1;
                twoBytes = false;
              }
            }
            kbd_print_scancode(kbc_makecode(scancode), size, bytes);
            timer_counter = 0;
            aux_counter = 0;
          }
          if(msg.m_notify.interrupts & BIT(timer0_bit_no)){
            timer_int_handler();
            if (timer_counter % (int)sys_hz() == 0) //se passar um segundo, incrementamos o aux_counter
            {
              aux_counter ++;
            }
          }
        default:
          break;
      }
    }
    else {
    }
  }

  if(timer_unsubscribe_int())
    return 1;

  if (kbc_unsubscribe_int())
    return 1;

  // estava a dar erro de tempo
  /*
    if (kbd_print_no_sysinb(sys_counter))
      return 1;
  */
  return 0;
}
