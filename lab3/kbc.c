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

int(read_command)(uint8_t* command) {
  int tries = 10;
  //este loop espera ate dar para ler
  while(tries > 0){

    if(util_sys_inb(STATREG, &statuscode)) // vamos buscar o statuscode para verificar o estado do buffer
      return 1;

    if(statuscode & (IBF | OBF)) { // se o input ou o output buffer estiverem full dá continue e tries é decrementado
      tries --;
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }
    else{ //senao nao estiverem full podemos "pedir" para ler e obter o command atual
      if (sys_outb(IN_BUF, READ_CMBYTE)) // 0x64 recebe o comando de leitura
        return 1;

      if (util_sys_inb(OUT_BUF, command)) { // vamos buscar ao 0x60 o valor de retorno do comando anterior(command byte)
        return 1;
      }
      return 0;
    }
  }

  if(tries == 0) // chegou ao fim das 10 tentativas -> erro
    return 1;

  return 0;
}

int(write_command)(uint8_t* command) { //aqui vemos se podemos escrever o comando
  int tries = 10;
  //este loop espera ate dar para escrever
  while(tries > 0){

    if(util_sys_inb(STATREG, &statuscode)) // vamos buscar o statuscode para verificar o estado do buffer
      return 1;

    if(statuscode & (IBF | OBF)) { // se o input ou o output buffer estiverem full dá continue e tries é decrementado
      tries --;
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }
    else{ //senao nao estiverem full podemos escrever
      if (sys_outb(IN_BUF, WRITE_CMBYTE)) // 0x64 recebe o comando de escrita
        return 1;

      if (sys_outb(IN_ARGS, *command)) // 0x60 recebe o argumento do comando anterior
        return 1;

      return 0; // o comando e o argumento foram escritos com sucesso
    }
  }

  if(tries == 0) // chegou ao fim das 10 tentativas -> erro
    return 1;

  return 0;
}

int(kbc_reenable_int)() {

  uint8_t cm_byte;

  if (read_command(&cm_byte)){ // vamos buscar o command byte
    return 1;
  }

  cm_byte = cm_byte | ENABLE_INT_KBD; // escrevemos um novo command byte onde damos enable às interrupções do keyboard

  if (write_command(&cm_byte)) // escrevemos o novo command byte
    return 1;

  return 0;
}


void(kbc_ih)(void){
  //esta funcao le o status register e o out_buf e mediante o valor do SR ignora ou nao o out_buf
  //All communication with other code must be done via global variables, static if possible.

<<<<<<< HEAD
=======
  util_sys_inb(STATREG, &statuscode); // primeiro vamos buscar o statuscode e só quando soubermos que nao ha erros - OBF, PARITY e TIMEOUTERR - é que vamos buscar o scancode

>>>>>>> b1d81682f73acc9dcacd3be9a734af0e74a9cedf
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
