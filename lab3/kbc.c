#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

int hook_id = 0;
int cnt = 0;
uint8_t scancode = 0x00, statuscode = 0x00;
bool kbc_iherr = false;

int(keyboard_subscribe)(int *bit_no){
  *bit_no = hook_id;

  if(sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id))
    return 1;

  return 0;
}

int(keyboard_unsubscribe)(){
  if(sys_irqrmpolicy(&hook_id))
    return 1;

  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {

  if (value == NULL)
    return 1;

  uint32_t temp;

  if (sys_inb(port, &temp))
    return 1;

  *value = (uint8_t) temp;

  #ifdef LAB3
    cnt++;
  #endif

  return 0;
}

void(kbc_ih)(void){
  //esta funcao le o status register e o out_buf e mediante o valor do SR ignora ou nao o out_buf
  //All communication with other code must be done via global variables, static if possible.

  if(util_sys_inb(STATREG, &statuscode) || util_sys_inb(OUT_BUF, &scancode)){kbc_iherr = true;}//verificar se util_sys funcionou direito

  //agora verificar se SR levantou algum erro

  if((statuscode & TIMEOUTERR) || (statuscode & PARITYERR)){
    scancode = 0x00; // se o SR der erro entao eu apago o que esta no OUT_BUF
  }

  kbc_iherr = true;
}
