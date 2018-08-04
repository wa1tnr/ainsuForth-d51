// main.c ainsuForth D51

// has clock_init() but nothing for the uart - 04:44 UTC 04 AUG 2018

#include "sam.h"
#include "atmel_start.h"
#include "clock_init.h"
#include "led_pulse.h"
#include "uart_setup.h"
#include "gpio_local.h"
#include "jake_uart.h"
#include "blinker.h"

extern void jake_uart(void); // no .h file haha

/* Many changes: by wa1tnr, July 2018 */

int main(void) {
    uint8_t value = 0;
    SystemInit();
    clock_init();
    SysTick_Config(5000000);
    init_act_LED();
    gpio_set_pin_level(PA23, true);
    gpio_set_pin_level(PA23, false);
    // while(1);
    jake_uart();
    // THIS // while(1);

    gpio_set_pin_level(PA23, true); short_timer(); gpio_set_pin_level(PA23, false); short_timer();
    gpio_set_pin_level(PA23, true); short_timer(); gpio_set_pin_level(PA23, false); short_timer();
    gpio_set_pin_level(PA23, true); short_timer(); gpio_set_pin_level(PA23, false); short_timer();

    // led_pulse();
    while (1) {
        value = byte_seen * 2; // arbitrary scale
        // delay_ms(value); // a form of output - a delay
        for(uint8_t i = value; i > 0; i--) {
            PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
            short_timer();
        } // toggle port pin on and off to represent the byte seen
    }
}
