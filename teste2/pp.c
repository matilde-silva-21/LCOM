

#include <stdbool.h>
#include <stdint.h>
#include <string.h>


uint8_t *video_map_phys(uint32_t ph_addr, size_t len); //fornecida

int lpv_get_mode_info(uint16_t mode, lpv_mode_info_t * lmi_p); //fornecida

int lpv_set_mode(uint16_t mode); //fornecida

int drawPixel(uint16_t x, uint16_t y, uint32_t color){
    char* aux = video_mem;
    aux = aux + (y*h_res + x);
    memcpy(aux, &color, sizeof(color));
}

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static uint8_t h_res;	        /* Horizontal resolution in pixels */
static uint8_t v_res;	        /* Vertical resolution in pixels */
static uint8_t bits_per_pixel, r_sz, r_pos, g_sz, g_pos, b_sz, b_pos;


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


int pp_test_line(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay){


    static void *video_mem; /* frame-buffer VM address (static global variable*/ 
    

    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
    int r;				    

    lpv_mode_info_t lmi_p;

    if(lpv_get_mode_info(mode, &lmi_p)!=0) {return 1;}
    
    h_res = lmi_p.x_res;
    v_res = lmi_p.y_res;
    bits_per_pixel = lmi_p.bpp;

    vram_size = h_res * v_res *(bits_per_pixel/8);
    vram_base = lmi_p.phys_addr;

    r_sz = lmi_p.r_sz;
    g_sz = lmi_p.g_sz;
    b_sz = lmi_p.b_sz;
    r_pos = lmi_p.r_pos;
    g_pos = lmi_p.g_pos;
    b_pos = lmi_p.b_pos;

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");


    if(lpv_set_mode(mode)!=0) return 1;

    int d;

    switch (dir)
    {
    case lpv_hor:
        for(int i=x; i<(len+x); i++){

            drawPixel(i, y, color);
        }

        break;

    case lpv_vert:
        for(int i=y; i<(len+y); i++){

            drawPixel(x, i, color);
        }
        break;
    
    default:
        return 1;
    }

    sleep(delay);

    lpv_set_mode(0);

    return 0;





}



