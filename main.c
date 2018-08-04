// main.c ainsuForth D51

#include "atmel_start.h"
#include "gpio_local.h"
#include "uartport.h"
#include "hardware.h"
#include "blinker.h"
#include "clock_init.h"

/* Many changes: by wa1tnr, July 2018 */

/*
void uSec(void) { }
void short_timer(void) { } } }
void raise_LED_pins(void) { } 
void lower_LED_pins(void) { } 
void blink_LED(void) { }
void flicker_LED(void) { }
void blinkLEDfast(void) { } }
void activity_LED_demo(void) { }
void long_long_timer(void) { }
void clock_init(void){ } // Jake Read

*/

void nmain(void) {
        raise_LED_pins();
	while (1) {
            flicker_LED();
            short_timer();
	}
}

int main(void) {
    SystemInit();
    clock_init();
    init_act_LED();
    nmain();
    while (1) {
        // none
    }
}
