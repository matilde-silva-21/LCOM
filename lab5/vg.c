#include "vg.h"

static uint16_t Xres; // horizontal resolution
static uint16_t Yres; // vertical resolution
static int bitsPerPixel;

static uint8_t color_mode;
static uint8_t redMask;
static uint8_t redFieldPos;
static uint8_t greenMask;
static uint8_t greenFieldPos;
static uint8_t blueMask;
static uint8_t blueFieldPos;

static void *video_mem;

extern int kbd_hookid;//hookid, timer_counter;
extern uint8_t scancode, statuscode;

int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t* info){

  struct minix_mem_range mr;
  int r;

  if(vbe_get_mode_info(*mode, info)){
    return 1;
  }

  //atribuir os valores obtidos às variaveis
  Xres = info->XResolution;
  Yres = info->YResolution;
  bitsPerPixel = info->BitsPerPixel;

  //color info - para o draw pattern
  redMask = info->RedMaskSize;
  redFieldPos = info->RedFieldPosition;

  greenMask = info->GreenMaskSize;
  greenFieldPos = info->GreenFieldPosition;

  blueMask = info->BlueMaskSize;
  blueFieldPos = info->BlueFieldPosition;

  color_mode = info->MemoryModel;

  unsigned int vram_base, vram_size;
  vram_base = info->PhysBasePtr;
  vram_size = Xres * Yres * (bitsPerPixel/8); // para ficar em bytes

  //mapear memória
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  //permissions
  if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

int vg_setmode(uint16_t *mode) {
  //nao sei se isto é necessário
  if (*mode != 0x105 && *mode != 0x110 && *mode != 0x115 && *mode != 0x11A && *mode != 0x14C) {
    return 1;
  }

  if(mode == NULL)
    return 1;

  reg86_t aux;
  memset(&aux, 0, sizeof(aux));

  aux.intno = 0x10;
  aux.ah = 0x4F;
  aux.al = 0x02; // function 0x02 = set VBE mode
  aux.bx = BIT(14) | *mode;

  /* Make the BIOS call */
  if (sys_int86(&aux) != OK) {
    printf("\tvideo_test_init(): sys_int86() failed \n");
    return 1;
  }

  //tambem acho que nao é preciso esta parte
  if (!(aux.ah == 0x00 && aux.al == 0x4F))
    return 1;

  return 0;
}

void vg_drawpixel(uint16_t x, uint16_t y, uint32_t color){
  uint8_t *aux = video_mem;
  uint32_t n = (bitsPerPixel + 7) >> 3; //arredonda para cima o nº de bytes na cor - tipo o ceil
  aux = aux + n *(y* Xres + x); // calcula onde colocar a cor
  memcpy(aux, &color, n); //copia n bytes da color para aux
}

int vg_drawline(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(int i = 0; i < len && (i + x) < Xres; i++){
    vg_drawpixel(x, y, color);
    x++;//passa para o proximo pixel
  }
  return 0;
}

int wait_esckey(){

  uint8_t kbd_bit_no = BIT(kbd_hookid);

  if (kbd_subscribe_int(&kbd_bit_no))
    return 1;

  int ipc_status, r;
  message msg;
  int size = 1;
  uint8_t bytes[2];

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
            } else {
              bytes[0] = 0xE0;
              bytes[1] = scancode;
            }
            //kbd_print_scancode(makecode(scancode), size, bytes);
            size = 1;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    } else {
    }
  }

  if (kbd_unsubscribe_int())
    return 1;

  return 0;
}

int vg_drawrectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(uint16_t i = 0; i < height; i++){
    vg_drawline(x, y, width, color);
    y++;
  }
  return 0;
}

int vg_drawpattern(uint8_t no_rectangles, uint32_t first, uint8_t step){

  uint16_t width = Xres / no_rectangles;
  uint16_t height = Yres / no_rectangles;

  uint32_t color;

  if(color_mode == INDEXED_MODE){
    for (size_t row = 0; row < no_rectangles; row++) {
      for (size_t col = 0; col < no_rectangles; col++) {
        color = (first + (row * no_rectangles + col) * step) % (1 << bitsPerPixel); //calcula a cor
        vg_drawrectangle(col*width, row*height, width, height, color); //col*widht e row*height é o ponto inicial do retangulo
      }
    }
  }
  else if(color_mode == DIRECT_MODE){
    uint8_t redFirst = (first & redMask) >> redFieldPos;
    uint8_t greenFirst = (first & greenMask) >> greenFieldPos;
    uint8_t blueFirst = (first & blueMask) >> blueFieldPos;
    uint16_t red, green, blue;
    for (size_t row = 0; row < no_rectangles; row++) {
      for (size_t col = 0; col < no_rectangles; col++) {

        red = (redFirst + col * step) % (1 << redMask);
        green = (greenFirst + row * step) % (1 << greenMask);
        blue = (blueFirst + (col + row) * step) % (1 << blueMask);

        color = 0 | (red << redFieldPos) | (green << greenFieldPos) | (blue << blueFieldPos);

        vg_drawrectangle(col*width, row*height, width, height, color);
      }
    }
  }

  else{
    return 1;
  }
  return 0;
}

int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img){ 

  int counter=0;

  for(int i=y; i<img.height+y; i++){
    for(int j=x; j<img.width+x; j++){
      vg_drawpixel(j,i, img.bytes[counter++]);
    }
  }
  

  return 0;
}

