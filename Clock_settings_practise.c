/* note, 72 MHz high frequency can be obtained; it is given in datasheet pg:316*/
/* always refer to max limit of AHB bus instead of max range of input HXT, input range can't be always accepted as HCLK range also*/
/* if for APB/PCLK, no range is given, it is safe to assume PCLK range extends till HCLK range */
/* system clock is HCLK*/

/* Q1.  enable 72MHz of external high frequency clock */

void SYS_Init(void){
	CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
	while (!CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk));
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));
}

/* since there is no implicit functions for PCLK, u can only modify PCLK individually through protocols */

/*Q2. enable 45MHz of internal clock high freqeuncy for HCLK, and 72/4 MHz for UART communication frequency*/ 
void SYS_Init(void){
	CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
	while (!CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk));
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));
	CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, 1);
	CLK_EnableModuleClock(UART0_MODULE);
}
