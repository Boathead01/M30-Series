#include "NuMicro.h"
#include <stdio.h>
#define LED PC3

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
	SYS->GPC_MFPL = (SYS->GPC_MFPL & ~SYS_GPC_MFPL_PC3MFP_Msk)|
	(SYS_GPC_MFPL_PC3MFP_GPIO);
	
	GPIO_SetMode(PC,BIT3,GPIO_MODE_OUTPUT);
	LED = 1;
}

int main(void){
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	UART_Pin_Init();
	printf("GPIO LED");
	LED_Init();
	while (1){
		LED=0;
		LED=1;
	}
}
