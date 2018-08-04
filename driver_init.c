/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

struct usart_sync_descriptor USART_0a;

void USART_0a_PORT_init(void)
{

	gpio_set_pin_function(PB16, PINMUX_PB16C_SERCOM5_PAD0);

	gpio_set_pin_function(PB17, PINMUX_PB17C_SERCOM5_PAD1);
}

void USART_0a_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void USART_0a_init(void)
{
	USART_0a_CLOCK_init();
	usart_sync_init(&USART_0a, SERCOM5, (void *)NULL);
	USART_0a_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA14

	// Set pin direction to input
	gpio_set_pin_direction(D4, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(D4,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(D4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA18

	// Set pin direction to output
	gpio_set_pin_direction(D6, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(D6,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(D6, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA19

	// Set pin direction to output
	gpio_set_pin_direction(D9, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(D9,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(D9, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA21

	// Set pin direction to output
	gpio_set_pin_direction(D11, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(D11,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	gpio_set_pin_function(D11, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA23

	// Set pin direction to output
	gpio_set_pin_direction(D13, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(D13,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(D13, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB03

	// Set pin direction to output
	gpio_set_pin_direction(PB03, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PB03,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_function(PB03, GPIO_PIN_FUNCTION_OFF);

	// USART_0a_init();

	delay_driver_init();
}
