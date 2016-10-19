//output.c

#include "ADUCRF101.h"
#include "leds.h"
#include "batery.h"
#include "output.h"
#include "core_cm3.h"
#include "math.h"

#include <stdlib.h>  
#include <stddef.h>                 
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "defs.h"
#include "radioeng.h"
#include "OutDAC.h"

#define FS		0x385
#define NS	118
#define TSPS 0x6;
#define PACKETRAM_START        0x10

short sample, address;
int i=0;

int DAC_ON=0;
int aux;

int j=900;

short channel= 0x0001;
short broadcast=0xFFFF;

short arrayStored1[39];
short arrayStored2[39];
short arrayUsed[39];



void Config_Timer0 (){
	//Config the timer0	
	pADI_TM0->LD|=TSPS; 
	pADI_TM0->CON|=T0CON_CLK_UCLK|T0CON_PRE_DIV1|T0CON_MOD_FREERUN|T0CON_ENABLE_EN;
	NVIC->ISER[0]|=ISER0_T0_EN;	
}

void GP_Tmr0_Int_Handler ()
{ 
	NVIC->ICER[0]=ICER0_T0_EN ;				//Disalbe the interrup

	if (RadioRxPacketAvailable())	{
		RadioReadShort(PACKETRAM_START+ 0, &address);
		
		if((address==channel)||(address==broadcast)){
			j=0;
			leds(3);
			DAC_ON=1;
			RadioReadShort(PACKETRAM_START+ 2*(i+1), &sample);
			arrayUsed[i]=sample;
			RadioReadShort(PACKETRAM_START+ (2*(i+1)+39), &sample);
			arrayStored1[i]=sample;
			RadioReadShort(PACKETRAM_START+ (2*(i+1)+39*2), &sample);
			arrayStored2[i]=sample;
			i++;			
						
			if(i==39){
				RadioToReceiveMode(118*2);
				i=0;
				leds(2);
			}
		}	
	}
	if(DAC_ON==1){
		if(j<39)
			aux=arrayUsed[j];
		if((j>=39)&&(j<78))
			aux=arrayStored1[j-39];		
		if((j>=78)&&(j<118))
			aux=arrayStored1[j-39*2];
		j++;
		if(j==118){
			j=900;
			DAC_ON=0;
		}
			
		pADI_GP4->GPSET=GPSET_SET0_SET;

		OutDAC(aux);
		pADI_GP4->GPCLR=GPCLR_CLR0_CLR;
	
	}
	
	pADI_TM0->CON|=TCON_ENABLE_DIS;	
}






