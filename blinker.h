// blinker.c ainsuForth D51

// #include "atmel_start.h"
// #include "gpio_local.h"
// #include "uartport.h"
// #include "hardware.h"

/* Many changes: by wa1tnr, July 2018 */


/*
void uSec(void) { }
*/

extern void short_timer(void);
extern void raise_LED_pins(void);

/*

void lower_LED_pins(void) { }

void blink_LED(void) { }

*/
extern void flicker_LED(void);
/*

void blinkLEDfast(void) { }

void activity_LED_demo(void) { }

void long_long_timer(void) { }
*/
