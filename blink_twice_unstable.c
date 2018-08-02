#include "atmel_start.h"

void LED_on(void) {
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 ON
}

void LED_off(void) {
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
}

void ldelay(void) { // homebrew
    for (volatile int i = 210022; i>0; i--) {
        for (volatile int j = 2; j>0; j--) {
        }
    }
}

void LED_output(void) {
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 23); // PA23 OUTPUT
}

#define RESET_COUNT 5

void blink(void) {
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 SET
    ldelay();
    PORT->Group[0].OUTCLR.reg |= (uint32_t)(1 << 23); // PA23 CLR
    ldelay();
    // PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
/*
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // PA23 TOGGLE ON/OFF
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 ON
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PA23 OFF
    ldelay();
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 ON
*/
}

int main(void) {
    int icount = 5;
    SystemInit();
    LED_output();
    blink();
    blink();
    blink();
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
