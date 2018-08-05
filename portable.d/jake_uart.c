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
    PORT->Group[1].DIRCLR.reg = (uint32_t)(1 << 17); // rx is input  PB17   //  0  17 PinMode
    PORT->Group[1].DIRSET.reg = (uint32_t)(1 << 16); // tx output    PB16   //  1  16 PinMode

    // aha moment 05 Aug 2018 04:28 UTC
    PORT->Group[1].PINCFG[17].bit.PMUXEN = 1;
    PORT->Group[1].PMUX[17>>1].reg |= PORT_PMUX_PMUXE(0x3);
    PORT->Group[1].PMUX[17>>3].reg |= PORT_PMUX_PMUXO(0x3);

    PORT->Group[1].PINCFG[16].bit.PMUXEN = 1;
    PORT->Group[1].PMUX[16>>1].reg |= PORT_PMUX_PMUXE(0x3);
    PORT->Group[1].PMUX[16>>3].reg |= PORT_PMUX_PMUXO(0x3);

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
    SERCOM5->USART.CTRLA.reg =
        SERCOM_USART_CTRLA_MODE(1) | 
        SERCOM_USART_CTRLA_DORD    |
        SERCOM_USART_CTRLA_RXPO(1) |
        SERCOM_USART_CTRLA_TXPO(0);

    while(SERCOM5->USART.SYNCBUSY.bit.CTRLB);

    SERCOM5->USART.CTRLB.reg =
        SERCOM_USART_CTRLB_RXEN |
        SERCOM_USART_CTRLB_TXEN |
        SERCOM_USART_CTRLB_CHSIZE(0);

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
    // SERCOM5->USART.BAUD.reg = 65326; //   9600 bps
    // SERCOM5->USART.BAUD.reg = 63018; // 115200 bps
    // SERCOM5->USART.BAUD.reg = 45402; // 921600 bps

    // SERCOM5->USART.BAUD.reg = 63018; // 115200 bps

    // wild baud and very slow:
    // SERCOM5->USART.BAUD.reg = 65529; // 300 bps perhaps
    SERCOM5->USART.BAUD.reg = 63018; // 115200 bps

    while(SERCOM5->USART.SYNCBUSY.bit.ENABLE);
    SERCOM5->USART.CTRLA.bit.ENABLE = 1;

    interrupts_setup();


#ifdef COMMENT_OUT_OMIT
    while (1) {
        while(!SERCOM5->USART.INTFLAG.bit.DRE);
        // SERCOM5->USART.DATA.reg   = (uint8_t)170; // presume this means send this byte out the port
        SERCOM5->USART.DATA.reg   = (uint16_t)0xa5; // was uint8_t in upstream but Arduino says 16_t here

        while(!SERCOM5->USART.INTFLAG.bit.TXC); // flush // not in jake's code

        // give debug hardware signals to human developer with oscope or human vision
        // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // blink  D6 / PA18
        // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
    }
#endif // #ifdef COMMENT_OUT_OMIT

/*
  uri https://raw.githubusercontent.com/arduino/ArduinoCore-samd/master/cores/arduino/SERCOM.cpp

int SERCOM::writeDataUART(uint8_t data)
{
  // Wait for data register to be empty
  while(!isDataRegisterEmptyUART());

  //Put data into DATA register
  sercom->USART.DATA.reg = (uint16_t)data;
  return 1;
}
*/

/*
void SERCOM::flushUART()
{
  // Skip checking transmission completion if data register is empty
  if(isDataRegisterEmptyUART())
    return;

  // Wait for transmission to complete
  while(!sercom->USART.INTFLAG.bit.TXC);
}
*/



}

void SysTick_Handler(void){
    // 9.5 millisecond tick -- divided by two for a 4.75 ms rate?
    PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // blink  D6 / PA18
    // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23
}

// we can then handle the interrupt:

void SERCOM5_1_Handler(void){
    // we must clear the flag, often this is done by reading the flag register, like this:
    // SERCOM4->USART->INTFLAG.bit.RXC = 1; // odd, writing '1' is normally the way to clear it
    // however, for this particular register, we read the data from the peripheral to clear the interrupt.
    // uint8_t data = SERCOM5->USART.DATA.reg;
    byte_seen = SERCOM5->USART.DATA.reg;
    // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 18); // blink  D6 / PA18
    // PORT->Group[0].OUTTGL.reg = (uint32_t)(1 << 23); // blink D13 / PA23

    // Yellow Wire - 250 uSec - five pulses for shift+U on keyboard, piUART sends this signala?
    // speed of PiUART is set to 9600 (by minicom -s 9600)

    // At 115200, this becomes 2.5 uSec (main time base of o-scope)

    // The yellow wire is the very last wire on the end of the PiUART away from the corner
    // (which makes it the most inboard wire on the RPi header pins)
    // This pin is labeled RX by Adafruit.  It connects to the RX pin
    // on a Feather microcontroller board.

    // Summary: byte_seen would be influenced by this pulse train
    // seen on the oscilloscope, if the program were functioning well.
}

// TODO: make ATSAMD51 doc page for all of this bringup !

// caught:

// https://gitlab.cba.mit.edu/jakeread/atkstepper17/blob/master/embedded/mkstepper-v011.md
