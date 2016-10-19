//config.c
#include "ADUCRF101.h"
#include "core_cm3.h"
#include "config.h"

void ConfigPins (void){
		
		/* FUNCTION OF THE PINS	
		P0.0 -> CH8
		P0.1 -> CH7
		P0.2 -> CH6
		P0.3 -> CH5
		P0.4 -> CH4
		P0.5 -> CH3
		P0.6 -> CH2
		
			
		P3.2 -> CH10
		P3.3 -> LED BATERRY GREEN
		P3.4 -> LED BATERRY RED
		P3.5 -> CH9
		
		P4.0 -> CH1
		P4.1 -> TALK
		P4.2 -> ALL CH'S
		
		*/	
		
		//Config the function of the pins like GPIO	
		pADI_GP0->GPCON|=0x0000;
		pADI_GP3->GPCON|=0x0550;
		pADI_GP4->GPCON|=0x0015;
		
		//Config the pins like output 
		pADI_GP0->GPOEN|=0xFF;
		pADI_GP3->GPOEN|=0x3C;
		pADI_GP4->GPOEN|=0x07;
				
		//Led's OFF
		pADI_GP0->GPSET=0xFF;
		pADI_GP3->GPSET=0x3C;
		pADI_GP4->GPSET=0x07;
		
		//CONFIG EXTERNAL INTERRUPS
		pADI_GP0->GPCON|=GP0CON_CON7_GPIOIRQ3;
		pADI_GP1->GPCON|=GP1CON_CON0_GPIOIRQ4;
		pADI_GP1->GPCON|=GP1CON_CON4_GPIOIRQ5;
		pADI_GP1->GPCON|=GP1CON_CON5_GPIOIRQ6;


	pADI_GP4->GPCON|=GP4CON_CON6_GPIO;
	pADI_GP4->GPOEN|=GPOEN_OEN6_OUT;
	pADI_GP4->GPCLR=GPCLR_CLR6_CLR;


}

