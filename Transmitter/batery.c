//batery.c

#include "batery.h"

short ADC_value=0;
	
void batery (void){
	
	int i;	
	pADI_ADC0->ADCCFG = ADCCFG_REF_LVDD			// LVDD as reference
                     |ADCCFG_CLK_FCOREDIV2    // ADC clk/32
                     |ADCCFG_ACQ_16    		// 16 aqu-clk
                     |ADCCFG_CHSEL_ADC1;  // ADC 1
	pADI_ADC0->ADCCON = ADCCON_REFBUF_EN    // REF On
                     |ADCCON_IEN_DIS      // no IRQ
                     |ADCCON_ENABLE_EN    // power-up ADC
                     |ADCCON_MOD_CONT    // cont. conversion
                     |ADCCON_START_EN
										 |ADCCON_START ;    // start ADC				
		
	while (!pADI_ADC0->ADCSTA&ADCSTA_READY) {} // wait for conversion to complete
	for(i=0;i<20;i++);		
	ADC_value=pADI_ADC0->ADCDAT;
			
	if(ADC_value<=10000){
		leds(12);		//RED
	}
	else{
		leds(13);	//GREEN
	}     
}

