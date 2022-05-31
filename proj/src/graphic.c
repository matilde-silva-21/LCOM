#include "graphic.h"

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

int vg_set_mode(uint16_t *mode) {
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

  if (!(aux.ah == 0x00 && aux.al == 0x4F))
    return 1;

  return 0;
}


xpm_image_t (loadXpm)(xpm_map_t xpm) {
    xpm_image_t img;

    xpm_load(xpm, XPM_8_8_8_8, &img);

    return img;
}

void drawPixel(uint16_t x, uint16_t y, uint32_t color){
    uint8_t *aux = video_mem;
    uint32_t n = (bitsPerPixel + 7) >> 3; //arredonda para cima o nº de bytes na cor - tipo o ceil
    aux = aux + n *(y* Xres + x); // calcula onde colocar a cor
    memcpy(aux, &color, n); //copia n bytes da color para aux
}

int drawXpm(uint16_t x, uint16_t y, xpm_image_t img){
    int counter=0;

    for(int i=y; i<img.height+y; i++){
        for(int j=x; j<img.width+x; j++){
            drawPixel(j,i, img.bytes[counter++]);
        }
    }

    return 0;
}
