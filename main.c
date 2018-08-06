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

int tick_h  = -1; // true
int tick_ch = -1; // true

void SysTick_Handler(void){
    int tick_v = tick_h;

    if (tick_v == -1) {
        tick_h = 0;
    } else {
        tick_h = -1;
    }
/*
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // D6 toggle
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // D13 toggle
*/

}

void say_a_tick(void) {
    if (tick_h == -1) {
        // gpio lower pin to Ground
        PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 18); // PA18 //  0 18 pinwrite
        PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 23); // PA23 //  0 23 pinwrite
    } else {
        // gpio raise pin to 3.3v
        PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 18); // PA18 //  1 18 pinwrite
        PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 //  1 23 pinwrite
    }
}

int tick_r, tick_c = -1;

void _ch_delay(void) {
    // for(int i=2400; i>0; i--) {
    for(int i=9600; i>0; i--) {
        uSec();
    }
}

bool tick_changed(void) {
    tick_r = tick_h;
    _ch_delay(); // wait a bit
    tick_c = tick_h;
    if (tick_c == tick_r) { // nothing's changed
    } else {
        return true; // escape
    }
    return true;
}

void hold_for_tick_change(void) {
    bool result = true;
    bool interim = false;
    while (result) {
        interim = tick_changed(); // false becomes true when changed
        result = ! interim;
    }
}

void deep_quiet(void) {
    for (int i=50;i>0;i--) {
        hold_for_tick_change();
    }
}

void _one_pulse(void) {
    say_a_tick();
    deep_quiet();
    deep_quiet();
}

void nooop(void) {
}

void nmain(void) {
    // while(1) { say_a_tick(); }
    while(1)    { _one_pulse(); }
        // hold_for_tick_change();

    while(1) { }
    while(1) {
        for (int i = 8; i > 0 ; i--) {
            if (i == 8) { nooop(); }
/*
            if (i == 6) { _one_pulse(); }
*/
        }
        for (int j = 12; j > 0; j--) { hold_for_tick_change(); }
    }

/*
    while(1) {
        hold_for_tick_change();
        say_a_tick();
    }
*/
    while(1) {
    }

    while(1) {
        // _one_pulse();
    }

    raise_LED_pins();
    while (1) {
        flicker_LED();
        short_timer();
    }
}

int main(void) {
    SystemInit();
    // init_act_LED();
    pins_setup();
    clock_init();
    // SysTick_Config(5500); // 19200 baud has    52   uSec pulses

    SysTick_Config(5000000); // 19200 baud has    52   uSec pulses

    nmain();
    while (1) {
        // none
    }
}
