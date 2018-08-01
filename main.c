// Wed Aug  1 04:18:10 UTC 2018
// Tue Jul 31 13:45:08 UTC 2018

/*
 git $ ag 38400
hpl_sercom_config.h
1415:#define CONF_SERCOM_5_USART_BAUD 38400
*/

// main.c

#include "atmel_start.h"
#include "gpio_local.h"
#include "usart.h"

/* Many changes: by wa1tnr, July 2018 */

void uSec(void) {
    for (volatile int i = 1; i < 2; i++) { // needs calibration
        // nothing
    }
}

void short_timer(void) { // human blinkie timescale
    uint32_t on_time  = 2140111222; // it's 2147 something ;)
    for(on_time =       2140112; on_time > 0; on_time--) { // 21.4 million
        uSec();
    }
}

void raise_LED_pins(void) { // multiple target boards
    // raise_D13(); raise_D12();
    raise_D13_metro();   // Metro M4 Express
    raise_D13_feather(); // Feather M4 Express
}

void lower_LED_pins(void) {
    // lower_D13(); lower_D12();
    lower_D13_metro();
    lower_D13_feather();
}

void blink_LED(void) {
    // raise_D13(); raise_D12();
    raise_LED_pins(); // Feather M4 or Metro M4
    short_timer(); // should be first instance of use of this timer
    lower_LED_pins(); // lower_D13(); lower_D12();
    short_timer();
}

void flicker_LED(void) {
    raise_LED_pins(); // raise_D13(); raise_D12();
    short_timer();
    lower_LED_pins(); // lower_D13(); lower_D12();
    short_timer();
}

void blinkLEDfast(void) {
    for(int i = 15; i > 0; i--) {
        blink_LED();
    }
}

void activity_LED_demo(void) {
    // init_act_LED();
    blinkLEDfast();
}

void long_long_timer(void) {
    for (int j=4; j>1; j--){
        for (int i=3; i>1; i--){
            short_timer();
        } // fake delay
    } // faker delay haha
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
        raise_LED_pins(); // raise_D13(); raise_D12();
        // activity_LED_demo();
	while (1) {
            flicker_LED();
            // short_timer();

            // this demo stops everything:
            // USUART_5_demo(); // let's see what the cat drug in
            short_timer();
	}
}

int main(void) {
    SystemInit();
    // clock_init();
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


/*
 * clock stuff is from Jake Read.
 *
 * mkstepper-v011.c
 *
 * Created: 2/5/2018 12:00:35 PM
 * Author : Jake
 * https://gitlab.cba.mit.edu/jakeread/atkstepper17/tree/e6559372a14fa69ba360da903480d962d786744f
 *
 * https://gitlab.cba.mit.edu/jakeread/atkstepper17/blob/e6559372a14fa69ba360da903480d962d786744f/embedded/mkstepper-v011/mkstepper-v011/main.c
 *
 * TODO: simplify this citation. ;)
 *
 */

/*

// clock clock who's there ///////////////////

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

int main(void) {
    SystemInit();
    clock_init();
    while (1) {
        // any code here .. or none
    }
}

*/

#ifdef AUTHORS_UPSTREAM
/*
 * mkstepper-v011.c
 *
 * Created: 2/5/2018 12:00:35 PM
 * Author : Jake
 */ 

void clock_init(void){
	
	// on Reset, the DFLL48< source clock is on and running at 48MHz
	// GCLK0 uses DFLL48M as a source and generates GCLK_MAIN
	// we want to use OSCCTRL to (1) set the DFLL48M to run on a reference clock, in closed-loop mode
	// (20 then to prescale the DFLL48M such that it runs at 120MHz
	
	// generic clock channel 0 is the reference for the DFLL - we'll try to set that up first
	
	// for 120mhz do https://github.com/adafruit/ArduinoCore-samd/blob/samd51/cores/arduino/startup.c
	
	// something?
	NVMCTRL->CTRLA.reg |= NVMCTRL_CTRLA_RWS(0);
	
	// reset
	GCLK->CTRLA.bit.SWRST = 1;
	while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST);
	
	// Setup internal reference to gclk gen 3
	GCLK->GENCTRL[3].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN;
	while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL3);
	GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN;
	while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);
	
	// enable DFLL48M clock
	OSCCTRL->DFLLCTRLA.reg = 0;
	OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP(0x1) | OSCCTRL_DFLLMUL_FSTEP(0x1) | OSCCTRL_DFLLMUL_MUL(0);
	while(OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLMUL);
	OSCCTRL->DFLLCTRLB.reg = 0;
	while(OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLCTRLB);
	OSCCTRL->DFLLCTRLA.reg |= OSCCTRL_DFLLCTRLA_ENABLE;
	while(OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_ENABLE);
	OSCCTRL->DFLLCTRLB.reg = OSCCTRL_DFLLCTRLB_WAITLOCK | OSCCTRL_DFLLCTRLB_CCDIS | OSCCTRL_DFLLCTRLB_USBCRM;
	while(!OSCCTRL->STATUS.bit.DFLLRDY);
	
	// this is generating a reference for our 120mhz
	GCLK->GENCTRL[5].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(24u);
	while(GCLK->SYNCBUSY.bit.GENCTRL5);
	
	// setup DPLL0 to 120MHz
	GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL0].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK5_Val);
	OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0x00) | OSCCTRL_DPLLRATIO_LDR(59);
	while(OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.DPLLRATIO);
	OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
	OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;
	while(OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK == 0);
	// set clock to use dpll0
	
	GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DPLL0) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN;
	while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);
	
	MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	clock_init();
	SysTick_Config(4000000);
	
	// lights
	stlb = pin_new(&PORT->Group[1], 14);
	pin_output(&stlb);
	pin_set(&stlb);
	stlr = pin_new(&PORT->Group[1], 13);
	pin_output(&stlr);
	pin_set(&stlr);
	
	// ready interrupt system
	__enable_irq();
	NVIC_EnableIRQ(SERCOM4_0_IRQn); //up0tx
	NVIC_EnableIRQ(SERCOM4_2_IRQn); //up0rx
	NVIC_EnableIRQ(SERCOM5_0_IRQn);
	NVIC_EnableIRQ(SERCOM5_2_IRQn);
	NVIC_EnableIRQ(TC0_IRQn);
	NVIC_EnableIRQ(TC2_IRQn);
	
	// ringbuffers (for uart ports)
	rb_init(&up0_rbrx);
	rb_init(&up0_rbtx);
	rb_init(&up1_rbrx);
	rb_init(&up1_rbtx);
	
	/*
	pin_t testpin = pin_new(&PORT->Group[0], 12);
	pin_output(&testpin);
	pin_set(&testpin);
	
	while(1){
		pin_toggle(&testpin);
	}
	*/
	
	// uarts (ports)
	// there's some chance we should really be using one gclk channel for all of the peripherals
	// and running that at 100MHz or so 
	
	up0 = uart_new(SERCOM4, &PORT->Group[0], &up0_rbrx, &up0_rbtx, 12, 13, HARDWARE_IS_APBD, HARDWARE_ON_PERIPHERAL_D); 
	MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM4;
	uart_init(&up0, 7, SERCOM4_GCLK_ID_CORE, 62675); // baud: 45402 for 921600, 63018 for 115200
	
	up1 = uart_new(SERCOM5, &PORT->Group[1], &up1_rbrx, &up1_rbtx, 3, 2, HARDWARE_IS_APBD, HARDWARE_ON_PERIPHERAL_D);
	MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5;
	uart_init(&up1, 7, SERCOM5_GCLK_ID_CORE, 62675);
	
	ups[0] = &up0;
	ups[1] = &up1;
	
	up0_stlr = pin_new(&PORT->Group[0], 19);
	pin_output(&up0_stlr);
	pin_set(&up0_stlr);
	up0_stlb = pin_new(&PORT->Group[0], 17);
	pin_output(&up0_stlb);
	pin_set(&up0_stlb);
	up1_stlr = pin_new(&PORT->Group[0], 21);
	pin_output(&up1_stlr);
	pin_set(&up1_stlr);
	up1_stlb = pin_new(&PORT->Group[0], 23);
	pin_output(&up1_stlb);
	pin_set(&up1_stlb);
	
	apaport_build(&apap0, 0, &up0, &up0_stlr, &up0_stlb);
	apaport_reset(&apap0);
	apaport_build(&apap1, 1, &up1, &up1_stlr, &up1_stlb);
	apaport_reset(&apap1);
	
	// SPI
	// TMC_MOSI		PA07 / SER0-3
	// TMC_MISO		PA04 / SER0-0
	// TMC_SCK		PA05 / SER0-1
	// TMC_CSN		PA06 / SER0-2
	
	// spi
	spi_tmc = spi_new(SERCOM0, &PORT->Group[0], 4, 7, 5, 6, HARDWARE_IS_APBA, HARDWARE_ON_PERIPHERAL_D);
	MCLK->APBAMASK.reg |= MCLK_APBAMASK_SERCOM0;
	spi_init(&spi_tmc, 8, SERCOM0_GCLK_ID_CORE, 8, 0, 2, 0, 1, 1, 0);
	
	// TMC
	// STEP			PB08
	// DIR			PB09
	// EN			PB07
	// SG			PB06
	
	en_pin = pin_new(&PORT->Group[1], 7);
	sg_pin = pin_new(&PORT->Group[1], 6);
	
	tmc = tmc26_new(&spi_tmc, &en_pin, &sg_pin);
	// set registers
	tmc26_init(&tmc);
	// enable
	tmc26_enable(&tmc);
	
	ticker_init(); // for counting
	// 32 bit timer, each tick is 1/3 of one us.
	// to read, do TICKER_SYNC, and then var = TICKER as defined
	// overflows in 17.9 minutes
	
	stepticker_init(); // fires interrupt to check if we need to take a step
	// 32 bit timer at 48mhz / 64 and fires on top of 12 for a 16us period: 48 small ticks of 1/3us within each period
	
	step_pin = pin_new(&PORT->Group[1], 8);
	dir_pin = pin_new(&PORT->Group[1], 9);
	pin_output(&step_pin);
	pin_output(&dir_pin);
	
	stepper = stepper_new(&step_pin, &dir_pin);
	stepper_reset(&stepper);
		
	uint8_t lpcnt = 0;
	uint8_t stpcnt = 0;
	
    while (1) 
    {
		apaport_scan(&apap0, 2);
		apaport_scan(&apap1, 2);
		
		/*
		now: step timer should not accumulate error !
		*/
			
		/*
		if(stepper.position_ticks_target == stepper.position_ticks){
			stpcnt ++;
			//pin_clear(&stlr);
			//stepper_targetposition_steps(&stepper, 1000 * (stpcnt %2));
			//stepper_targetspeed_steps(&stepper, 500);
			//stepper_goto(&stepper, 360 * (stpcnt % 2), 1200);
		} else {
			//pin_set(&stlr);
		}
		*/
    }
}

#endif // #ifdef AUTHORS_UPSTREAM
