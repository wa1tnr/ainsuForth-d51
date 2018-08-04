// main.c ainsuForth D51

// has clock_init() but nothing for the uart - 04:44 UTC 04 AUG 2018

#include "atmel_start.h"
#include "gpio_local.h"
#include "uartport.h"
#include "hardware.h"
#include "blinker.h"
#include "clock_init.h"

/* Many changes: by wa1tnr, July 2018 */

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
