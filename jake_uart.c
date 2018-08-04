// USART on the ATSAMD51 - Jake Read - modified by wa1tnr
// #include "sam.h"
#include "driver_init.h"
#include "blinker.h"
// #include "gpio_local.h"

volatile uint8_t byte_seen = 0; // RX data UART

// Interrupts on the ATSAMD51

// To configure an interrupt on the ATSAMD51, roughly, we do this:

void interrupts_setup(void){

    __enable_irq(); // globally enables the NVIC

    // turns on a particular 'irq line' or 'irq number'
    // for example, in the SERCOM->UART section of the datasheet, 
    // see 34.8.8 - the interrupt 'flag' sections. these are analagous to 'line numbers'
    // and are linked to 'interrupt numbers' per 10.2.2
    // NVIC_EnableIRQ(SERCOM4_2_IRQn); 
    NVIC_EnableIRQ(SERCOM5_0_IRQn); // TX
    NVIC_EnableIRQ(SERCOM5_1_IRQn); // RX
}

void jake_uart(void) { // main()
    /* Initialize the SAM system */
    // SystemInit();

    // clock_init();

    // SysTick_Config(5000000);

    /* setup UARTs
    NP2TX       PB02 / SER5-0 / Peripheral D  PB16
    NP2RX       PB03 / SER5-1 / Peripheral D  PB17
    */

    // setup pins for peripheral
    PORT->Group[1].DIRCLR.reg = (uint32_t)(1 << 17); // rx is input  PB17
    PORT->Group[1].DIRSET.reg = (uint32_t)(1 << 16); // tx output    PB16
    PORT->Group[1].PINCFG[3].bit.PMUXEN = 1;
    PORT->Group[1].PMUX[3>>1].reg |= PORT_PMUX_PMUXE(0x3); 
    PORT->Group[1].PMUX[3>>3].reg |= PORT_PMUX_PMUXO(0x3);
    PORT->Group[1].PINCFG[2].bit.PMUXEN = 1;
    PORT->Group[1].PMUX[2>>1].reg |= PORT_PMUX_PMUXE(0x3);
    PORT->Group[1].PMUX[2>>3].reg |= PORT_PMUX_PMUXO(0x3);
    // unmask clocks
    MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5;
    // generate clocks to, starting with clock 6 (arbitrary choice, lower # held for system things)
    // datasheet says normally one gclk per peripheral shrugman knows why
    GCLK->GENCTRL[6].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL) | GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL6);
    GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK6;
    // now the sercom
    while(SERCOM5->USART.SYNCBUSY.bit.ENABLE);
    SERCOM5->USART.CTRLA.bit.ENABLE = 0;
    while(SERCOM5->USART.SYNCBUSY.bit.SWRST);
    SERCOM5->USART.CTRLA.bit.SWRST = 1;
    while(SERCOM5->USART.SYNCBUSY.bit.SWRST);
    while(SERCOM5->USART.SYNCBUSY.bit.SWRST || SERCOM5->USART.SYNCBUSY.bit.ENABLE);
    // now reset and ready, do config
    SERCOM5->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE(1) | SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_TXPO(0);
    while(SERCOM5->USART.SYNCBUSY.bit.CTRLB);
    SERCOM5->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0);
    /*
    BAUD = 65536*(1-S*(fBAUD/fref))
    where S is samples per bit, 16 for async uart
    where fBAUD is the rate that you want
    where fref is the peripheral clock from GCLK, in this case (and most) 48MHz
    */

    /*

    RegBAUD = 

    115200 48,000,000 / 16 * 1 - 65536 *

    */
    // SERCOM5->USART.BAUD.reg = 45402;
    // SERCOM5->USART.BAUD.reg = 63018;
    SERCOM5->USART.BAUD.reg = 65326; // 9600 bps

// 63018 for 115200

    while(SERCOM5->USART.SYNCBUSY.bit.ENABLE);
    SERCOM5->USART.CTRLA.bit.ENABLE = 1;

    interrupts_setup();

/*
    while (1) {
        while(!SERCOM5->USART.INTFLAG.bit.DRE);
        SERCOM5->USART.DATA.reg = (uint8_t)170;
        // blink_LED();
        // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
    }
*/
}

void SysTick_Handler(void){
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // blink D6 / PA18
    // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
}

// we can then handle the interrupt:

void SERCOM5_1_Handler(void){
    // we must clear the flag, often this is done by reading the flag register, like this:
    // SERCOM4->USART->INTFLAG.bit.RXC = 1; // odd, writing '1' is normally the way to clear it
    // however, for this particular register, we read the data from the peripheral to clear the interrupt.
    // uint8_t data = SERCOM5->USART.DATA.reg;
    byte_seen = SERCOM5->USART.DATA.reg;
    // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
}

// TODO: make ATSAMD51 doc page for all of this bringup !

// caught:

// https://gitlab.cba.mit.edu/jakeread/atkstepper17/blob/master/embedded/mkstepper-v011.md
