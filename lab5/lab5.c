#include <lcom/lab5.h>
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

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

  printf("start\n");

  if (mode != 0x105 && mode != 0x110 && mode != 0x115 && mode != 0x11A && mode != 0x14C) {
    return 1;
  }

  reg86_t aux;
  memset(&aux, 0, sizeof(aux));

  aux.intno = 0x10;
  aux.ah = 0x4F;
  aux.al = 0x02; // function 0x02 = set VBE mode

  /* Make the BIOS call */
  if (sys_int86(&aux) != OK) {
    printf("\tvideo_test_init(): sys_int86() failed \n");
    return 1;
  }

  if (!(aux.ah == 0x00 && aux.al == 0x4F))
    return 1;

  aux.bx = BIT(14) | mode;

  sleep(delay);
  if (vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

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

