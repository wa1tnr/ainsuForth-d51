// led_pulse.c - show human operator the clock is operating

// was: main.c ainsuForth D51
// has clock_init() but nothing for the uart - 04:44 UTC 04 AUG 2018

#include "gpio_local.h"
#include "blinker.h"

void led_main(void) {
    raise_LED_pins();
    while (1) {
        flicker_LED();
        short_timer();
    }
}

void led_pulse(void) {
    init_act_LED();
    led_main();
    while (1) {
        // none
    }
}
