//set GPIO input 
void GPIO_Init(void){
	GPIO_SetMode(PC, BIT3, GPIO_MODE_INPUT);
}
//                      (or)
void GPIO_Init(void){
	GPIO_SetMode(PC, BIT3, 0x3FFF3FBF);  //see through datasheet
}

//set speed (T_rise and T_fall) -> applicable only for output mode
// acc to datasheet, for M031, it is not available. 
//see for GPIO speed control registers later, in M032 case it isn't available
