// Thu Aug  2 06:27:20 UTC 2018
// Thu Aug  2 05:57:06 UTC 2018
// Wed Aug  1 04:18:10 UTC 2018

#include "atmel_start.h"
#include "gpio_local.h"
#include "uartport.h"
#include "hardware.h"

// int jclock =     0 ; // false
// int jclock =    -1 ; // true
// int jclock = 31722 ; // true
   int jclock =     0 ; // false

// zero is false.  -1 is true.  1 is also true.
// any non-zero value is true (partially verified)

/* Many changes: by wa1tnr, July 2018 */

void uSec(void) {
    for (volatile int i = 1; i < 2; i++) { // needs calibration
        // nothing
    }
}

void short_timer(void) { // human blinkie timescale
    uint32_t on_time  = 2140111222; // it's 2147 something ;)

    int rclock = 21401122; // 21.4 million

    if (jclock)  rclock = (rclock / 400); // high end of rapid but pleasant

    if (!jclock) rclock = (rclock /  45); // 25 a bit too sluggish to toggle states

    for(int j = 2; j>0; j--) {
        for(on_time =   rclock; on_time > 0; on_time--) {
            uSec();
        }
    }
}

void raise_LED_pins(void) { // multiple target boards
    raise_D13_feather(); // Feather M4 Express
}

void lower_LED_pins(void) {
    lower_D13_feather();
}

void blink_LED(void) {
    raise_LED_pins(); // Feather M4 or Metro M4
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

// clock clock who's there ///////////////////

#undef JAKES_CLOCK
#ifndef JAKES_CLOCK
void clock_init(void){ // Jake Read
    NVMCTRL->CTRLA.reg |= NVMCTRL_CTRLA_RWS(0);
    GCLK->CTRLA.bit.SWRST = 1;
    while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST);
    GCLK->GENCTRL[3].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL3);
    GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);

    OSCCTRL->DFLLCTRLA.reg = 0;
    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP(0x1) | OSCCTRL_DFLLMUL_FSTEP(0x1) | OSCCTRL_DFLLMUL_MUL(0);
    while(OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLMUL);
    OSCCTRL->DFLLCTRLB.reg = 0;
    while(OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLCTRLB);
    OSCCTRL->DFLLCTRLA.reg |= OSCCTRL_DFLLCTRLA_ENABLE;
    while(OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_ENABLE);
    OSCCTRL->DFLLCTRLB.reg = OSCCTRL_DFLLCTRLB_WAITLOCK | OSCCTRL_DFLLCTRLB_CCDIS | OSCCTRL_DFLLCTRLB_USBCRM;
    while(!OSCCTRL->STATUS.bit.DFLLRDY);

    GCLK->GENCTRL[5].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(24u);
    while(GCLK->SYNCBUSY.bit.GENCTRL5);

    GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL0].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK5_Val);
    OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0x00) | OSCCTRL_DPLLRATIO_LDR(59);
    while(OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.DPLLRATIO);
    OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
    OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;
    while(OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK == 0);

    GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DPLL0) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);

    MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;
}
#endif // #ifdef JAKES_CLOCK

void nmain(void) {
	while (1) {
            flicker_LED();
            short_timer();
	}
}

void caution_lamp(void) {
    // SystemInit();
    if (jclock) {
        clock_init();
    }
    init_act_LED();
    nmain();
    while (1) {
        // none
    }
}

// clock clock - - - - - - - - - - - - - - - -
// clock clock - - - - - - - - - - - - - - - -

// Jake Read  is the individual who published the crucial code in this file.

// Tue Jul 31 14:47:05 UTC 2018

// wa1tnr
//
// ATSAMD51J19A using Atmel Start
//  - working LED Blinkie D13 in under 2 kilobytes of .bin uploaded
// Feather M4 Express and
// Metro M4 Express

