//main.c
#include <stdlib.h>  
#include <stddef.h>                 
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "defs.h"

#include "ADUCRF101.h"
#include "leds.h"
#include "batery.h"
#include "radioeng.h"
#include "core_cm3.h"
#include "output.h"
#include "config.h"


#define NS 										 119
#define PACKETRAM_START        0x10
#define FS		0x385

/* Main Program Start Here */
int main (void){
		
	//INITIALIZE THE RADIO
	RIE_Responses RIE_Response = InitRadioReceive();
	RadioSetModulationType(GFSK_Modulation);	//Select GFSK modulation

	ConfigPins();
	
	//CONFIG THE WACHT DOG
  pADI_WDT->T3CON=T3CON_PD_DIS|T3CON_IRQ_EN|T3CON_PRE_DIV1|0x40;
	
	batery();
	
	if (RIE_Response == RIE_Success)
		RIE_Response = RadioRxPacketFixedLen(2*NS);

	while(1){
		//Feed the Wacht dog
		pADI_WDT->T3CON=T3CON_ENABLE_DIS;		
		
		//Establish the slot of time at 112.5 usec
		Config_Timer0 ();
		
	}

}
