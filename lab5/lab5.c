#include <lcom/lab5.h>
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "vg.h"

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

  //vai buscar a informação do mode e mapear a memória necessária
  vbe_mode_info_t info;
  if(vg_get_mode_info(&mode, &info)){
    return 1;
  }

  if(vg_setmode(&mode))
    return 1;

  sleep(delay);

  if (vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(uint16_t i = 0; i < height; i++){
    vg_drawline(x, y, width, color);
    y++;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  return 0;
}

int(video_test_controller)() {
  return 0;
}

