// blinker.c ainsuForth D51

#include "atmel_start.h"
#include "gpio_local.h"

/* Many changes: by wa1tnr, July 2018 */

void nSecC(void) { // 107 ns
    for (volatile int i = 1; i < 2; i++) {
    }
}

#define CAL_IVAL 71 // calibration interval

void uSec(void) { // experimental
    for (volatile int i = CAL_IVAL; i > 0; i--) {
    }
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // blink  D6 / PA18
}

void measured_timer(void) {
    for (int i=100000; i>0; i--) { // 100k iterations
        uSec();
    }
}

/*
void xcalibrate_usec(void) { // endless loop
    while(-1) {
        measured_timer();
    }
}
*/


void calibrate_usec(void) { // endless loop
    // lower_D13_feather();
    for (volatile int i=50000; i>0; i--) { // off by a factor of 10x
        uSec();
    }
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
}


void short_timer(void) {
    uint32_t on_time  = 2140111222; // it's 2147 something ;)
    for(int j = 2; j>0; j--) {
        for(on_time =       2140112; on_time > 0; on_time--) { // 21.4 million
            nSecC();
        }
    }
}

void raise_LED_pins(void) {
    raise_D13_feather();
}

void lower_LED_pins(void) {
    lower_D13_feather();
}

void blink_LED(void) {
    raise_LED_pins();
    short_timer();
    lower_LED_pins();
    short_timer();
}

void flicker_LED(void) {
    raise_LED_pins();
    short_timer();
    lower_LED_pins();
    short_timer();
}

void blinkLEDfast(void) {
    for(int i = 15; i > 0; i--) {
        blink_LED();
    }
}

void activity_LED_demo(void) {
    blinkLEDfast();
}

void long_long_timer(void) {
    for (int j=4; j>1; j--){
        for (int i=3; i>1; i--){
            short_timer();
        }
    }
}

// void calibrate_usec(void) { // endless loop
