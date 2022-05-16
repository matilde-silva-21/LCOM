#include "vg.h"

static uint16_t Xres; // horizontal resolution
static uint16_t Yres; // vertical resolution
static int bitsPerPixel;

static void *video_mem;

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
  /*
  if (mode != 0x105 && mode != 0x110 && mode != 0x115 && mode != 0x11A && mode != 0x14C) {
    return 1;
  }*/

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
/*
  if (!(aux.ah == 0x00 && aux.al == 0x4F))
    return 1;
*/

  return 0;
}

void vg_drawpixel(uint16_t x, uint16_t y, uint32_t color){
  uint8_t *aux = video_mem;
  uint32_t n = (bitsPerPixel + 7) / 8; //not sure por que e que se faz isto - numero de bytes na cor i think
  aux = aux + n *(y* Xres + x); // calcula onde colocar a cor

  for(uint32_t i = 0; i < n; i++){
    *(aux++) = (uint8_t) color;
    color >>= 8;
  }

  //memcpy(aux, &color, n); //copia n bytes da color para aux
}

int vg_drawline(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(int i = 0; i < len; i++){
    vg_drawpixel(x, y, color);
    x++;
  }
  return 0;
}
