#include "NuMicro.h"
#include <stdio.h>
#define LED PF15
#define RESET (48000-1)
/*
Common bit masks:
Bit Mask	                       Meaning
CLK_PWRCTL_HIRCEN_Msk	     Enable internal high-speed RC (HIRC)
CLK_PWRCTL_LIRCEN_Msk	     Enable internal low-speed RC (LIRC)
CLK_PWRCTL_HXTEN_Msk	     Enable external high-speed crystal (HXT)
CLK_PWRCTL_LXTEN_Msk	     Enable external low-speed crystal (LXT)

HCLK is CPU clock
CLK_PWRCTL_HXTEN_Msk for diving clock speed by number

*/

void SYS_Init(void){
	/* internal 48MHZ clock*/
	CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);  
	
	CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk); //wait for clock to stabilize and turn on
	
	CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk); //enable LIRC clock
	
	CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk); //wait for LIRC clock ready
	
	//CLK_setCoreClock only for external clock;
	
	/* divides PCLK0 used for UART0,I2C0 etc and PCLK1 to HCLK/2 due to max efficiency. some cant run at 
	full HCLK speed*/
	CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);
	
	CLK_EnableModuleClock(UART0_MODULE); //enables gate for UART0, every peripheral has seperate clock gate to save power
	
	CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1)); //use HIRC as uart clock 
	
}

void UART_Pin_Init(void){
	/*Gneral purpose port B_ Multifunctional*/
	SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk|SYS_GPB_MFPH_PB13MFP_Msk))|
	(SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
	
	UART_Open(UART0,115200);
}

void LED_Init(void){
	SYS->GPB_MFPH = (SYS->GPF_MFPH & ~SYS_GPF_MFPH_PF15MFP_Msk)|
	(SYS_GPF_MFPH_PF15MFP_GPIO);
	
	GPIO_SetMode(PF,BIT15,GPIO_MODE_OUTPUT);
	LED = 1;
}

void Systik_Initialize(){
	SysTick->VAL = 0;
	SysTick->LOAD=48000000-1;
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk    // Use processor clock
                   | SysTick_CTRL_ENABLE_Msk;      // Enable SysTick
	while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
		;
  SysTick->CTRL = 0;
}
int main(void){
	SYS_UnlockReg();
	SYS_Init();	
	SYS_LockReg();
	UART_Pin_Init();
	printf("GPIO LED");
	LED_Init();
	while (1){
		LED^=1;
		Systik_Initialize();
	}
}
