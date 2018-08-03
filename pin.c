/*
 * pin.c
 *
 * Created: 2/5/2018 11:21:37 PM
 *  Author: Jake
 */ 

/*
 * ainsuForth_D51 notice:
 *
 * outside project files are from Jake Read.
 *
 * mkstepper-v011.c
 *
 * Created: 2/5/2018 12:00:35 PM
 * Author : Jake
 * https://gitlab.cba.mit.edu/jakeread/atkstepper17/tree/e6559372a14fa69ba360da903480d962d786744f
 *
 * https://gitlab.cba.mit.edu/jakeread/atkstepper17/blob/e6559372a14fa69ba360da903480d962d786744f/embedded/mkstepper-v011/mkstepper-v011/main.c
 *
 */

#include "pin.h"
#include "sam.h"

pin_t pin_new(PortGroup *port, uint32_t pin_number){
	pin_t pin;
	pin.port = port;
	pin.bm = (uint32_t)(1 << pin_number);
	return pin;
}


void pin_output(pin_t *pin){
	pin->port->DIRSET.reg = pin->bm;
	pin->port->OUTCLR.reg = pin->bm;
}

void pin_input(pin_t *pin){
	pin->port->DIRCLR.reg = pin->bm;
}

void pin_set(pin_t *pin){
	pin->port->OUTSET.reg = pin->bm;
}

void pin_clear(pin_t *pin){
	pin->port->OUTCLR.reg = pin->bm;
}

void pin_toggle(pin_t *pin){
	pin->port->OUTTGL.reg = pin->bm;
}

int pin_read(pin_t *pin){
	return pin->port->IN.reg & pin->bm;
}
