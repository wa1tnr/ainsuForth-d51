/*
 * tmc2130.c
 *
 * Created: 2/7/2018 10:17:39 PM
 *  Author: Jake
 */ 

#include "tmc2130.h"

void tmc2130_init(tmc2130_t *tmc, spiport_t *spi, pin_t *en, pin_t *diag0, pin_t *diag1){	
	tmc->spi = spi;
	tmc->en_pin = en;
	tmc->diag0 = diag0;
	tmc->diag1 = diag1;
}

void tmc2130_write(tmc2130_t *tmc, uint8_t address, uint32_t word){
	// SPI
	// TMC2130 is CSN Active Low, slave takes data on rising edge of clock, drives data out 'following the falling edge' - msb first.
	// TMC2130 takes a 40-bit (5 byte) word
	// 1st byte is address containing WRITE_notREAD bit, 1st byte shifted out is always status, useful
	// then 32 bit data in 4 bytes
	uint8_t bytes[5];
	bytes[0] = address;
	bytes[1] = word >> 24; // top 4 & mask for visibility
	bytes[2] = word >> 16; // middle 8
	bytes[3] = word >> 8;
	bytes[4] = word; // last 4 and 0's
	spi_txchars_polled(tmc->spi, bytes, 5);
}

void tmc2130_start(tmc2130_t *tmc){
	// off during config
	tmc2130_disable(tmc);
	
	// Write to GCONF
	uint8_t writeGConf = 0x00 | 0b10000000;
	// actually nothing we want, for now
	// 0b 
	uint32_t gConf = 0b000000000000000000;
	tmc2130_write(tmc, writeGConf, gConf);
	
	// this writes to IHOLD_IRUN register
	tmc2130_setCurrents(tmc, 12, 18);
		
	// Write CHOPCONF
	uint8_t writeChopConf = 0x6C | 0b10000000;
	// with internal AIN, we have 2.5V at AIN, effective max.
	//     bs,    mstep, pwmsync, bs, vsense, blank time, chopper mode, random toff, bs, hysteresis, hysteresis, toff,
	//  0b 0010   0100   0000     00  1       10          0             0            00  0100        000         0010
	// 0b 0010 0100 0000 00 1 01 0 0 00 0100 010 0010
	uint32_t chopConf = 0b00100100000000101000000010010010;
	tmc2130_write(tmc, writeChopConf, chopConf);
}

void tmc2130_setCurrents(tmc2130_t *tmc, uint8_t ihold, uint8_t irun){
	if(ihold > 31){
		ihold = 31;
	}
	if(irun > 31){
		irun = 31;
	}
	
	// Write IHOLD_IRUN settings
	uint8_t writeIHold_IRun = 0x10 | 0b10000000;
	// 1st is 0->31/32 IHold, 2nd is 0->31/32 IRun, 3rd is 0->15 Delay after standstill
	uint32_t IHold_IRun = ihold | ((uint32_t)irun << 8) | ((uint32_t)1 << 16);
	tmc2130_write(tmc, writeIHold_IRun, IHold_IRun);
}

void tmc2130_update(tmc2130_t *tmc){
	//uint32_t smarten = 0b10100000000000000000;
	//tmc2130_write(tmc, smarten);
}

void tmc2130_enable(tmc2130_t *tmc){
	pin_clear(tmc->en_pin);
}

void tmc2130_disable(tmc2130_t *tmc){
	pin_set(tmc->en_pin);
}