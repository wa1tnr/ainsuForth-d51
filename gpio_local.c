// #include "driver_init.h" // gives atmel_start_pins.h for D13 and such
#include "atmel_start_pins.h"

void init_act_LED(void) {
    gpio_set_pin_direction(PA23, GPIO_DIRECTION_OUT);

    // No longer supporting Metro M4 Express directly.

    // gpio_set_pin_direction(PA16, GPIO_DIRECTION_OUT);
}

/*

void raise_D13_metro(void) { // Metro M4 Express
    gpio_set_pin_level(PA16, true);
}
*/

void raise_D13_feather(void) { // Feather M4 Express
    gpio_set_pin_level(PA23, true);
}

/*
void lower_D13_metro(void) {
    gpio_set_pin_level(PA16, false);
}
*/

void lower_D13_feather(void) {
    gpio_set_pin_level(PA23, false);
}
