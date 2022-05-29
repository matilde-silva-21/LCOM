#include <lcom/lab5.h>
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
#include "timer.h"
#include "vbemacros.h"
#include "vg.h"

extern int timer_counter;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  // vai buscar a informação do mode e mapear a memória necessária
  vbe_mode_info_t info;
  if (vg_get_mode_info(&mode, &info)) {
    return 1;
  }

  if (vg_set_mode(&mode))
    return 1;

  sleep(delay);

  if (vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  vbe_mode_info_t info;
  if (vg_get_mode_info(&mode, &info)) {
    return 1;
  }

  if (vg_set_mode(&mode))
    return 1;

  if (vg_drawrectangle(x, y, width, height, color))
    return 1;

  if (wait_esckey())
    return 1;

  if (vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vbe_mode_info_t info;
  if (vg_get_mode_info(&mode, &info)) {
    return 1;
  }

  if (vg_set_mode(&mode))
    return 1;

  if (vg_drawpattern(no_rectangles, first, step))
    return 1;

  if (wait_esckey())
    return 1;

  if (vg_exit() != 0)
    return 1;
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  uint16_t mode = 0x105;

  vbe_mode_info_t info;
  if (vg_get_mode_info(&mode, &info)) {
    return 1;
  }

  if (vg_set_mode(&mode))
    return 1;

  xpm_image_t img;

  xpm_load(xpm, XPM_INDEXED, &img);

  draw_xpm(x, y, img);

  if (wait_esckey())
    return 1;

  if (vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

  // n of interrupts per frame -- (int)sys_hz() garante que a conta está certa em todas as freq

  int ipf = ((int) sys_hz()) / fr_rate;

  int mov_x = 0, mov_y = 0;

  bool negative_mov = false; // if the pos is smaller than the current one (xf<xi || yf<yi)

  if(xi!=xf && yi!=yf){return 1;}

  if (speed < 0) {
    ipf = ipf * (-speed);
    if (xi == xf) {
      mov_y = 1;
    }
    // movimento horizontal
    else {
      mov_x = 1;
    }
  }

  else{
    if (xi == xf) {
      mov_y = speed;
    }
    else {
      mov_x = speed;
    }
  }
  

  // verificar o sentido do movimento
  if (xf < xi) {
    negative_mov = true;
    mov_x = -mov_x;
  }
  if (yf < yi) {
    negative_mov = true;
    mov_y = -mov_y;
  }

  // inicializar com o modo 0x105
  if (vg_initialize(0x105)) {
    return 1;
  }

  xpm_image_t img;

  xpm_load(xpm, XPM_INDEXED, &img);

  // desenha a primeira imagem
  if (draw_xpm(xi, yi, img))
    return 1;

  uint8_t mouse_bit_no;

  if (timer_subscribe_int(&mouse_bit_no))
    return 1;

  uint8_t kbd_bit_no;

  if (kbd_subscribe_int(&kbd_bit_no))
    return 1;

  int ipc_status, r;
  message msg;
  int size = 1;
  uint8_t bytes[2];

  bool moving = true;

  uint16_t prev_x, prev_y;

  while (scancode != ESC_BREAK) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
            kbc_ih();
            if (twoBytes(scancode)) {
              size = 2;
              continue;
            }
            if (size == 1) {
              bytes[0] = scancode;
            }
            else {
              bytes[0] = 0xE0;
              bytes[1] = scancode;
            }
            size = 1;
          }
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
            timer_int_handler();
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {
    }
    if (moving) {
      if (timer_counter >= ipf) {

        printf("timer_counter: %d\n", timer_counter);

        prev_x = xi;
        prev_y = yi;
        xi += mov_x;
        yi += mov_y;

        // ver se é o último movimento a fazer
        if ((xi >= xf && yi >= yf && !negative_mov) || (xf >= xi && yf >= yi && negative_mov)) {
          if (draw_next_xpm(xpm, prev_x, prev_y, xf, yf)) {
            return 1;
          }
          moving = false;
        }
        else {
          if (draw_next_xpm(xpm, prev_x, prev_y, xi, yi)) {
            return 1;
          }
        }

        timer_counter = 0;
      }
    }
  }

  if (vg_exit())
    return 1;

  if (kbd_unsubscribe_int())
    return 1;

  if (timer_unsubscribe_int())
    return 1;

  return 0;
}
