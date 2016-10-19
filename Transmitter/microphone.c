//microphone.c

#include "microphone.h"
#include "radioeng.h"

int num;
short ADC_voice=0;	
short address;

void Config_Timer0 (){
	//Config the timer0	
	pADI_TM0->LD|=TSPS; 
	pADI_TM0->CON|=T0CON_CLK_UCLK|T0CON_PRE_DIV1|T0CON_MOD_FREERUN|T0CON_ENABLE_EN;
	NVIC->ISER[0]|=ISER0_T0_EN;	
}

void GP_Tmr0_Int_Handler ()
{ 
	NVIC->ICER[0]=ICER0_T0_EN ;				//Disalbe the interrup

  pADI_ADC0->ADCCFG = ADCCFG_REF_LVDD			// LVDD as reference
                     |ADCCFG_CLK_FCOREDIV2    // ADC clk/32
                     |ADCCFG_ACQ_16    		// 16 aqu-clk
                     |ADCCFG_CHSEL_ADC0;  // ADC 0

	pADI_ADC0->ADCCON = ADCCON_REFBUF_EN    // REF On
                     |ADCCON_IEN_DIS      // no IRQ
                     |ADCCON_ENABLE_EN    // power-up ADC
                     |ADCCON_MOD_SOFT     // cont. conversion
                     |ADCCON_START_EN
										 |ADCCON_START ;    // start ADC				
	
	while (!pADI_ADC0->ADCSTA&ADCSTA_READY) {} // wait for conversion to complete
	ADC_voice=pADI_ADC0->ADCDAT;
	
	if(num==0){
	//Copy the address in the frist position of memory
		RadioCopyShort(PACKETRAM_START+ 2*num, &address);
		num++;
	}

	RadioCopyShort(PACKETRAM_START+ 2*num, &ADC_voice);

	num ++;
	
	if(num==NS){
					pADI_GP4->GPSET=GPSET_SET6_SET;

		RadioSend(238);	//118*2+2
	pADI_GP4->GPCLR=GPCLR_CLR6_CLR;

		num=0;
	}
	pADI_TM0->CON|=TCON_ENABLE_DIS;	
}
