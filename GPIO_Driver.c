#include <stdint.h>
#include "NuMicro.h"
#include <stdio.h>


// debounce structure
typedef struct{
	//pointer to hold base address GPIO peripheral
	uint32_t *PA_base_addr;
	uint32_t *PA_Enable;
	uint32_t *PA_Disable;
	uint32_t *CLK_PA_Select;
}GPIOA_Debounce;

//interrupt trigger 
typedef struct{
	volatile uint32_t *PA_base_addr;
	volatile uint32_t *PA_type;
}GPIOA_Interrupt;


/******************************************************
* @fn   								- GPIOA interrupt
*
* @brief								- enables the interrupt for PA
*
* @param[in]						- rf refers to rising edge or falling edge
*													0 - falling edge
*													1 - rising edge
*
* @param[in]						- specify the pin number of port A
*
*	@return 							- returns Enable bybe set
*
* @note 								- enables the interrupt of PA alone
********************************************************/


volatile uint32_t* GPIOA_Interrupt_Enable(uint8_t rf, uint32_t u32Pin){
	GPIOA_Interrupt Modes;
	Modes.PA_base_addr = (volatile uint32_t*)0x4000401C;
	if (rf == 1)
		*Modes.PA_base_addr |= (1<< (u32Pin+16));
	else
		*Modes.PA_base_addr |= (1<< (u32Pin));
	return Modes.PA_base_addr;
}

/******************************************************
* @fn   								- GPIOA interrupt
*
* @brief								- disables the interrupt for PA
*
* @param[in]						- rf refers to rising edge or falling edge
*													0 - falling edge
*													1 - rising edge
*
* @param[in]						- specify the pin number of port A
*
*	@return 							- returns address pointer byte
*
* @note 								- diables the interrupt of PA alone
********************************************************/


volatile uint32_t *GPIOA_Interrupt_Disable(uint8_t rf, uint32_t u32Pin){
	GPIOA_Interrupt Modes;
	Modes.PA_base_addr = (volatile uint32_t*)0x4000401C;
	if (rf == 1)
		*Modes.PA_base_addr |= (1<< (u32Pin+16));
	else
		*Modes.PA_base_addr |= (1<< (u32Pin));
	return Modes.PA_base_addr;
}



/******************************************************
* @fn   								- GPIOA interrupt
*
* @brief								- enables the interrupt for PA
*
* @param[in]						- enables the type of interrupt
*													0 - Edge trigger interrupt.
*													1 - Level trigger interrupt.
*
*	@return 							- bit pattern 
*
* @note 								- enables the interrupt of PA alone
********************************************************/


volatile uint32_t* GPIOA_Interrupt_type(uint32_t val,uint32_t u32Pin){
	GPIOA_Interrupt Modes;
	Modes.PA_base_addr = (volatile uint32_t*)0x40004018;
	*Modes.PA_base_addr |= (val<<u32Pin);
	return Modes.PA_base_addr;
}
