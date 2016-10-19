//leds.c

#include "leds.h"

/* OPTIONS:
	0 -> TALK
	
	1 -> CH1
	2 -> CH2
	3 -> CH3
	4 -> CH4
	5 -> CH5
	6 -> CH6
	7 -> CH7
	8 -> CH8
	9 -> CH9
	10 -> CH10

	11 -> ALL CHANNEL
*/

void leds(int option){
	switch (option){
		case 14:		
			//TALK -> P4.1
			pADI_GP4->GPSET|=GPSET_SET1_SET;		
		break;
		
		case 0:		
			//TALK -> P4.1
			pADI_GP4->GPCLR|=GPCLR_CLR1_CLR;		
		break;
		
		case 1:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH1 ON -> P4.0
			pADI_GP4->GPCLR=GPCLR_CLR0_CLR;			
		break;
		
		case 2:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH2 ON -> P0.6
			pADI_GP0->GPCLR=GPCLR_CLR6_CLR;	
		
		break;
		
		case 3:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH3 ON -> P0.5
			pADI_GP0->GPCLR=GPCLR_CLR5_CLR;				
		break;
		
		case 4:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P0.4
			pADI_GP0->GPCLR=GPCLR_CLR4_CLR;	
		break;
		
		case 5:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH5 ON -> P0.3
			pADI_GP0->GPCLR=GPCLR_CLR3_CLR;	
		break;

		case 6:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P0.2
			pADI_GP0->GPCLR=GPCLR_CLR2_CLR;	
		break;
	
		case 7:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P0.1
			pADI_GP0->GPCLR=GPCLR_CLR1_CLR;	
		break;
		
		case 8:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P0.0
			pADI_GP0->GPCLR=GPCLR_CLR0_CLR;	
		break;
		
		case 9:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P3.5
			pADI_GP3->GPCLR=GPCLR_CLR5_CLR;	
		break;
		
		case 10:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P3.2
			pADI_GP3->GPCLR=GPCLR_CLR2_CLR;	
		break;
		
		case 11:
			//SET ALL OFF
			pADI_GP0->GPSET=0xFF;
			pADI_GP3->GPSET=0x24;
			pADI_GP4->GPSET=0x07;
			//SET CH4 ON -> P4.2
			pADI_GP4->GPCLR=GPCLR_CLR2_CLR;	
		break;
		
		case 12:
			pADI_GP3->GPSET=GPSET_SET3_SET;
			//SET BATERRY LED RED ON -> P3.4
			pADI_GP3->GPCLR=GPCLR_CLR4_CLR;	
	  break;
		
		case 13:
			pADI_GP3->GPSET=GPSET_SET3_SET;
			//SET BATERRY LED GREEN ON -> P3.3
			pADI_GP3->GPCLR=GPCLR_CLR3_CLR;	
	  break;
			
	}
}
