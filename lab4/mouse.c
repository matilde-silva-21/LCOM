#include "mouse.h"

int mouse_hookid = 12;
uint8_t scancode = 0, statuscode = 0;
int ih_error = 0;

state_t pattern_state = INIT;

struct packet previousPacket;

bool existEvent = false;

int(mouse_subscribe_int)(int *bit_no) {
  *bit_no = mouse_hookid;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid))
    return 1;

  return 0;
}

int(mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hookid))
    return 1;

  return 0;
}

void(mouse_ih)() {
  util_sys_inb(STAT_REG, &statuscode);

  if ((statuscode & (PARITY_BIT | TIMEOUT_BIT)) == 0 && (statuscode & OBF_BIT)) {
    util_sys_inb(OUT_BUF, &scancode);
    ih_error = 0;
  }
  else
    ih_error = 1;
  util_sys_inb(OUT_BUF, &scancode);
}

void(getMousePacket)(struct packet *pp, uint8_t bytes[3]) {
  // bytes
  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];

  // buttons
  if (bytes[0] & RIGHT)
    pp->rb = true;
  else
    pp->rb = false;
  if (bytes[0] & LEFT)
    pp->lb = true;
  else
    pp->lb = false;
  if (bytes[0] & MIDDLE)
    pp->mb = true;
  else
    pp->mb = false;

  // x and y displacement
  if (bytes[0] & X_SIGN)
    pp->delta_x = bytes[1] | FILLMSB;
  else
    pp->delta_x = (uint16_t) bytes[1];
  if (bytes[0] & Y_SIGN)
    pp->delta_y = bytes[2] | FILLMSB;
  else
    pp->delta_y = (uint16_t) bytes[2];

  // x and y overflow
  if (bytes[0] & X_OVFL)
    pp->x_ov = true;
  else
    pp->x_ov = false;
  if (bytes[0] & Y_OVFL)
    pp->y_ov = true;
  else
    pp->y_ov = false;
}

int(mouseStatus)(uint8_t *st) {
  if (util_sys_inb(STAT_REG, st))
    return 1;
  return 0;
}

int (send_mouse_command)(uint8_t cmd){
  if(sys_irqdisable(&mouse_hookid))
    return 1;
  uint8_t ack, st;
  do {
    if (util_sys_inb(STAT_REG, &st))
      return 1;
    if (st & IBF_BIT) // checks if we can write
      continue;
    if (sys_outb(STAT_REG, MOUSE_COMMAND))
      return 1;

    if (util_sys_inb(STAT_REG, &st))
      return 1;

    if (st & IBF_BIT) // checks if we can write
      continue;

    if (sys_outb(ARGS_REG, cmd))
      return 1;

    tickdelay(micros_to_ticks(20000));

    if (util_sys_inb(OUT_BUF, &ack))
      return 1;

    if (ack == ACK_ERROR)
      return 1;

  } while (ack != ACK);
  return 0;
}

void mouse_check_pattern(struct mouse_ev evt, int tolerance, int lineXLen) {
  int XDif = 0;
  int YDIf = 0;
  switch (pattern_state) {
    case INIT: // estado inicial
      if (evt.type == LB_PRESSED)
        pattern_state = DRAW_UP;
      XDif = 0;
      YDIf = 0;
      break;
    case DRAW_UP: // desenhar a primeira linha
      if (evt.type == MOUSE_MOV) {
        if (evt.delta_x < 0 && -evt.delta_x > tolerance) {
          pattern_state = INIT;
        }
        if (evt.delta_y < 0 && -evt.delta_y > tolerance)
          pattern_state = INIT;
        XDif += evt.delta_x;
        YDIf += evt.delta_y;
      }
      else if (evt.type == LB_RELEASED) {
        if (XDif >= lineXLen && YDIf > XDif) { // verificar se a primeira linha é valida
          XDif = 0;
          YDIf = 0;
          pattern_state = END_UPLINE;
        }
        else {
          pattern_state = INIT;
        }
      }
      else if (evt.type == BUTTON_EV) {
        pattern_state = INIT;
      }
      break;
    case END_UPLINE: // acaba a primeira linha
      if(evt.type == RB_PRESSED){
        pattern_state = DRAW_DOWN;
      }
      else if(evt.type == LB_PRESSED){
        pattern_state = DRAW_UP;
      }
      else if(evt.type == BUTTON_EV){
        pattern_state = INIT;
      }
      else if(evt.type == MOUSE_MOV){
        if(abs(evt.delta_x) > tolerance || abs(evt.delta_y) > tolerance){
          pattern_state = INIT;
        }
      }
      break;
    case DRAW_DOWN: // desenhar a segunda linha
      if(evt.type == BUTTON_EV || evt.type == RB_PRESSED){
        pattern_state = INIT;
      }
      else if(evt.type == MOUSE_MOV){
        if (evt.delta_x < 0 && -evt.delta_x > tolerance) {
          pattern_state = INIT;
        }
        if (evt.delta_y < 0 && -evt.delta_y > tolerance)
          pattern_state = INIT;
      }
      else if(evt.type == RB_RELEASED){
        if(XDif >= lineXLen && abs(YDIf) > XDif){
          pattern_state = DETECTED;
        }
        else{
          pattern_state = INIT;
        }
      }
      break;
    case DETECTED:
      break;
    default:
      break;
  }
}

struct mouse_ev mouse_get_event(struct packet* pp){
  struct mouse_ev evt;
  if(previousPacket.lb == false && pp->lb == true){ // carregar no botao esquerdo
    if(pp->mb || pp->rb)
      evt.type = BUTTON_EV;
    else
      evt.type = LB_PRESSED;
    existEvent = true;
  }
  else if(previousPacket.rb == false && pp->rb == true){ // carregar botao direito
    if(pp->mb || pp->lb)
      evt.type = BUTTON_EV;
    else
      evt.type = RB_PRESSED;
    existEvent = true;
  }
  else if(previousPacket.lb == true && pp->lb == false){ // largar botao esquerdo
      evt.type = LB_RELEASED;
      existEvent = true;

  }
  else if(previousPacket.rb == true && pp->rb == false){ // largar botao direito
    evt.type = RB_RELEASED;
    existEvent = true;

  }
  else if(pp->delta_x != 0 || pp->delta_y != 0){
    evt.type = MOUSE_MOV;
    evt.delta_x = pp->delta_x;
    evt.delta_y = pp->delta_y;
    existEvent = true;

  }
  else{
    existEvent = false;
  }

  previousPacket = *pp;
  return evt;
}
