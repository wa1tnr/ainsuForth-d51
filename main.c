// main.c ainsuForth D51

// has clock_init() but nothing for the uart - 04:44 UTC 04 AUG 2018

#include "sam.h"
#include "atmel_start.h"
#include "clock_init.h"
#include "led_pulse.h"
#include "uart_setup.h"
#include "gpio_local.h"

extern void jake_uart(void); // no .h file haha

/* Many changes: by wa1tnr, July 2018 */

int main(void) {
    SystemInit();
    clock_init();
    init_act_LED();
    SysTick_Config(5000000);

    jake_uart();

    while(1);

    led_pulse();
    while (1) {
        // none
    }
}
