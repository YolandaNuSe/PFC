/**
 *****************************************************************************
   @example  ADCtimerOverflow.c
   @brief    Demonstrate the use of the timer overflow to trigger ADC conversions.
             The ADC is configured to convert on ADC1 in single ended mode.
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

unsigned char ucTimeout;
volatile unsigned short usAdcResult;

int main()
{
  WdtGo(T3CON_ENABLE_DIS);

  Urt_Init(UART_BAUD_19200);
  Urt_PinSetup(FALSE);

  DioCfg(pADI_GP4,0x10); // P4.2  as GPIO
  DioOen(pADI_GP4, BIT2); // P4.2  as output
  
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
  AdcInit(ADCCON_REFBUF_EN,  ADCCON_IEN_EN, ADCCON_DMA_DIS); // Enable reference buffer 
  GptLd (pADI_TM1, 0x79); // wait ~1.95ms
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE|TCON_MOD_PERIODIC);
  GptClrInt(pADI_TM1,TCLRI_TMOUT);
  while (!ucTimeout){}
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE_DIS|TCON_MOD_PERIODIC);
  NVIC_DisableIRQ(TIMER1_IRQn);
  // ADC power up sequence complete

  AdcCfg(ADCCFG_CHSEL_ADC1, ADCCFG_REF_INTERNAL125V, ADCCFG_CLK_FCOREDIV16, ADCCFG_ACQ_8 );
  NVIC_EnableIRQ(ADC0_IRQn);  
  AdcCnv(ADCCON_MOD_T0OVF,ADCCON_START_EN);

  SysClkCfg(CLKCON_CD_DIV1,CLKCON_CLKMUX_HFOSC,CLKCON_CLKOUT_UCLKCG,XOSCCON_ENABLE_EN);
 
  GptLd (pADI_TM0, 0x3000); 
  GptCfg(pADI_TM0, T0CON_CLK_UCLK, T0CON_PRE_DIV16, T0CON_ENABLE|T0CON_MOD_PERIODIC|TCON_RLD);
  //NVIC_EnableIRQ(TIMER0_IRQn);  // to check timer overflow

  while(1);
}


///////////////////////////////////////////////////////////////////////////
// Timer0 handler 
///////////////////////////////////////////////////////////////////////////
void GP_Tmr0_Int_Handler ()
{
  if (GptSta(pADI_TM0) && TSTA_TMOUT) // if timout interrupt
  {
    DioTgl(pADI_GP4,BIT2); // toggle P4.2 
    GptClrInt(pADI_TM0,TCLRI_TMOUT);
  }
}

///////////////////////////////////////////////////////////////////////////
// Timer1 handler 
///////////////////////////////////////////////////////////////////////////
void GP_Tmr1_Int_Handler ()
{
  if (GptSta(pADI_TM1)== TSTA_TMOUT) // if timout interrupt
  {
    GptClrInt(pADI_TM1,TCLRI_TMOUT);
    ucTimeout = 1;
  }
}


///////////////////////////////////////////////////////////////////////////
// ADC handler 
///////////////////////////////////////////////////////////////////////////
void ADC0_Int_Handler(void)
{   
  usAdcResult = AdcRd(0);
  printf("ADC_reading = %2f\n", usAdcResult*1.25/16384); 
  DioTgl(pADI_GP4,BIT2); // toggle P4.2 
  return;
}


