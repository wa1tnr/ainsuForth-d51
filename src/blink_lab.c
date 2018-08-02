#include "atmel_start.h"
#include "src/vocab.h"

void LED_output(void) {
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 23); // PA23 OUTPUT
}

void LED_on(void) {
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 SET
}

void LED_off(void) {
    PORT->Group[0].OUTCLR.reg |= (uint32_t)(1 << 23); // PA23 CLR
}

void LED_toggle(void) { // hasn't worked yet iirc
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // PA23 TOGGLE ON/OFF
}

void ldelay(void) { // homebrew
    for (volatile int i = 2100; i>0; i--) {
        for (volatile int j = 420; j>0; j--) {
        }
    }
}

#define RESET_COUNT 5

void blink_toggled(void) {
    LED_toggle();
    ldelay();
    LED_toggle();
    ldelay();
}

void blink(void) {
    LED_on();
    ldelay();
    LED_off();
    ldelay();
}

/*
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // PA23 TOGGLE ON/OFF
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 ON
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 ON
*/

void say_in_toggles(void) {
    blink_toggled();
    ldelay();
    ldelay();
    ldelay();
    blink_toggled();
    ldelay();
    ldelay();
    ldelay();
    blink_toggled();
    ldelay();
    ldelay();
    ldelay();
    blink_toggled();
    ldelay();
    ldelay();
    ldelay();
    blink_toggled();
    ldelay();
    ldelay();
    ldelay();
    ldelay();
    ldelay();
    ldelay();
    ldelay();
}

void say_in_blinks(void) {
    blink();
    blink();
    blink();
    blink();
    blink();
    blink();
    blink();
}

void blink_lab(void) { // not integrated to a standalone file just yet.
    int icount = 5;
    // SystemInit();
    LED_output();
    say_in_toggles();
    say_in_blinks();
    caution_lamp();
    while (1);
    while (1) {
        if (icount == 1) {
            icount = RESET_COUNT;
        }
        icount--;
    // blink(); // toggle LED on or off
    // ldelay();
    }
    while(1);
}
