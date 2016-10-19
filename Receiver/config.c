//config.c

#include "ADUCRF101.h"
#include "core_cm3.h"
#include "config.h"

void ConfigPins (void) {
	/* FUNCTION OF THE PINS	
	P3.3 -> LED BATERRY GREEN
	P3.4 -> LED BATERRY RED7
	
	P4.2 -> LED

	P0.0 -> LSB
	P0.1 -> 1
	P0.2 -> 2
	P0.3 -> 3
	P0.4 -> 4
	P0.5 -> 5
	P0.6 -> 6
	P0.7 -> 7
	
	P1.0 -> 8
	P1.1 -> 9
	P1.2 -> 10
	P1.3 -> 11
	P1.4 -> 12
	P1.5 -> MSB
	*/	
	
	//Config the function of the pins like GPIO	
	pADI_GP0->GPCON|=0x0000;
	pADI_GP1->GPCON|=0x0054;
	
	pADI_GP3->GPCON|=GP3CON_CON4_GPIO|GP3CON_CON3_GPIO;
	pADI_GP4->GPCON|=GP4CON_CON2_GPIO;
	
	//Config the pins like output 
	pADI_GP0->GPOEN|=0xFF;
	pADI_GP1->GPOEN|=0x3F;
	pADI_GP3->GPOEN|=GPOEN_OEN3_OUT|GPOEN_OEN4_OUT;
	pADI_GP4->GPOEN|=GPOEN_OEN2_OUT;

	//Output OFF
	pADI_GP0->GPCLR|=0xFF;
	pADI_GP1->GPCLR|=0x3F;
	
	//Led's OFF
	pADI_GP3->GPSET=GPSET_SET3_SET|GPSET_SET4_SET;
	pADI_GP4->GPSET=GPSET_SET2_SET;
	
	pADI_GP4->GPCON|=GP4CON_CON0_GPIO;
	pADI_GP4->GPOEN|=GPOEN_OEN0_OUT;
	pADI_GP4->GPCLR=GPCLR_CLR0_CLR;
}
