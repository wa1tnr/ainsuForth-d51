// Thu Aug  9 22:06:59 UTC 2018

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
    PORT->Group[1].DIRSET.reg  = (uint32_t)(1 << 16); // PB16 //  1 16 pinmode  // TX
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
// #define      OSC_RATIO_AINSU 3661 // 20 MHz


// 15 MHz like Arduino?
#define      OSC_RATIO_AINSU 2746

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


void uSec(void) {
    for (volatile int i = 1; i < 2; i++) { // not really uSec unless calibrated
    }
}

// secu 129 too tight a timing -- loosen (slow it)
// see a lot of QQQ with 137 #define SECU 137

// SECU 141 is on the money for 38400 bps  09 Aug 23:15 UTC
#define SECU 141

void pip_space(void) {
// 5 MHz / 200 ns is SECU 1000
// so what is that -- 3x reduction (from 15 MHz)
// it's an exact number so that's a clue.
// the shapes of the curves are awful.  Not square
// enough by a long shot.

// #define SECU 1000

// 8.7 uSec is measured width at 115200.  Want 38400 for now.
// 26  uSec approx for 38400  and 3.3v
// LSB on left sent first MSB on right sent last.
// zero is ground and one is 3.3v
// quiescent is 3.3v (endless 11111.. but no marking to distinguish them)
// no gap between successive 'pulses' of the same value (GND or 3.3)
// so you just turn on (or off) the pin, and allow a timer to expire
// that has counted out the time interval for that many pulses (of
// 26 uSec duration, per, or thereabouts).

    for (volatile int j = SECU; j>0; j--) {
    }
}


void send_sm_nothing(void) { // time gap
    for (volatile int p=335;p>0; p--) {
        pip_space(); // delay 26 uSec
    }
}

void send_nothing(void) { // time gap
    for (volatile int p=3355;p>0; p--) {
        pip_space(); // delay 26 uSec
    }
}

void common_end(void) {

    // post-amble hi hi

    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
    pip_space(); // delay 26 uSec
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
    // now a very wide space
    pip_space(); // delay 26 uSec
    send_nothing(); // guard time
}

void bit_CLR(void) { // send the bit cleared - ground the GPIO pin
    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
    pip_space(); // delay 26 uSec
}
void bit_SET(void) { // send the bit set - raise the GPIO pin to 3.3 VDC
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
    pip_space(); // delay 26 uSec
}


void send_preamble(void) {
    // preamble

    send_nothing(); // guard time
    pip_space(); // delay 26 uSec

    bit_CLR();

    // preamble ends.
}

void send_groups(void) { // pattern

    send_preamble();
    bit_CLR(); bit_CLR(); bit_CLR();
    bit_CLR();
    bit_SET(); bit_SET(); bit_SET();
    bit_SET();
    bit_CLR(); bit_SET();
    bit_SET(); bit_CLR();
    common_end();

}

void send_skeleton(void) { // pattern
    send_preamble();
    bit_CLR();
    bit_SET();
    common_end();
}


// ###bookmark
// ###bookmark
void send_bravo(void) { // 01 0000 1

    send_preamble();
    bit_CLR(); bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET();
    common_end();
}

void send_foxtrot(void) { // 01 1000 1

    send_preamble();
    bit_CLR(); bit_SET();
    bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET();
    common_end();
}

void send_delta(void) { // 00 1000 1

    send_preamble();
    bit_CLR(); bit_CLR();
    bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET();
    common_end();
}

void send_echo(void) { // 01 1000 1
   //  new            echo:     101 000 1

    send_preamble();
    bit_SET(); bit_CLR(); bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET();
    common_end();
}

void send_charley(void) { // 11 0000 1
    send_preamble();
    bit_SET(); bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR(); bit_CLR(); 
    bit_SET();
    common_end();
}


// ###bookmark 
// ###bookmark

void send_golf(void) { // 111 000 1

    send_preamble();
    bit_SET(); bit_SET(); bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET();
    common_end();
}


void send_three(void) { // zero then double alt  11 00 11 0
    send_preamble();
    bit_SET(); bit_SET();
    bit_CLR(); bit_CLR();
    bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

void send_two(void) { // like three: 01 00 11 0
    send_preamble();
    bit_CLR(); bit_SET();
    bit_CLR(); bit_CLR();
    bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}


void send_lf(void) { // 0101000
    send_preamble();
    bit_CLR(); bit_SET();
    bit_CLR(); bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR();
    common_end();
}

void send_cr(void) { // 1011000
    send_preamble();
    bit_SET();
    bit_CLR();
    bit_SET(); bit_SET();
    bit_CLR(); bit_CLR(); bit_CLR();
    common_end();
}

void send_crlf(void) {
    send_cr();
    send_lf();
}

void send_space(void) { // ascii 32 0x20 delimiter
    // preamble

    send_nothing(); // guard time
    pip_space(); // delay 26 uSec

    bit_CLR();

    // preamble ends.


    // send 2^0 bit:
    bit_CLR();

    // send 2^1 bit:
    bit_CLR();

    // send 2^2 bit:
    bit_CLR();

    // send 2^3 bit:
    bit_CLR();

    // send 2^4 bit:
    bit_CLR();

    // send 2^5 bit:
    bit_SET();

    // send 2^6 bit:
    bit_CLR();

    // post-amble hi hi

    common_end();
}


void send_gap(void) { 
    send_sm_nothing();
    send_space();    // character delimiter: whitespace
}

void send_gapp(void) { // send the gap between character groups
    send_nothing();  // large inter-char time delay
    send_space();    // character delimiter: whitespace
}


void send_gapspace(void) {
    send_gap();
    send_space();
    send_gap();
}

void send_crfl(void) {
    send_cr();
    send_gapspace();
    send_lf();
    send_gapspace();
}

void send_zero(void) { // 0000110
    send_preamble();
    bit_CLR(); bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

void send_nine(void) { // 1 00 111 0
    send_preamble();
    bit_SET();
    bit_CLR(); bit_CLR();
    bit_SET(); bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

void send_six(void) { // 011 011 0
    send_preamble();
    bit_CLR(); bit_SET(); bit_SET();
    bit_CLR(); bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

void send_eight(void) { // 0001110 maybe
    send_preamble();
    bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET(); bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

void send_november(void) { // zero one one one zero zero one
    send_preamble();
    bit_CLR();
    bit_SET();
    bit_SET();
    bit_SET();
    bit_CLR();
    bit_CLR();
    bit_SET();
    common_end();
}

void send_romeo(void) { // 0 1 00 101
    send_preamble();
    bit_CLR();
    bit_SET();
    bit_CLR();
    bit_CLR();
    bit_SET();
    bit_CLR();
    bit_SET();
    common_end();
}

void send_four(void) { // 0010110
    send_preamble();
    bit_CLR(); bit_CLR();
    bit_SET(); bit_CLR();
    bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

void send_tango(void) { // double zero then alternate out to the end
    send_preamble();

    bit_CLR();
    bit_CLR();
    bit_SET();
    bit_CLR();
    bit_SET();
    bit_CLR();
    bit_SET();
    common_end();
}

void send_one(void) { // send_one(); one triple zero one one zero : number_one

    send_preamble();









    bit_SET();
    bit_CLR();
    bit_CLR();
    bit_CLR();
    bit_SET();
    bit_SET();
    bit_CLR();

    common_end();
}

void send_alpha(void) { // one bit at beginning and end, four clear bits in between

    send_preamble();

    bit_SET();
    bit_CLR();
    bit_CLR();
    bit_CLR();
    bit_CLR();
    bit_CLR();
    bit_SET();

    common_end();
}



void send_whiskey(void) { // three set bits and then alternating totaling five

    // preamble

    send_nothing(); // guard time
    pip_space(); // delay 26 uSec

    bit_CLR();

    // preamble ends.


    // send 2^0 bit:
    bit_SET();

    // send 2^1 bit:
    bit_SET();

    // send 2^2 bit:
    bit_SET();

    // send 2^3 bit:
    bit_CLR();

    // send 2^4 bit:
    bit_SET();

    // send 2^5 bit:
    bit_CLR();

    // send 2^6 bit:
    bit_SET();

    common_end();
}


void send_seven(void) { // set set set clear set set

    // preamble

    send_nothing(); // guard time
    pip_space(); // delay 26 uSec

    bit_CLR();

    // preamble ends.


    // send 2^0 bit:
    bit_SET();

    // send 2^1 bit:
    bit_SET();

    // send 2^2 bit:
    bit_SET();

    // send 2^3 bit:
    bit_CLR();

    // send 2^4 bit:
    bit_SET();

    // send 2^5 bit:
    bit_SET();

    // send 2^6 bit:
    bit_CLR();

    common_end();
}



void send_five(void) { // set clear set clear set set is five
    // preamble

    send_nothing(); // guard time
    pip_space(); // delay 26 uSec

    bit_CLR();

    // preamble ends.


    // send 2^0 bit:
    bit_SET();

    // send 2^1 bit:
    bit_CLR();

    // send 2^2 bit:
    bit_SET();

    // send 2^3 bit:
    bit_CLR();

    // send 2^4 bit:
    bit_SET();

    // send 2^5 bit:
    bit_SET();

    // send 2^6 bit:
    bit_CLR();

    // post-amble hi hi

    common_end();
}



void send_at_symb(void) { // send at symbol circle a
    // preamble

    send_nothing(); // guard time
    pip_space(); // delay 26 uSec

    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
    pip_space(); // delay 26 uSec


    // preamble ends.



    // send 2^0 bit:
    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16, clr
    pip_space(); // delay 26 uSec

    // send 2^1 bit:
    pip_space(); // delay 26 uSec

    // send 2^2 bit:
    pip_space(); // delay 26 uSec

    // send 2^3 bit:
    pip_space(); // delay 26 uSec

    // send 2^4 bit:
    pip_space(); // delay 26 uSec

    // send 2^5 bit:
    pip_space(); // delay 26 uSec

    // send 2^6 bit:
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
    pip_space(); // delay 26 uSec

    // post-amble hi hi

    common_end();
}


void send_quebec(void) { // send upper case Q
    // preamble
    // comment

    send_nothing(); // guard time

    pip_space(); // delay 26 uSec
    // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 16); // PA16 // D11 toggle

    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
    pip_space(); // delay 26 uSec


    // preamble ends.



    // send 2^0 bit:
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
    pip_space(); // delay 26 uSec



    // send 2^1 bit:
    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16, clr
    pip_space(); // delay 26 uSec

    // send 2^2 bit:
    pip_space(); // delay 26 uSec

    // send 2^3 bit:
    pip_space(); // delay 26 uSec

    // send 2^4 bit:
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
    pip_space(); // delay 26 uSec

    // send 2^5 bit:
    // PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite

    PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
    // PORT->Group[1].OUTCLR.reg = (uint32_t)(1 << 16); // PB16 //  0 16 pinwrite
    pip_space(); // delay 26 uSec

    // send 2^6 bit:

    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 16); // PB16 //  1 16 pinwrite
    pip_space(); // delay 26 uSec

    // post-amble hi hi
    common_end();
}


// ###bookmark
// ###bookmark
void printZero(void) { // 0000110
    // send_preamble();
    bit_CLR(); bit_CLR(); bit_CLR(); bit_CLR();
    bit_SET(); bit_SET();
    bit_CLR();
    common_end();
}

uint8_t hold = 127; // input
uint8_t held = -1; // register
uint8_t scrap = -1; // dodge
uint8_t stuffit = -1 ; // gonna print this one


void printj(void) {
    if ((held && hold)  == held) { send_zero(); send_gap(); }
    if ((!held && hold)  == !held) { send_one(); send_gap(); }
    send_at_symb();                send_gap();
    send_space();                send_gap();
}

void printp(void) {
//  if ((held && hold) == held) { // held is the bitmask

        if (held && 0) { send_november(); send_gap(); send_zero(); send_gapspace();}
        if (held &&  1) { send_november(); send_gap(); send_one(); send_gapspace(); }
        if (held &&  2) { send_november(); send_gap(); send_two(); send_gapspace();}
        if (held &&  4) { send_november(); send_gap(); send_four(); send_gapspace();}
        if (held &&  8) { send_november(); send_gap(); send_eight(); send_gapspace();}
        if (held && 16) { send_november(); send_gap(); send_one(); send_gap(); send_six(); send_gapspace();}
        if (held && 32) { send_november(); send_gap(); send_three(); send_gap(); send_two(); send_gapspace();}
        if (held && 64) { send_november(); send_gap(); send_six(); send_gap(); send_four(); send_gapspace();}

        if (hold == 80) { send_alpha();   send_gapspace(); }
        if (hold == 81) { send_bravo();   send_gapspace(); }
        if (hold == 82) { send_charley();   send_gapspace(); }
        if (hold == 83) { send_delta();   send_gapspace(); }
        if (hold == 84) { send_echo();   send_gapspace(); }
        send_at_symb(); send_gapspace();
        send_at_symb(); send_gapspace();
        send_at_symb(); send_gapspace();
        send_at_symb(); send_gapspace();
 // }
}

void printty_zero(void) {
    send_zero();
    send_gapspace();
}
void printty_one(void) {
    send_one();
    send_gapspace();
}


void showbits(unsigned int x) {
    for(int i = 0; i < 8; i++) {
       (x & (1u << i)) ? printty_zero() : printty_one();
    }
    send_crlf(); send_gapspace();
}


void blitwbits(unsigned int x) {
    for(int i = 0; i < 7; i++) {
       // (x & (1u << i)) ?  bit_CLR() : bit_SET();
       (x & (1u << i)) ?  bit_SET() : bit_CLR();
    }
    // send_crlf(); send_gapspace();
}

void print_mar(void) {
    // stuffit = 43;

    uint8_t sample = 43;
    send_preamble();
    blitwbits(sample);
    common_end();

    send_preamble();
    sample = 64;
    blitwbits(sample);
    common_end();

} //  stuffit =  43;



void do_that(void) {
    // int j = 0;
    // send_preamble(); // begin the serial frame
    for (int i=0; i<7; i++) { // scan from LSB to MSB
        // int result =    1  << i; // enumerate each bit
        // scrap = result;
        // held = result;
        held = i;
        // printj();
        printp();
    }
    // common_end();
}

void do_this(void) {
    send_crlf(); send_gapspace();
    for (int i=0; i<10; i++) {
        hold = 80 + i;
        // do_that();
        showbits(hold);
    }
}
// ###bookmark
// ###bookmark


int main(void) {
    // Nice triple, 8 MHZ:
    SystemCoreClockUpdate();
    SystemInit();
    pins_setup(); // D11 in OUTPUT mode

    ClockInit120();
    setup_PA14_as_GCLK_IO();
    // omit:
    // SysTick_Config(4000);

    stuffit = 43;
    send_crlf(); send_gapspace();
    print_mar();

    do_this();



    for (volatile int k=21044444; k>0; k--){
        for (volatile int l=21044444; l>0; l--){
            for (volatile int m=21044444; m>0; m--){
            }
        }
    }



    for (int j=44444; j>0; j--){
        // send_skeleton(); // compiler warning

                   send_gapp();

        send_whiskey();        send_gapp();
        send_tango();          send_gapp();
        send_quebec();         send_gapp();
        send_romeo();          send_gapp();
        send_november();       send_gapp();

        send_golf();           send_gapp();
        send_foxtrot();        send_gapp();
        send_echo();           send_gapp();
        send_delta();          send_gapp();
        send_charley();        send_gapp();
        send_bravo();          send_gapp();
        send_alpha();          send_gapp();

        send_zero();           send_gapp();
        send_nine();           send_gapp();
        send_eight();          send_gapp();
        send_seven();          send_gapp();
        send_six();            send_gapp();
        send_five();           send_gapp();
        send_four();           send_gapp();
        send_three();          send_gapp();
        send_two();            send_gapp();
        send_one();            send_gapp();

        send_at_symb();        send_gapp();
    }

// 5 MHz / 200 ns is SECU 1000
// so what is that -- 3x reduction (from 15 MHz)
// it's an exact number so that's a clue.
// the shapes of the curves are awful.  Not square
// enough by a long shot.

// #define SECU 1000
// #define SECU 129

// 8.7 uSec is measured width at 115200.  Want 38400 for now.
// 26  uSec approx for 38400  and 3.3v
// LSB on left sent first MSB on right sent last.
// zero is ground and one is 3.3v
// quiescent is 3.3v (endless 11111.. but no marking to distinguish them)
// no gap between successive 'pulses' of the same value (GND or 3.3)
// so you just turn on (or off) the pin, and allow a timer to expire
// that has counted out the time interval for that many pulses (of
// 26 uSec duration, per, or thereabouts).

    while (1) {
        for (volatile int j = SECU; j>0; j--) {
        }
        PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 21); // PA21 // D11 toggle
    }
}

/*
        // pin setup - Feather M4 Express, D11 (PA21) GPIO in OUTPUT mode:
        PORT->Group[0].DIRSET.reg  = (uint32_t)(1 << 21); // PA21 //  1 11 pinmode  // D11

        // pin toggle - PA21
        PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 21); // PA21 // D11 toggle

*/
