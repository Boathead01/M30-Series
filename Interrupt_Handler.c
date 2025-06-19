//there is a change here. while using INTSRC, ur code will work but CPU automatically goes to search for interrupt handler
//so by this u can manually tune the cpu to focus on PB->INTSRC, but that will take time, so time efficiency reduces

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
	while (1){
		if (PB->INTSRC & (1<<4)){
			PB->INTSRC=(1<<4);
			LED=0;
			sw1_cnt++;
			printf("no of times button pressed:%d\n",sw1_cnt);
			delay();
			LED=1;
		}
	}
}

