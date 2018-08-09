// Thu Aug  9 18:34:58 UTC 2018

#include "atmel_start.h"

// raw version -- omits are commented, to improve performance characteristics

// see the cooked version for confidence building (LED blinkie version)

#define SCALED 8000 // D13 toggle rate scaler - too fast for human eye otherwise

int tick_h = -1;


// https://www.avrfreaks.net/forum/resolved-samd51-asf4start-clock-fail-debugger-hang

void pins_setup(void) {
    PORT->Group[0].DIRSET.reg  = (uint32_t)(1 << 21); // PA21 //  1 11 pinmode  // D11
    PORT->Group[0].DIRSET.reg  = (uint32_t)(1 << 23); // PA23 //  1 13 pinmode  // D13
    // toggle: // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 21); // PA21 // D11 toggle
}

void ClockInit120(void)
{
	// CONFIGURE FLASH MEMORY WAIT STATES
	//NVMCTRL->CTRLA.bit.RWS = 5; // 4WS allows up to 119 MHz, 5WS allows 120 MHz
	NVMCTRL->CTRLA.bit.AUTOWS = 1; // automatically determine the necessary wait states

	// CONFIGURE EXTERNAL 32K CRYSTAL OSCILLATOR
	OSC32KCTRL->XOSC32K.bit.CGM = 1; // control gain mode: 1 = standard/XT 2 = high-speed/HS
	OSC32KCTRL->XOSC32K.bit.STARTUP = 0; // oscillator startup time, 0 = 62 ms
	OSC32KCTRL->XOSC32K.bit.ONDEMAND = 0; // always run
	OSC32KCTRL->XOSC32K.bit.XTALEN = 1; // enable crystal driver circuit for XIN32/XOUT32 pins
	OSC32KCTRL->XOSC32K.bit.EN32K = 1; // enable the 32 kHz output clock
	OSC32KCTRL->XOSC32K.bit.ENABLE = 1;

	while (!OSC32KCTRL->STATUS.bit.XOSC32KRDY); // wait until crystal oscillator is stable and ready to be used as a clock source

	OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K_Val; // RTC should use the external 32K crystal

	// CONFIGURE MASTER CLOCK
	MCLK->CPUDIV.bit.DIV = 1; // use a divisor of 1 for the master clock

	// CONFIGURE PLL0
	OSCCTRL->Dpll[0].DPLLCTRLB.bit.REFCLK = 1; // use OSC32K as the PLL reference clock
	// OSCCTRL->Dpll[0].DPLLRATIO.reg = (3<<16) + 3661; // multiply OSC32K by 3662.11 to get 120 MHz (actual multiplier is LDR + 1 + LDRFRAC/32)


#define      OMIT_THESE_ALTERNATES
#ifndef      OMIT_THESE_ALTERNATES
    #define  OSC_RATIO_AINSU 1220
    #define  OSC_RATIO_AINSU 1830
    #define  OSC_RATIO_AINSU 3661
#endif // #ifndef OMIT_THESE_ALTERNATES


// the selected DPLL ratio:
#define      OSC_RATIO_AINSU 3661 // 20 MHz
#define      OSC_RATIO_AINSU 2746 // 15 MHz like Arduino?

        // try 1220 - 6.66 MHz on D11 and 40 MHz D4
        // try 1830 - 10.0 MHz on D11 
        // try 3661 - 20.0 MHz on D11 .. said to be a 120 MHz GCLK 0
	OSCCTRL->Dpll[0].DPLLRATIO.reg = (3<<16) + OSC_RATIO_AINSU ; // multiply OSC32K by 3662.11 to get 120 MHz (actual multiplier is LDR + 1 + LDRFRAC/32)

	// errata: When using a low-frequency input clock on FDPLLn, several FDPLL unlocks may occur while the output
	// frequency is stable. Workaround: when using a low-frequency input clock on FDPLLn, enable the lock bypass
	// feature to avoid FDPLL unlocks.
	OSCCTRL->Dpll[0].DPLLCTRLB.bit.LBYPASS = 1; // CLK_DPLL0 output clock is always on, and not dependent on frequency lock
	OSCCTRL->Dpll[0].DPLLCTRLA.bit.ONDEMAND = 0; // always run
	OSCCTRL->Dpll[0].DPLLCTRLA.bit.ENABLE = 1;

	while (!OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK) ; // no point in checking DPLLSTATUS.bit.CLKRDY, because LBYPASS is enabled

	// CONFIGURE CLOCK GENERATOR 0
	GCLK->GENCTRL[0].bit.SRC = 7; // use PLL0 as the input
	GCLK->GENCTRL[0].bit.DIV = 1; // divisor 1 for the input clock from the PLL
	GCLK->GENCTRL[0].bit.GENEN = 1;

	// do peripheral clock initialization here...
}

void setup_PA14_as_GCLK_IO(void) {

    // configure PA14 (board D4) as GCLK_IO[0] output of main clock generator,
    // to confirm 120 MHz operation
    PORT->Group[GPIO_PORTA].DIRSET.reg = (1 << 14); // set pin as output
    PORT->Group[GPIO_PORTA].PINCFG[14].bit.PMUXEN = 1; // enable the peripheral mux for this pin
    PORT->Group[GPIO_PORTA].PMUX[(14>>1)].bit.PMUXE = MUX_PA14M_GCLK_IO0; // select the GCLK_IO0 peripheral function
    GCLK->GENCTRL[0].bit.OE = 1; // enable output from clock generator 0
}


// for confidence -- omit in working program:
// void SysTick_Handler(void){
//  tick_h++ ;
// }
// omit the above three lines


void led_stuph(void) {
    if (tick_h > SCALED) {
        PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // PA23 // D13 toggle
        tick_h = -1;
    }
}

int main(void) {
    // Nice triple, 8 MHZ:
    SystemCoreClockUpdate();
    SystemInit();
    pins_setup(); // D11 in OUTPUT mode

    ClockInit120();
    setup_PA14_as_GCLK_IO();
    // omit:
    // SysTick_Config(4000);

    while (1) {
        // do something
        // this test can be repeated in an arduino program,
        // to compare empty loops between platforms and clock setups,
        // possibly (may not be a valid approach):

        PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 21); // PA21 // D11 toggle
        // shows 20 MHz on D11 GPIO

        // confidence builder only -- omit this line:
        // led_stuph();
    }
}

/*
        // pin setup - Feather M4 Express, D11 (PA21) GPIO in OUTPUT mode:
        PORT->Group[0].DIRSET.reg  = (uint32_t)(1 << 21); // PA21 //  1 11 pinmode  // D11

        // pin toggle - PA21
        PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 21); // PA21 // D11 toggle

*/


