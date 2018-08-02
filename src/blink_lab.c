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

void bl_togl_Del(void) {
    blink_toggled();
    for (int i = 3; i > 0; i--) {
        ldelay();
    }
}

void say_in_toggles(void) {
    for (int i = 5; i > 0; i--) {
        bl_togl_Del();
    }
    for (int i = 7; i > 0; i--) {
        ldelay();
    }
}

void say_in_blinks(void) {
    for (int i = 3; i > 0; i--) {
        blink();
    }
}

void four_ldelay(void) {
    for (int i = 4; i > 0; i--) {
        ldelay();
    }
}

void blink_lab(void) { // not integrated to a standalone file just yet.
    int icount = 5;
    LED_output();
    say_in_toggles();

    four_ldelay();
    say_in_blinks();

    for (int i = 2; i > 0; i--) {
        four_ldelay();
    }

    say_in_blinks();

    for (int i = 4; i > 0; i--) {
        four_ldelay();
    }

    for (int j = 3; j > 0; j--) {
        if (icount == 1) {
            say_in_toggles(); four_ldelay(); four_ldelay(); say_in_blinks(); say_in_toggles(); four_ldelay(); four_ldelay(); say_in_blinks(); say_in_toggles(); four_ldelay(); four_ldelay(); say_in_blinks();
            for (int i = 4; i>0; i--) { four_ldelay(); }
            icount = RESET_COUNT;
            for (int i = 12; i>0; i--) { say_in_toggles(); four_ldelay(); four_ldelay(); }
        }
        icount--;
    }
    for (int i = 8; i>0; i--) { four_ldelay(); }
    for (int i = 3; i>0; i--) {
        say_in_toggles(); four_ldelay(); four_ldelay();
        say_in_blinks(); four_ldelay(); four_ldelay();
    }
    for (int i = 8; i>0; i--) { four_ldelay(); }
    for (int i = 8; i>0; i--) { four_ldelay(); }
        say_in_blinks(); four_ldelay(); four_ldelay();
        say_in_blinks(); four_ldelay(); four_ldelay();
        say_in_blinks(); four_ldelay(); four_ldelay();
    caution_lamp(); // a trap
    while (1); //  last trap /////
}
