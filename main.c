// main.c ainsuForth D51

// has clock_init() but nothing for the uart - 04:44 UTC 04 AUG 2018

#include "atmel_start.h"
#include "clock_init.h"
#include "led_pulse.h"

/* Many changes: by wa1tnr, July 2018 */

int main(void) {
    SystemInit();
    clock_init();
    led_pulse();
    while (1) {
        // none
    }
}
