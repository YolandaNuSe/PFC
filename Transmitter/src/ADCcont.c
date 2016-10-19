/**
 *****************************************************************************
   @example  ADCcont.c
   @brief    Demonstrate the use of the ADC in continuous mode.
             Conversion done on ADC0 channel, connected to the potentionmeter.
             The potentiometer is connected between LVDD and P3.4, driving 0 out.
   @version  V0.1
   @author   PAD CSE group
   @date     February 2012 

All files for ADuCRF101 provided by ADI, including this file, are
provided  as is without warranty of any kind, either expressed or implied.
The user assumes any and all risk from the use of this code.
It is the responsibility of the person integrating this code into an application
to ensure that the resulting application performs as required and is safe.

**/
#include "include.h"

volatile unsigned short usAdcResult;
unsigned char ucTimeout;

int main()
{ 
  WdtGo(T3CON_ENABLE_DIS);
   
  // ADC initialisation, using GP timer 1.
  AdcPd(ADCCON_ENABLE_EN);       // Enable ADC and reference
  ucTimeout = 0;
  GptLd (pADI_TM1, 0x9C); // wait ~2.5ms
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE|TCON_MOD_PERIODIC);
  GptClrInt(pADI_TM1,TCLRI_TMOUT);
  NVIC_EnableIRQ(TIMER1_IRQn);
  while (!ucTimeout){}
  ucTimeout = 0;
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE_DIS|TCON_MOD_PERIODIC);
  AdcInit(ADCCON_REFBUF_EN,  ADCCON_IEN_DIS, ADCCON_DMA_DIS);  // Enable reference buffer  
  GptLd (pADI_TM1, 0x79); // wait ~1.95ms
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE|TCON_MOD_PERIODIC);
  GptClrInt(pADI_TM1,TCLRI_TMOUT);
  while (!ucTimeout){}
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE_DIS|TCON_MOD_PERIODIC);
  NVIC_DisableIRQ(TIMER1_IRQn);
  // ADC power up sequence complete

  AdcCfg( ADCCFG_CHSEL_ADC0, ADCCFG_REF_LVDD, ADCCFG_CLK_FCOREDIV16, ADCCFG_ACQ_8 );
  AdcInit(ADCCON_REFBUF_EN,ADCCON_IEN_EN,ADCCON_DMA_DIS); // configures DMA, Interrupt and conversion mode
  NVIC_EnableIRQ(ADC0_IRQn);  
  
  // potentiometer connected between LVDD and P3.4
  DioCfg(pADI_GP3,0x55555500); // port 3 configured as GPIO
  DioOen(pADI_GP3, BIT4); // P3.4  as output
  DioClr(pADI_GP3, BIT4); // P3.4  low

  AdcCnv(ADCCON_MOD_CONT,ADCCON_START_EN); // start ADC in continous mode.
    
  while(1);
}

///////////////////////////////////////////////////////////////////////////
// ADC handler 
///////////////////////////////////////////////////////////////////////////

void ADC0_Int_Handler(void)
{   
  usAdcResult = AdcRd(0);
  return;
}

//*****************************************************************************
// Function    :
// Description : .
//*****************************************************************************/
void GP_Tmr1_Int_Handler ()
{
  if (GptSta(pADI_TM1)== TSTA_TMOUT) // if timout interrupt
  {
    GptClrInt(pADI_TM1,TCLRI_TMOUT);
    ucTimeout = 1;
  }
}
