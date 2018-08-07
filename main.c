// Tue Aug  7 06:43:03 UTC 2018
// has right curly brace

// Tue Aug  7 05:54:23 UTC 2018
// has mmmm

// Tue Aug  7 01:17:10 UTC 2018
// last good

// Mon Aug  6 06:11:27 UTC 2018

// Working serial program - sends a string of UUUUUU to the
// serial terminal running at 38400 bps

// main.c

#include "atmel_start.h"
#include "gpio_local.h"
#include "fstack.h" // forth-like interpreter, stack ops

int tick_h  = -1; // true
int tick_ch = -1; // true
int tick_r, tick_c = -1;

void pins_setup(void) {
// void push(int n);
// int pop(void);
    int pins_state = 1901; // arbitrary
    push(pins_state);

    // serial pins
    //    input
    PORT->Group[1].DIRCLR.reg = (uint32_t)(1 << 17); // rx is input // PB17
    //    output
    PORT->Group[1].DIRSET.reg = (uint32_t)(1 << 16); // tx output   // PB16

    // gpio outputs
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 18); // PA18 //  1  6 pinmode  // D6
    PORT->Group[0].DIRSET.reg |= (uint32_t)(1 << 23); // PA23 //  1 13 pinmode  // D13
    PORT->Group[0].DIRSET.reg  = (uint32_t)(1 << 21); // PA21 //  1 11 pinmode  // D11

    // gpio raise pin to 3.3v
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 18); // PA18 //  1  6 pinwrite // D6
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 21); // PA21 //  1 11 pinwrite // D11
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 //  1 13 pinwrite // D13

    // gpio lower pin to Ground
    PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 18); // PA18 //  0  6 pinwrite // D6
    PORT->Group[0].OUTCLR.reg |= (uint32_t)(1 << 21); // PA21 //  0 11 pinwrite // D11
    PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 23); // PA23 //  0 13 pinwrite // D13

#undef PIN_D11_INPUT
#ifdef PIN_D11_INPUT
    // gpio input pins
    PORT->Group[0].DIRCLR.reg  = (uint32_t)(1 << 21); // PA21 //  0 11 pinmode  // D11
#endif // #ifdef PIN_D11_INPUT

    // gpio output pin toggles
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // PA18 //  D6 toggle
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 21); // PA21 // D11 toggle
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // PA23 // D13 toggle
    // Group[1] means PORTB rather than PORTA (which is Group[0])
    PORT->Group[1].OUTTGL.reg = (uint32_t)(1 <<  3); // PB03 //  D8 toggle  // NEOPIX
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

void raise_D11(void) { // scope trigger
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 21); // PA21 //  1 11 pinwrite // D11
}

void lower_D11(void) { // ground it
    PORT->Group[0].OUTCLR.reg |= (uint32_t)(1 << 21); // PA21 //  0 11 pinwrite // D11
}

void pulse_D11(void) {
    lower_D11();
    for (int i=11; i>0; i--) {
        uSec();
    }
    raise_D11(); // negative logic
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

void SysTick_Handler(void){
    int tick_v = tick_h;

    if (tick_v == -1) {
        tick_h = 0;
    } else {
        tick_h = -1;
    }
}

void tx_to_ground(void) {
    // gpio lower pin to Ground
    PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 18); // PA18 //  0 18 pinwrite
    PORT->Group[0].OUTCLR.reg =  (uint32_t)(1 << 23); // PA23 //  0 23 pinwrite

    // Serial TX echo:
    PORT->Group[1].OUTCLR.reg =  (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
}

void tx_to_vcc(void) {
    // gpio raise pin to 3.3v
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 18); // PA18 //  1 18 pinwrite
    PORT->Group[0].OUTSET.reg |= (uint32_t)(1 << 23); // PA23 //  1 23 pinwrite

    // Serial TX echo:
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
}

void say_a_tick(void) {
    if (tick_h == -1) {
        tx_to_ground();
    } else {
        tx_to_vcc();
    }
}

void _ch_delay(void) {
    for(int i=2; i>0; i--) {
        uSec();
    }
}

bool tick_changed(void) {
    if (tick_c == tick_r) { // nothing's changed
        return false;
    } else {
        return true; // escape
    }
}

void hold_for_tick_change(void) {
    bool result = true;
    bool interim = false;
    tick_r = tick_h;
    while (result) {
        tick_c = tick_h;
        interim = tick_changed(); // false becomes true when changed
        _ch_delay();
        result = ! interim;
    }
}


// ///////////////////////////////////
// ///////////////////////////////////
// ///////////////////////////////////


// ///////////////////////////////////
// ///////////////////////////////////
// ///////////////////////////////////

void _one_gap(void) {
    tx_to_vcc();
}

void _two_gap(void) {
    tx_to_vcc();
    hold_for_tick_change();
    hold_for_tick_change();
}

void _one_pulse(void) {
    say_a_tick();
    hold_for_tick_change();
    say_a_tick();
    hold_for_tick_change();
}

void _gap_then_pulse(void) {
    _one_gap();
    _one_pulse();
}

void _gap_pulse_twice_iter(void) {
    for (int i = 2; i>0; i--) {
        _gap_then_pulse();
    }
}


void send_uuu(void) {
    tx_to_vcc();
    hold_for_tick_change();
    _one_gap();

    for (int i =  9; i > -1 ; i--) {
        if (i ==  9) { _one_gap();   }
        if (i ==  8) { _one_pulse(); }
        if (i ==  7) { _one_gap();   }
        if (i ==  6) { _one_pulse(); }

        if (i ==  5) { _one_gap();   }
        if (i ==  4) { _one_pulse(); } 
        if (i ==  3) { _two_gap();   }
        if (i ==  0) { _one_pulse(); }
        hold_for_tick_change();
    }
    _one_gap();
    hold_for_tick_change();
    tx_to_vcc();

    // very long time between typed 'U' characters:
    for (int j = 14899; j > 0; j--) { hold_for_tick_change(); }
}


void send_right_curly_brace(void) {
    tx_to_vcc();
    // very long time between typed ' ' characters:

    for (int j =  3899; j > 0; j--) { hold_for_tick_change(); }
    pulse_D11(); // scope trigger
    for (int j =   2; j > 0; j--) { hold_for_tick_change(); }

    // ---- new stanza

    tx_to_vcc();
    hold_for_tick_change();
    _one_gap();
    for (int i =  9; i > -1 ; i--) {
        if (i ==  9) { _one_gap();   }
        if (i ==  8) { _one_pulse(); }
        if (i ==  7) { _one_gap();   }

        if (i ==  6) { _one_pulse(); }
        if (i ==  5) { _one_pulse(); }
        if (i ==  4) { _one_gap(); 
        hold_for_tick_change();
        hold_for_tick_change();


  } //     if (i ==  4) { _one_pulse(); }
        if (i ==  3) { _one_pulse(); }
        if (i ==  2) { _one_pulse(); }
        if (i ==  1) { _one_gap();   }
        if (i ==  0) { _one_pulse(); }
        hold_for_tick_change();
    }
    _one_gap();
    hold_for_tick_change();
    tx_to_vcc();

    // very long time between typed 'U' characters:
    // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
    // pulse_D11(); // scope trigger
    // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
}


void send_m_m_m(void) {
    tx_to_vcc();
    // very long time between typed ' ' characters:

    for (int j =  3899; j > 0; j--) { hold_for_tick_change(); }
    pulse_D11(); // scope trigger
    for (int j =   2; j > 0; j--) { hold_for_tick_change(); }

    // ---- new stanza

    tx_to_vcc();
    hold_for_tick_change();
    _one_gap();
    for (int i =  9; i > -1 ; i--) {
        if (i ==  9) { _one_gap();   }
        if (i ==  8) { _one_pulse(); }
        if (i ==  7) { _one_gap();   }

        if (i ==  6) { _one_pulse(); }
        if (i ==  5) { _one_pulse(); }
        if (i ==  4) { _one_pulse(); }
        if (i ==  3) { _one_pulse(); }
        if (i ==  2) { _one_pulse(); }
        if (i ==  1) { _one_gap();   }
        if (i ==  0) { _one_pulse(); }
        hold_for_tick_change();
    }
    _one_gap();
    hold_for_tick_change();
    tx_to_vcc();

    // very long time between typed 'U' characters:
    // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
    // pulse_D11(); // scope trigger
    // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
}



void send_UUUU(void) {
    // send_uuu();
    tx_to_vcc();
    // very long time between typed 'U' characters:

    for (int j =  3899; j > 0; j--) { hold_for_tick_change(); }
    pulse_D11(); // scope trigger
    for (int j =   2; j > 0; j--) { hold_for_tick_change(); }

    // ---- new stanza

    tx_to_vcc();
    hold_for_tick_change();
    _one_gap();
    for (int i =  9; i > -1 ; i--) {
        if (i ==  9) { _one_gap();   }
        if (i ==  8) { _one_pulse(); }
        if (i ==  7) { _one_gap();   }
        if (i ==  6) { _one_pulse(); }
        if (i ==  5) { _one_gap();   }
        if (i ==  4) { _one_pulse(); } 
        if (i ==  3) { _one_gap();   }
        if (i ==  2) { _one_pulse(); }
        if (i ==  1) { _one_gap();   }
        if (i ==  0) { _one_pulse(); }
        hold_for_tick_change();
    }
    _one_gap();
    hold_for_tick_change();
    tx_to_vcc();

    // very long time between typed 'U' characters:
    // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
    // pulse_D11(); // scope trigger
    // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
}





void nooop(void) {
}

void nmain(void) {


    while(1) {
        send_right_curly_brace();
        // send_m_m_m(); // send_mmmm    lower-case em

#define ISNT_COMMENTED_OUT
#undef  ISNT_COMMENTED_OUT
#ifndef ISNT_COMMENTED_OUT
        // send_uuu();
        tx_to_vcc();

        // very long time between typed 'U' characters:
        for (int j =  3899; j > 0; j--) { hold_for_tick_change(); }
        pulse_D11(); // scope trigger
        for (int j =   2; j > 0; j--) { hold_for_tick_change(); }

        // ---- new stanza

        tx_to_vcc();
        hold_for_tick_change();
        _one_gap();
        for (int i =  9; i > -1 ; i--) {
            if (i ==  9) { _one_gap();   }
            if (i ==  8) { _one_pulse(); }
            if (i ==  7) { _one_gap();   }
            if (i ==  6) { _one_pulse(); }
            if (i ==  5) { _one_gap();   }
            if (i ==  4) { _one_pulse(); } 
            if (i ==  3) { _one_gap();   }
            if (i ==  2) { _one_pulse(); }
            if (i ==  1) { _one_gap();   }
            if (i ==  0) { _one_pulse(); }
            hold_for_tick_change();
        }
        _one_gap();
        hold_for_tick_change();
        tx_to_vcc();

        // very long time between typed 'U' characters:
        // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }
        // pulse_D11(); // scope trigger
        // for (int j =  7899; j > 0; j--) { hold_for_tick_change(); }

        // ---- closed, stanza
#endif // #ifndef ISNT_COMMENTED_OUT

    } // while











    while(1) { // ---------- trapped -------------
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
    PORT->Group[0].DIRSET.reg  = (uint32_t)(1 << 21); // PA21 //  1 11 pinmode  // D11
    clock_init();
    // SysTick_Config(5500); // 19200 baud has    52   uSec pulses


    // smaller SysTick means ticks come more rapidly

    // To make pulses much wider, increase SysTick tenfold

    // system runs at half speed so need to double-quicken it here to 26 uSec
    // SysTick_Config(5320); // 19200 baud has    52   uSec pulses
    // SysTick_Config(2660); // 19200 baud has    52   uSec pulses

    // 1330 is a bit too hot -- slow it a bit
    // SysTick_Config(1330); // 19200 baud has    52   uSec pulses
    int tick_rate = 1390; // 1390 is very fast for 38400
    // tick_rate = 710;
    // tick_rate = 355;
    // tick_rate = 1420;


    // tick_rate = 1400;  // it's a delay, a lower value means a shorter delay
    // tick_rate = 1500;  // it's a delay, a lower value means a shorter delay
    // 1400 may be too fast but 1500 is too slow and produces a nice 'j' once in a while
    // 1470 gave an occasional U and a Capital O once in a while
    // 1440 generates the usual close but not perfect pattern
    // 1448 had few errors but enough to want to pursue
// ###bookmark
    //  1368 1368 1368  -- thirteen sixty eight had NO ERRORS in very long runs.
    // tick_rate = 1368;  // it's a delay, a lower value means a shorter delay
    tick_rate = 1358;  // it's a delay, a lower value means a shorter delay
    push(tick_rate);
    // SysTick_Config(1420); // 19200 baud has    52   uSec pulses
    SysTick_Config(tick_rate); // 19200 baud has    52   uSec pulses

    nmain();
    while (1) {
        // none
    }
}
