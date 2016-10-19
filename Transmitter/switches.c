//switches.c

#include "switches.h"

unsigned int num_button;
unsigned int channel, channel_aux, state;
unsigned int state_talk=0;
unsigned int state_broadcast=0;
unsigned int talkOn=0;

extern short address;

void Ext_Int3_Handler () {	//TALK
	//Feed the Wacht dog
	pADI_WDT->T3CON=T3CON_ENABLE_DIS;			
	
	NVIC->ISER[0]|=ISER0_EXTINT3_DIS;
	
	num_button=0;
	
	if(state_talk==0){	//Is the frist time
		state_talk=1;
		leds(0);
		talkOn=1;
	}
	else{
		state_talk=0;
		talkOn=0;
		leds(14);
	}
	
	//Config the timer0	
	pADI_TM1->LD|=DELETE_REBOUND; // Interval of 1s
	pADI_TM1->CON|=T1CON_CLK_UCLK|T1CON_PRE_DIV256|T1CON_MOD_FREERUN|T1CON_ENABLE_EN;
	NVIC->ISER[0]|=ISER0_T1_EN;
}
			
void Ext_Int4_Handler () {	//ALL
	//Feed the Wacht dog
	pADI_WDT->T3CON=T3CON_ENABLE_DIS;				
	
	NVIC->ISER[0]|=ISER0_EXTINT4_DIS;
	
	address=B_ON;
	num_button=1;

	if ((state_broadcast==0)&(state_talk==0)){	//Is the frist time
		state_broadcast=1;
		leds(11);
	} else if ((state_broadcast==1)&(state_talk==0)){
		state_broadcast=0;
		leds(channel);
	}
	
	//Config the timer1	
	pADI_TM1->LD|=DELETE_REBOUND; // Interval of 1s
	pADI_TM1->CON|=T1CON_CLK_UCLK|T1CON_PRE_DIV256|T1CON_MOD_FREERUN|T1CON_ENABLE_EN;
	NVIC->ISER[0]|=ISER0_T1_EN;	
}

void Ext_Int5_Handler ()	//+
{
	//Feed the Wacht dog
	pADI_WDT->T3CON=T3CON_ENABLE_DIS;			
	
	NVIC->ISER[0]|=ISER0_EXTINT5_DIS;	
	
	num_button=2;
	
	if(state_broadcast==0&(state_talk==0)){
		if (channel==10)
			channel=1;
		else
			channel=channel+1;
		leds(channel);
		switch (channel){
			case 1:
				address=CH1;
			break;
			case 2:
				address=CH2;
			break;
			case 3:
				address=CH3;
			break;
			case 4:
				address=CH4;
			break;
			case 5:
				address=CH5;
			break;
			case 6:
				address=CH6;
			break;
			case 7:
				address=CH7;
			break;
			case 8:
				address=CH8;
			break;
			case 9:
				address=CH9;
			break;
			case 10:
				address=CH10;
			break;
		}
	}
	
	//Config the timer0	
	pADI_TM1->LD|=DELETE_REBOUND; // Interval of 1s
	pADI_TM1->CON|=T1CON_CLK_UCLK|T1CON_PRE_DIV256|T1CON_MOD_FREERUN|T1CON_ENABLE_EN;
	NVIC->ISER[0]|=ISER0_T1_EN;

}

void Ext_Int6_Handler ()	//-
{
	//Feed the Wacht dog
	pADI_WDT->T3CON=T3CON_ENABLE_DIS;			
	
	NVIC->ISER[0]|=ISER0_EXTINT6_DIS;
	
	num_button=3;
	
	if((state_broadcast==0)&(state_talk==0)){
		if (channel==1)
			channel=10;
		else
			channel=channel-1;
		leds(channel);
		switch (channel){
			case 1:
				address=CH1;
			break;
			case 2:
				address=CH2;
			break;
			case 3:
				address=CH3;
			break;
			case 4:
				address=CH4;
			break;
			case 5:
				address=CH5;
			break;
			case 6:
				address=CH6;
			break;
			case 7:
				address=CH7;
			break;
			case 8:
				address=CH8;
			break;
			case 9:
				address=CH9;
			break;
			case 10:
				address=CH10;
			break;
		}

	//Config the timer0	
	pADI_TM1->LD|=DELETE_REBOUND; // Interval of 1s
	pADI_TM1->CON|=T1CON_CLK_UCLK|T1CON_PRE_DIV256|T1CON_MOD_FREERUN|T1CON_ENABLE_EN;
	NVIC->ISER[0]|=ISER0_T1_EN;
	}
}


void GP_Tmr1_Int_Handler ()
{
	pADI_TM1->CON|=TCON_ENABLE_DIS;		//Reset the counter
	NVIC->ICER[0]=ICER0_T1_EN ;				//Disalbe the interrup
	
	/* 0 -> TALK
		 1 -> ALL
	   2 -> +
	   3 -> -  		*/

		switch (num_button){ 
			case 0:
				pADI_INTERRUPT->EICLR=EICLR_IRQ3;	
				NVIC->ISER[0]|=ISER0_EXTINT3_EN;
				break;
			
			case 1:
				pADI_INTERRUPT->EICLR=EICLR_IRQ4;	
				NVIC->ISER[0]|=ISER0_EXTINT4_EN;
				break;
			
			case 2:
				pADI_INTERRUPT->EICLR=EICLR_IRQ5;	
				NVIC->ISER[0]|=ISER0_EXTINT5_EN;	
				break;
			
			case 3:
				pADI_INTERRUPT->EICLR=EICLR_IRQ6;	
				NVIC->ISER[0]|=ISER0_EXTINT6_EN;	
				break;			
		}
}


//Function to config the External Interrup of the switches
void Config_External_Interrup(void)
{	
	/* FUNCTIONS OF THE BUTTONS
	P0.7 -> IRQ3 -> BUTTON TALK
	P1.0 -> IRQ4 -> BUTTON ALL
	P1.4 -> IRQ5 -> BUTTON +
	P1.5 -> IRQ6 -> BUTTON -
	*/
	
	//Configure the interruptions like rise fall.
	pADI_INTERRUPT->EI0CFG|=EI0CFG_IRQ3MDE_FALL|EI0CFG_IRQ3EN;
	pADI_INTERRUPT->EI1CFG|=EI1CFG_IRQ4MDE_FALL|EI1CFG_IRQ4EN;
	pADI_INTERRUPT->EI1CFG|=EI1CFG_IRQ5MDE_FALL|EI1CFG_IRQ5EN;
	pADI_INTERRUPT->EI1CFG|=EI1CFG_IRQ6MDE_FALL|EI1CFG_IRQ6EN;

	pADI_INTERRUPT->EICLR|=EICLR_IRQ3|EICLR_IRQ4|EICLR_IRQ5|EICLR_IRQ6;
		
	NVIC->ISER[0]|=ISER0_EXTINT3_EN|ISER0_EXTINT4_EN|ISER0_EXTINT5_EN|ISER0_EXTINT6_EN;
}

