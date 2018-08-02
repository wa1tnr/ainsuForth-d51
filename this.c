#include "atmel_start.h"

void blink(void) {
    PORT->Group[1].OUTTGL.reg = (uint32_t)(1 << 23); // PB23 ON
}

void LED_on(void) {
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PB23 ON
}

void LED_off(void) {
    PORT->Group[0].OUTSET.reg |= (uint32_t)(0 << 23); // PB23 OFF
}

void LED_output(void) {
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 23); // PB23 OUTPUT
}

#define RESET_COUNT 5

int main(void) {
    int icount = 5;
    SystemInit();
    while (1) {
        if (icount == 1) {
            icount = RESET_COUNT;
        }
        icount--;
    }
    blink(); // toggle LED on or off
    while(1);
}
