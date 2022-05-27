// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#define BIT(n) (1 << (n))


// Any header files included below this line should have been created by you

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_get_mode(uint16_t *mode){

    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
    int r;				    

    vbe_mode_info_t info;
    /* Use VBE function 0x01 to initialize vram_base and vram_size */
    
    if(vbe_get_mode_info(*mode, &info)!=0){return 1;}

    h_res = info.XResolution;
    v_res = info.YResolution;
    bits_per_pixel = info.BitsPerPixel;

    vram_base = info.PhysBasePtr;
    vram_size = h_res * v_res * (bits_per_pixel/8); //dividir por oito para ficar em bytes

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

    return 0;

}

int vg_set_mode(uint16_t *mode){

    if(*mode!=0x105 && *mode !=0x110 && *mode!=0x115 && *mode!=0x11A && *mode!=0x14C){return 1;}
    if(mode == NULL){return 1;}

    reg86_t r86;
   
    /* Specify the appropriate register values */
    
    memset(&r86, 0, sizeof(r86));	/* zero the structure */

    r86.intno = 0x10; /* BIOS video services */
    r86.ah = 0x4F;    /* Set Video Mode function */
    r86.al = 0x02;    /* 80x25 text mode */
    r86.bx = BIT(14) | *mode;
    
    /* Make the BIOS call */

    if( sys_int86(&r86) != OK ) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;

    if(r86.al != 0x4F || r86.ah != 0x00) {return 1;}

    return 0;

}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color){

  char *aux = video_mem;
  aux = aux + (y*h_res + x);

  memcpy(aux, &color, sizeof(color));
  
  return 0;


}

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
  
  if(vg_get_mode(&mode)!=0){return 1;}

  if(vg_set_mode(&mode)!=0){return 1;}

  sleep(delay);

  vg_exit();

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  if(vg_get_mode(&mode)!=0){return 1;}

  if(vg_set_mode(&mode)!=0){return 1;}

  for(int i=y; i<height+y; i++){
    for(int j=x; j<width+x; j++){
      vg_draw_pixel(j,i, color);
    }
  }
  
  sleep(4);

  vg_exit();
  
  return 0;

}



int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  uint16_t mode = 0x105;
  
  if(vg_get_mode(&mode)!=0){return 1;}

  if(vg_set_mode(&mode)!=0){return 1;}

  xpm_image_t img;

  xpm_load(xpm, XPM_INDEXED , &img);
  

  int counter=0;

  for(int i=y; i<img.height+y; i++){
    for(int j=x; j<img.width+x; j++){
      vg_draw_pixel(j,i, img.bytes[counter++]);
    }
  }
  
  sleep(4);

  vg_exit();
  
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}


