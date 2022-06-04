#include "game.h"

extern int mouse_hookid;
extern uint8_t keyboard_scancode, keyboard_statuscode;
extern uint8_t mouse_scancode, mouse_statuscode;
extern int ih_error;
extern int kbd_hookid; //, hookid, timer_counter;
extern int timer_counter;
// extern Mouse *mouse;

int(game_loop)() {
  // mouse->x = 0; mouse->y = 0;
  uint16_t mode = 0x105;

  initMenuXpm();

  vbe_mode_info_t info;
  if (vg_get_mode_info(&mode, &info)) {
    return 1;
  }

  if (vg_set_mode(&mode))
    return 1;

  int mouse_bit_no;
  uint8_t kbd_bit_no;

  if (mouse_subscribe_int(&mouse_bit_no)) {
    return 1;
  }

  if (send_mouse_command(ENABLE_MOUSE)) {
    return 1;
  }

  if (kbd_subscribe_int(&kbd_bit_no))
    return 1;

  int ipc_status, r; //, pack_count = 0;
  message msg;
  struct packet pp;
  int currentByte = 1;
  uint8_t mouseBytes[3];
  int size = 1;
  uint8_t kbdBytes[2];

  Button button = Initial;

  if (drawMenu(button))
    return 1;

  xpm_image_t mouse_img = loadXpm(mouse_xpm);

  Mouse mouse = {50, 50, mouse_img};

  drawMouse(&mouse);

  while (keyboard_scancode != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
            mouse_ih(); // read 1 byte per interrupt -> a packet has 3 bytes
            if (ih_error)
              continue;

            if (currentByte == 1) {
              // 1º byte do packet
              if ((mouse_scancode & BIT(3)) == 0) {
                continue; // if the bit(3) is different from 1 wait for the next interrupt
              }
              currentByte = 2; // for the next iteration
              mouseBytes[0] = mouse_scancode;
            }
            else if (currentByte == 2) {
              currentByte = 3;
              mouseBytes[1] = mouse_scancode;
            }
            else if (currentByte == 3) {
              currentByte = 1;
              mouseBytes[2] = mouse_scancode;
              getMousePacket(&pp, mouseBytes);
              mouse_print_packet(&pp);
              // eraseMouse(&mouse);
              updateMouseCoordinates(&pp, &mouse);
              fprintf(stderr, "x = %d     y = %d\n", mouse.x, mouse.y);
              button = getButton(mouse.x, mouse.y);
              printf("%d", button);
              if (drawMenu(button)) {
                return 1;
              }
              drawMouse(&mouse);
              if (pp.lb && button != Initial) {
              }
            }
          }
          if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
            kbc_ih();
            if (twoBytes(keyboard_scancode)) {
              size = 2;
              continue;
            }
            if (size == 1) {
              kbdBytes[0] = keyboard_scancode;
            }
            else {
              kbdBytes[0] = 0xE0;
              kbdBytes[1] = keyboard_scancode;
            }
            size = 1;
          }
          break;
        default:
          break;
      }
    }
  }

  if (send_mouse_command(DISABLE_MOUSE))
    return 1;

  if (mouse_unsubscribe_int())
    return 1;

  if (kbd_unsubscribe_int()) {
    return 1;
  }

  if (vg_exit())
    return 1;
  return 0;
}

int(alien_movement)(Alien aliens[], int sizeOfAliens) {

  bool gameOver = false;

  uint16_t mode = 0x105;

  int ipf = ((int) sys_hz()) / 60, speed = 2; // 60 = frame rate

  bool mov_img = false, right_mov = true;

  int frame_counter = 0, frames_per_state = 20;

  vbe_mode_info_t info;
  if (vg_get_mode_info(&mode, &info)) {
    return 1;
  }

  if (vg_set_mode(&mode))
    return 1;

  uint8_t timer_bit_no;
  uint8_t kbd_bit_no;

  if (kbd_subscribe_int(&kbd_bit_no))
    return 1;

  if (timer_subscribe_int(&timer_bit_no))
    return 1;

  int ipc_status, r;
  message msg;
  int size = 1;
  uint8_t kbdBytes[2];

  // xpm_image_t mouse_img = loadXpm(mouse_xpm);

  while (keyboard_scancode != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
            kbc_ih();
            if (twoBytes(keyboard_scancode)) {
              size = 2;
              continue;
            }
            if (size == 1) {
              kbdBytes[0] = keyboard_scancode;
            }
            else {
              kbdBytes[0] = 0xE0;
              kbdBytes[1] = keyboard_scancode;
            }
            size = 1;
          }

          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            timer_int_handler();
          }
          break;
        default:
          break;
      }
    }

    else {
    }
    if (!gameOver) {
      if (timer_counter >= ipf) {

        frame_counter++;
        xpm_image_t img = loadXpm(background);
        drawBackground(img);

        for (int i = 0; i < sizeOfAliens ; i++) {
          Alien *a = &aliens[i];
          if (right_mov) {
            change_alien_x_coordinates(a,speed);
            drawAlien(a, mov_img);
            if ((a->x + a->width) >= x_right_border) {
              right_mov = false;
              change_all_y(aliens, 20, sizeOfAliens);
              speed++;
              frames_per_state--;
            }
          }

          else {
            change_alien_x_coordinates(a, -speed);
            drawAlien(a, mov_img);
            if (a->x <= x_left_border) {
              change_all_y(aliens, 20, sizeOfAliens);
              right_mov = true;
              speed++;
              frames_per_state--;
            }
          }

          if((a->y+a->height) >= territory){
            gameOver = true;
            //drawBackground(img);
          }

        }
        timer_counter = 0;
        if(frame_counter >= frames_per_state){
          if(mov_img){mov_img = false;}
          else {mov_img = true;}
          frame_counter = 0;
        }
        

      }
    }
  }

  if (kbd_unsubscribe_int()) {
    return 1;
  }

  if (timer_unsubscribe_int())
    return 1;

  if (vg_exit())
    return 1;
  return 0;
}



