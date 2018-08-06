#include "sam.h"

#define PORTGROUP_A             0
#define PORTGROUP_B             1

// PA08 - RX
// PA10 - TX

#define RX_PIN  8
#define TX_PIN  10

#define PIO_SERCOM_ALT 3        // Peripheral Function D (3)

Sercom *sercom=SERCOM2; // use SERCOM5 definition found in "sercom.h"

#define UART_INT_CLOCK  0x1u    // UART uses internal clock generator when this define is used
#define SAMPLE_RATE_x16 0               // setup for 16 times oversampling

// defines for SAMD21
#define GCM_SERCOM0_CORE          (0x14U)
#define GCM_SERCOM1_CORE          (0x15U)
#define GCM_SERCOM2_CORE          (0x16U)
#define GCM_SERCOM3_CORE          (0x17U)
#define GCM_SERCOM4_CORE          (0x18U)
#define GCM_SERCOM5_CORE          (0x19U)

typedef enum
{
        UART_TX_PAD_0 = 0x0ul,  // Only for UART
        UART_TX_PAD_2 = 0x1ul,  // Only for UART
        UART_TX_RTS_CTS_PAD_0_2_3 = 0x2ul,  // Only for UART with TX on PAD0, RTS on PAD2 and CTS on
 PAD3
} SercomUartTXPad;

typedef enum
{
        SERCOM_RX_PAD_0 = 0,
        SERCOM_RX_PAD_1,
        SERCOM_RX_PAD_2,
        SERCOM_RX_PAD_3
} SercomRXPad;


void Uart_clock_init()
{
        // Start the Software Reset and wait for it to finish
        sercom->USART.CTRLA.bit.SWRST = 1 ;
        while ( sercom->USART.CTRLA.bit.SWRST || sercom->USART.SYNCBUSY.bit.SWRST )     ;

        // Turn on peripheral clock for SERCOM being used
        PM->APBCMASK.reg |= PM_APBCMASK_SERCOM2;

        //Setting clock
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_ID( GCM_SERCOM2_CORE )     |       // connected  SERCOMx to
                GCLK_CLKCTRL_GEN_GCLK0          |       // generic Clock Generator 0
                GCLK_CLKCTRL_CLKEN ;                    // and enable it

        while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ) ; // Wait for synchronization
}


void Uart_pin_init()
{
        PORT->Group[PORTGROUP_A].DIRCLR.reg = PORT_PA08;        // RX as input

        PORT->Group[PORTGROUP_A].DIRSET.reg = PORT_PA10;        // TX as output
        PORT->Group[PORTGROUP_A].OUTSET.reg = PORT_PA10;        // TX idle state is high

        // set port multiplexer for peripheral TX
        // =======================================
        uint32_t temp = (PORT->Group[PORTGROUP_A].PMUX[TX_PIN>>1].reg) & PORT_PMUX_PMUXO( PIO_SERCOM
_ALT );
        PORT->Group[PORTGROUP_A].PMUX[TX_PIN>>1].reg = temp | PORT_PMUX_PMUXE( PIO_SERCOM_ALT );

        PORT->Group[PORTGROUP_A].PINCFG[TX_PIN].reg = PORT_PINCFG_PMUXEN ; // Enable port mux

/*
        PORT->Group[PORTGROUP_A].PINCFG[RX_PIN].reg = PORT_PINCFG_PMUXEN ; // Enable port mux
*/


// is now
        temp = (PORT->Group[PORTGROUP_A].PMUX[RX_PIN>>1].reg) & PORT_PMUX_PMUXO( PIO_SERCOM_ALT );
        PORT->Group[PORTGROUP_A].PMUX[RX_PIN>>1].reg = temp | PORT_PMUX_PMUXE( PIO_SERCOM_ALT );
        PORT->Group[PORTGROUP_A].PINCFG[RX_PIN].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN ; // Enable port mux
}

void Uart_init(unsigned long baudrate)
{
        sercom->USART.CTRLA.reg =
                SERCOM_USART_CTRLA_DORD                                         |       // LSB_FIRST
                SERCOM_USART_CTRLA_TXPO(UART_TX_PAD_2)          |       // TX on Pad2
                SERCOM_USART_CTRLA_RXPO(SERCOM_RX_PAD_0)        |       // RX on Pad0
                SERCOM_USART_CTRLA_SAMPR(SAMPLE_RATE_x16)       |       // 16 times oversampling
                SERCOM_USART_CTRLA_RUNSTDBY                                     |       // Run in st
andby mode
                SERCOM_USART_CTRLA_MODE_USART_INT_CLK           ;       // Use internal clock


        // Asynchronous arithmetic mode
        // 65535 * ( 1 - sampleRateValue * baudrate / SystemCoreClock);
        // 65535 - 65535 * (sampleRateValue * baudrate / SystemCoreClock));
        sercom->USART.BAUD.reg = 65535.0f * ( 1.0f - (16.0 * (float)(baudrate)) / (float)(SystemCore
Clock));

        sercom->USART.CTRLB.reg =
                SERCOM_USART_CTRLB_CHSIZE(0)    |       // 8 bit character size
                SERCOM_USART_CTRLB_TXEN                 |       // Enable Transmit
                SERCOM_USART_CTRLB_RXEN                 ;       // Enable Receive

        // Get Synced
        while (sercom->USART.SYNCBUSY.bit.CTRLB) ;

        //Set the Interrupt to use
        sercom->USART.INTENSET.reg =
                SERCOM_USART_INTENSET_RXC       ;       // Interrupt on received complete

        // Enable interrupts
        NVIC_EnableIRQ(SERCOM2_IRQn);
        //      NVIC_SetPriority (SERCOM2_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  // set Priority

        // enable the peripheral block
        sercom->USART.CTRLA.bit.ENABLE = 0x1u;

        // Wait for sercom to enable
        while(sercom->USART.SYNCBUSY.bit.ENABLE) ;
}


void Uart_write(const uint16_t data)
{
        while(sercom->USART.INTFLAG.bit.DRE == 0) ; // wait for TX data empty
        sercom->USART.DATA.reg = data;
}


void SERCOM2_Handler()
{
        if (sercom->USART.INTFLAG.bit.RXC)
        {
                // Got a character
                uint16_t rxData = sercom->USART.DATA.reg;
                return;
        }
}


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

        Uart_clock_init();
        Uart_pin_init();
        Uart_init(57600);



        REG_PORT_DIRSET0 = PORT_PA11;

    /* Replace with your application code */
    while (1)
    {
                REG_PORT_OUTTGL0 = PORT_PA11;
                Uart_write('G');

                REG_PORT_OUTTGL0 = PORT_PA11;
                Uart_write('W');
    }
}
