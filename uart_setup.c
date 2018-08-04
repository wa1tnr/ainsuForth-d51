/* uart_init.c - local split from Jake Read's code */
/* import jake_main.c and remove unwanted code */

/* mkstepper-v011.c * * Created: 2/5/2018 12:00:35 PM * Author : Jake */ 

#include "sam.h"
#include "pin.h"
#include "uartport.h"
// #include "spiport.h"
#include "apaport.h"
#include "hardware.h"

/* PINS */
// void uart_init(void); // prototype

void uart_setup(void) // int main(void)
{
   // ainsu: Jake claims PB14 and PB13 GPIO
	
	// ready interrupt system
	__enable_irq();
	NVIC_EnableIRQ(SERCOM5_0_IRQn); // tx
	NVIC_EnableIRQ(SERCOM5_1_IRQn);
	
	// ringbuffers (for uart ports)
	// rb_init(&up0_rbrx);
	// rb_init(&up0_rbtx);
	
	// uarts (ports)
        // ainsu: Jake claims PA12 PA13 for UART
	
        // ainsu: Jake claims PB3 PB2 for UART
	up0 = uart_new(SERCOM5, &PORT->Group[1], &up0_rbrx, &up0_rbtx, 3, 2, HARDWARE_IS_APBD, HARDWARE_ON_PERIPHERAL_D);
	MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5;
	uart_init(&up0, 7, SERCOM5_GCLK_ID_CORE, 62675);
	
	ups[0] = &up0;
	
        // ainsu: Jake claims PA19
        // ainsu: Jake claims PA17
        // ainsu: Jake claims PA21
        // ainsu: Jake claims PA23
    // while (1) { }
}

// ainsu theory: tick handler initiates traffic and may be required or useful
// void SysTick_Handler(void){
	//uart_sendchar_buffered(&up0, 120);
// }

void SERCOM5_0_Handler(void){
	uart_txhandler(&up0);
}

void SERCOM5_1_Handler(void){
	uart_rxhandler(&up0);
}

