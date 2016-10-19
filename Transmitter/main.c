//main.c

#include "ADUCRF101.h"
#include "core_cm3.h"
#include "config.h"
#include "leds.h"
#include "switches.h"
#include "radioeng.h"
#include "microphone.h"
#include "batery.h"


//Variables
extern unsigned int channel;
extern unsigned int talkOn;
extern short address;

/* Main Program Start Here */
int main (void){
	
	ConfigPins();
	Config_External_Interrup();	

	//CONFIG THE WACHT DOG
  pADI_WDT->T3CON=T3CON_PD_DIS|T3CON_IRQ_EN|T3CON_PRE_DIV1|0x40;
	
	//CONFIG THE VARIABLES
	channel=1;
	leds(channel);
	address=0x0001;
	
	//INITIALIZE THE RADIO, TRANSMIT SIDE
	InitRadioTransmit();
	RadioSetModulationType(GFSK_Modulation);	//Select GFSK modulation
	
	//Check the level of the battery
	batery();	
	
	while(1){
		//Feed the Wacht dog
		pADI_WDT->T3CON=T3CON_ENABLE_DIS;		
		
		//Check if the button TALK is ON to send the info
		if(talkOn==1){
			Config_Timer0();
		}	
	}
}
