// Mon Aug  6 06:11:27 UTC 2018

// main.c

#include "atmel_start.h"
#include "gpio_local.h"

void pins_setup(void) {

    // serial pins
    //    input
    PORT->Group[1].DIRCLR.reg = (uint32_t)(1 << 17); // rx is input // PB17
    //    output
    PORT->Group[1].DIRSET.reg = (uint32_t)(1 << 16); // tx output   // PB16

    // gpio outputs
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 18); // PA18 //  1 18 pinmode  // D6
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 23); // PA23 //  1 23 pinmode  // D13

    // gpio raise pin to 3.3v
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 18); // PA18 //  1 18 pinwrite
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 //  1 23 pinwrite

    // gpio lower pin to Ground
    PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 18); // PA18 //  0 18 pinwrite
    PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 23); // PA23 //  0 23 pinwrite

    // gpio input pins
    PORT->Group[0].DIRCLR.reg  = (uint32_t)(1 << 21); // PA21 //  0 21 pinmode  // D11

    // gpio output pin toggles
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // D6 toggle
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // D13 toggle
}



void uSec(void) {
    for (volatile int i = 1; i < 2; i++) { // needs calibration
        // nothing
    }
}


void short_timer(void) { // human blinkie timescale
    uint32_t on_time  = 2140111222; // it's 2147 something ;)
    for(int j = 2; j>0; j--) {
        for(on_time =       2140112; on_time > 0; on_time--) { // 21.4 million
            uSec();
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

void nmain(void) {
        raise_LED_pins();
	while (1) {
            flicker_LED();
            short_timer();
	}
}

void SysTick_Handler(void){
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // D6 toggle
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // D13 toggle
}

int main(void) {
    SystemInit();
    // init_act_LED();
    pins_setup();
    clock_init();
    SysTick_Config(4000);
    nmain();
    while (1) {
        // none
    }
}
