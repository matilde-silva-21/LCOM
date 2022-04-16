#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

int kbd_hookid = 0;
uint8_t scancode = 0x00, statuscode = 0x00;
bool kbc_iherr = false;

int(keyboard_subscribe)(uint8_t *bit_no){
  *bit_no = kbd_hookid;

  if(sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hookid))
    return 1;

  return 0;
}

int(keyboard_unsubscribe)(){
  if(sys_irqrmpolicy(&kbd_hookid))
    return 1;

  return 0;
}

void(kbc_ih)(void){
  //esta funcao le o status register e o out_buf e mediante o valor do SR ignora ou nao o out_buf
  //All communication with other code must be done via global variables, static if possible.

  util_sys_inb(STATREG, &statuscode); // primeiro vamos buscar o statuscode e só quando soubermos que nao ha erros - OBF, PARITY e TIMEOUTERR - é que vamos buscar o scancode

  if(((statuscode & TIMEOUTERR) >> 6) || ((statuscode & PARITYERR) >> 7)){
    scancode = 0x00; // se o SR der erro entao eu apago o que esta no OUT_BUF
    kbc_iherr = true;
    return;
  }

  util_sys_inb(OUT_BUF, &scancode);
  kbc_iherr = false;
}

bool(kbc_makecode)(uint8_t scancode){
  if ((scancode & MAKE_BIT) >> 6)
    return false;
  else
    return true;
}
