// main.c ainsuForth D51

#include "atmel_start.h"
#include "gpio_local.h"
#include "uartport.h"
#include "hardware.h"
#include "blinker.h"
#include "clock_init.h"

/* Many changes: by wa1tnr, July 2018 */

/*
void uSec(void) { }
void short_timer(void) { } } }
void raise_LED_pins(void) { } 
void lower_LED_pins(void) { } 
void blink_LED(void) { }
void flicker_LED(void) { }
void blinkLEDfast(void) { } }
void activity_LED_demo(void) { }
void long_long_timer(void) { }

*/
#ifdef NEVER_EEE
void xclock_init(void){ // Jake Read
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
#endif // #ifdef NEVER_EEE

void nmain(void) {
        raise_LED_pins();
	while (1) {
            flicker_LED();
            short_timer();
	}
}

int main(void) {
    SystemInit();
    clock_init();
    init_act_LED();
    nmain();
    while (1) {
        // none
    }
}
