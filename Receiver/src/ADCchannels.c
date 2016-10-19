#include "ADUCRF101.h"

int main (void)
{
	
	/* FUNCTION OF THE PINS	
	P0.0 -> CH8
	P0.1 -> CH7
	P0.2 -> CH6
	P0.3 -> CH5
	P0.4 -> CH4
	P0.5 -> CH3
	P0.6 -> CH2
	P0.7 -> CH1
		
	P3.2 -> CH10
	P3.3 -> LED BATERRY GREEN
	P3.4 -> LED BATERRY RED
	P3.5 -> CH9
	
	P4.1 -> TALK
	P4.2 -> ALL CH'S
	*/	
	
  
	//Config the function of the pins like GPIO	
	pADI_GP0->GPCON=0x0000;
	pADI_GP3->GPCON=0x0550;
	pADI_GP4->GPCON=0x0014;
	
	//Config the pins like output 
	pADI_GP0->GPOEN=0xFF;
	pADI_GP3->GPOEN=0x3C;
	pADI_GP4->GPOEN=0x06;
	
	//Led's ON
	pADI_GP0->GPCLR=0xFF;
	pADI_GP3->GPCLR=0x3C;
	pADI_GP4->GPCLR=0x06;

  while(1){ }
}






