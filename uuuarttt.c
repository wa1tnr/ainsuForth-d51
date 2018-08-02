
int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

    //clock_init();

    /*setup blinking
    STLR        PB13
    STLB        PB14
    */
    PORT->Group[1].DIRSET.reg |= (uint32_t)(1 << 13);
    PORT->Group[1].DIRSET.reg |= (uint32_t)(1 << 14);
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 13);
    PORT->Group[1].OUTSET.reg |= (uint32_t)(1 << 14);

    SysTick_Config(5000000);

    /* setup UARTs
    NP1RX       PA12 / SER4-1
    NP1TX       PA13 / SER4-0

    NP2RX       PB03 / SER5-1 / Peripheral D
    NP2TX       PB02 / SER5-0 / Peripheral D
    */

    // setup pins for peripheral
    PORT->Group[1].DIRCLR.reg = (uint32_t)(1 << 3); // rx is input
    PORT->Group[1].DIRSET.reg = (uint32_t)(1 << 2); // tx output
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
    SERCOM5->USART.BAUD.reg = 45402;
    while(SERCOM5->USART.SYNCBUSY.bit.ENABLE);
    SERCOM5->USART.CTRLA.bit.ENABLE = 1;


    while (1) 
    {
        while(!SERCOM5->USART.INTFLAG.bit.DRE);
        SERCOM5->USART.DATA.reg = (uint8_t)170;
    }
}

void SysTick_Handler(void){
    PORT->Group[1].OUTTGL.reg = (uint32_t)(1 << 14); // blink STLB
}

