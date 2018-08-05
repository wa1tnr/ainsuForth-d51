// main.c ainsuForth D51

// has clock_init() but nothing for the uart - 04:44 UTC 04 AUG 2018

#include "sam.h"
#include "atmel_start.h"
#include "clock_init.h"
#include "led_pulse.h"
// #include "uart_setup.h"
#include "gpio_local.h"
#include "jake_uart.h"
#include "blinker.h"

extern void jake_uart(void); // no .h file haha

/* Many changes: by wa1tnr, July 2018 */

int main(void) {
    uint8_t value = 0;
    SystemInit();
    clock_init();
    init_act_LED(); // omit until further down -- test only for calibrate_usec()
    lower_D13_feather();


    raise_D13_feather();
        short_timer();
    lower_D13_feather();
        short_timer();

    raise_D13_feather();
        short_timer();
    lower_D13_feather();
        short_timer();


    for (int j=3;j>0;j--) {
        short_timer();
        short_timer();
    }

        calibrate_usec(); // endless loop - testing only -- omit when running the usual program
        calibrate_usec(); // endless loop - testing only -- omit when running the usual program
        calibrate_usec(); // endless loop - testing only -- omit when running the usual program
        calibrate_usec(); // endless loop - testing only -- omit when running the usual program
        calibrate_usec();

    lower_D13_feather();
        short_timer();
        short_timer();
        short_timer();


    raise_D13_feather();
        short_timer();
    lower_D13_feather();
        short_timer();

    raise_D13_feather();
        short_timer();
    lower_D13_feather();
        short_timer();

    while(-1) {
    }
    while(-1); // trap for safety

    // SysTick_Config(5000000); //  five million

    // SysTick_Config(5000000); //  9.5      milliseconds

    // 2.5 million:
    // SysTick_Config(2500000); //  4.75     milliseconds - a faster tick

    // 1.25 million:
    // SysTick_Config(1250000); //  2.375    milliseconds

    // 625 thousand:
    // SysTick_Config(625000); / /  1.2      milliseconds

    // thats 24/50 on a 2.5 ms time base 
    //     (2.5 * (24/50))   24 50 / 2.5 *  = 1.2
    //     24 50 / is 0.48  and it's a 48 MHz clock

    // 312.5 thousand:
    //  SysTick_Config(312500); //  0.595     milliseconds (measured!)

    //  SysTick_Config(275000); //  a bit slow for 0.5 ms target

        SysTick_Config(265000); //  ____ survey says

    //  SysTick_Config(260000); //  very slightly too fast

    //  SysTick_Config(250000); //  0.475     milliseconds (measured)  23.75/50 on 1 ms scale
    init_act_LED();
    gpio_set_pin_level(PA23, true);
    gpio_set_pin_level(PA23, false);
    // while(1);
    jake_uart();
    // THIS // while(1);

    gpio_set_pin_level(PA23, true); short_timer(); gpio_set_pin_level(PA23, false); short_timer();
    gpio_set_pin_level(PA23, true); short_timer(); gpio_set_pin_level(PA23, false); short_timer();
    gpio_set_pin_level(PA23, true); short_timer(); gpio_set_pin_level(PA23, false); short_timer();

    while (1) { // 21400 is a good starter magnitude for the empty count loop
        // for (volatile int i = 211 ; i > 0; i--) { // 210 gives a 10 uSec scaling
        // for (volatile int i = 21 ; i > 0; i--) { // 21 gives a 1 uSec scaling

#undef NO_LOOP_HERE
#define NO_LOOP_HERE
#ifndef NO_LOOP_HERE
        for (volatile int i = 8 ; i > 0; i--) { // 8 gives a ?      scaling
            // A scale factor of 8 here gives a nice 500 nSec time base
            // vertical is 2.00 volts probe is X1
        }
            // 50 nanoseconds scale is the limit and the pulse looks awful
            // could be a 23 nanosecond pulse 360 degrees phase transition.
            // 48 MHz = 20.833333333333 ns
            // https://www.unitjuggler.com/convert-frequency-from-MHz-to-ns(p).html?val=48

            // this looks like good evidence for a 48 MHz base clock frequency.
#endif // #ifndef NO_LOOP_HERE

        // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // blink  D6 / PA18
        // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
    }

    // --------------- ignored ------------------
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
