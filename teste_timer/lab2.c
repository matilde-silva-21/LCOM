

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)

  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int subscribe_timer(uint8_t &timer_hook_id){
    if(sys_irqsetpolicy(10, IRQ_REENABLE, timer_hook_id)!=0) return 1;

    return 0;
}

int unsubscribe_timer(uint8_t &timer_hook_id){
    if(sys_irqrmpolicy(timer_hook_id)!=0) return 1;

    return 0; 
}


int pp_test_alarm(int timer, int interval, enum l3223_time_units unit){
    
    //esta parte estou a criar a control word para a escrever no control register 

    uint8_t control_word = BIT(4), TIMER_PORT, timer_hook_id = 0x00, st_reg; //ativar modo alarm
    switch (timer)
    {
    case 0:
        TIMER_PORT = 0x20;
        break;
    case 1:
        TIMER_PORT = 0x21;
        control_word |= BIT(6);
        break;
    case 2:
        TIMER_PORT = 0x22;
        control_word |= BIT(7);
        break;
    default:
        return 1;
    }

    switch (unit)
    {
    case 0:
        break;
    case 1:
        control_word |= BIT(0);
        break;
    case 2: 
        control_word |= BIT(1);
        break;
    default:
        return 1;
    }

    if(sys_outb(0x23, &control_word)!=0) return 1;


    //esta parte estou a fazer a escrever o periodo no timer pretendido

    if(sys_outb(TIMER_PORT, util_get_LSB(interval))!=0) return 1;
    if(sys_outb(TIMER_PORT, util_get_MSB(interval))!=0) return 1;

    if(subscribe_timer(timer_hook_id)!=0) return 1;

    uint8_t irq_set = BIT(timer_hook_id);

    while (true)
    {
        //quando recebo o interrupt pode nao ser do timer em destaque por isso tenho de verificar primeiro

        if(msg & irq_set){
            if(util_sys_inb(0x43, &st_reg)) return 1;
            uint8_t val = st_reg & BIT(timer);
            if(val >> timer){if(pp_print_alarm(timer, interval, unit)) return 1;}
            break;
        }
    }

    if(unsubscribe_timer(timer_hook_id)!=0) return 1;

    return 0;

}

int util_sys_inb (uint8_t port, uint8_t *st){
    uint32_t *copy = st;
    if(sys_inb(port, copy)!=0){return 1;}
    *st = (uint8_t) copy;
    return 0; 
}

uint8_t util_get_LSB(int val){
    return (uint8_t) val;
}

uint8_t util_get_MSB(int val){
    uint16_t copy = (uint16_t) val;
    return (uint8_t)(val >> 8);
}

