//leds.c


#include "ADUCRF101.h"
#include "leds.h"
#include "batery.h"
#include "core_cm3.h"


void leds(int option){
	switch (option){
		case 0:
			pADI_GP3->GPSET=GPSET_SET3_SET;
			//SET BATERRY LED RED ON -> P3.4
			pADI_GP3->GPCLR=GPCLR_CLR4_CLR;	
	  break;
		
		case 1:
			pADI_GP3->GPSET=GPSET_SET3_SET;
			//SET BATERRY LED GREEN ON -> P3.3
			pADI_GP3->GPCLR=GPCLR_CLR3_CLR;	
	  break;
		
		case 2:
			//SET LED OFF
			pADI_GP4->GPSET=GPSET_SET2_SET;
	  break;
		
		case 3:
			//SET LED ON
			pADI_GP4->GPCLR=GPCLR_CLR2_CLR;	
	  break;
	}
}
