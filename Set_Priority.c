#include "NuMicro.h"
#include <stdio.h>
#define LED PC3

void Set_Priority(){
	NVIC_SetPriority(GPIO_PAPB_IRQn, 0xFFFFFF3F); //u can also give 0 instead of bit pattern, 0 is the highest prioirty and 3 is the lowest
}
//acc to datasheet :
//    		. Note that priority “0” is treated as the fourth priority on the system, after three system exceptions “Reset”, “NMI” and “Hard Fault”.
// those three interrupts are in -1 -2 -3, the highest priority and are fixed, so no chance of making our pririty the first , as in -3.
volatile int sw1_interface =0;
int sw1_cnt=0;


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

void Button_Init(void){
	SYS->GPB_MFPL = (SYS->GPB_MFPL & ~(SYS_GPB_MFPL_PB4MFP_Msk))|
	(SYS_GPB_MFPL_PB4MFP_GPIO);
	GPIO_SetMode(PB,BIT4,GPIO_MODE_INPUT);
	GPIO_SetMode(PB,BIT4,GPIO_INT_FALLING); //falling edge interrupt, button is connected to ground
	NVIC_EnableIRQ(GPIO_PAPB_IRQn);
	GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_512); //512 clock cycles for debouce func
  GPIO_ENABLE_DEBOUNCE(PB, BIT4);
	
}
int main(void){
	
	SYS_UnlockReg();
	SYS_Init();
	SYS_LockReg();
	UART_Pin_Init();
	printf("GPIO LED");
	LED_Init();
	Button_Init();
	Set_Priority();
	while (1){
		if (sw1_interface){
			printf("no of times button pressed:%d\n",sw1_cnt);
			sw1_interface=0;
		}
	}
}

void GPABGH_IRQHandler(void){           // not GPIO_PAPB_IRQHandler
	if (GPIO_GET_INT_FLAG(PB,BIT4)){
		sw1_interface=1;
		LED = 0;
		sw1_cnt++;
		GPIO_CLR_INT_FLAG(PB,BIT4);
		LED=1;
	}
}
