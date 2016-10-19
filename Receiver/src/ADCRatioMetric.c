/**
 *****************************************************************************
   @example  ADCRatioMetric.c
   @brief    Demonstrate how to configure the ADuCRF101 for ratiometric measurements.
             The ADC is configured to acquire samples after external interrupt 3 (P0.7) 
             is asserted. The results are sent via UART after 24 ADC conversions.
             The potentiometer is only connected for the durationof the ADC measurements.
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


#define SIZE 24
volatile unsigned char ucState, ucLastState;
volatile unsigned short usAdcResult[SIZE], usIndex;
unsigned char ucTimeout;

#define SLEEP 0
#define AWAKE 1
#define CONVERT 2
#define PRINT 3


int main()
{
  int i;
  WdtGo(T3CON_ENABLE_DIS); // disable watchdog timer

  UrtCfg(0,19200,COMLCR_WLS_8BITS,COMLCR_STOP_DIS);
   
  EiCfg(EXTINT3, INT_EN, INT_FALL); // enable ext int3
  NVIC_EnableIRQ(EINT3_IRQn);
    
  PerClkIni(1, CLK_DMA|CLK_T2|CLK_SPI0|CLK_SPI1|CLK_I2C|CLK_PWM|CLK_T0|CLK_T1); // disable all clocks in active mode
    
  while ((DioRd(pADI_GP0)& BIT6)==BIT6){} // wait for user to press the boot button on the miniboard (P0.6)

  // configure P1.0 &P1.1 in tristate
  DioCfg(pADI_GP1,0x0000); // port 1 configured for GPIO
  DioOen(pADI_GP1,BIT0+BIT1); // P1.0 & P1.1 are outputs
  DioOce(pADI_GP1,BIT0+BIT1); // P1.0 & P1.1 will be open circuit when driving out 1.
  DioSet(pADI_GP1,BIT0+BIT1); // P1.0 & P1.1 are driving out a 1

  // configure P3.4 in tristate
  DioCfg(pADI_GP3,0x550); // port 3 configured as GPIO
  DioOen(pADI_GP3,BIT4); // P3.4  is an output
  DioOce(pADI_GP3,BIT4); // P3.4 will be open circuit when driving out a 1.
  DioSet(pADI_GP3,BIT4); // P3.4 driving out a 1

  ucState = SLEEP;
  PwrCfg(PWRMOD_MOD_HIBERNATE,SCR_SLEEPONEXIT_DIS);
  if (!SWACT_ACT) // not in debug mode
    while (!PWRMOD_WICENACK_BBA); // wait until WICENACK is set 
  __DSB();  // wait for PWRMOD register to be written
  __WFI();  // Go to sleep! */  

  
  while (1)
  {
    if (ucState == CONVERT) // return from ext int, config ADC & P3.4
    {    
      
      DioClr(pADI_GP3,BIT4); // connected for measurements
      usIndex = 0;
 
      // ADC initialisation, using GP timer 1.
      AdcPd(ADCCON_ENABLE_EN);       // Enable ADC and reference
      ucTimeout = 0;
      PerClkAct(1,CLK_T1);// turn on T1 clock
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
      PerClkAct(0,CLK_T1);// turn off T1 clock
      NVIC_DisableIRQ(TIMER1_IRQn);
      // ADC power up sequence complete

      AdcCfg(ADCCFG_CHSEL_ADC0,ADCCFG_REF_LVDD,ADCCFG_CLK_FCOREDIV32,ADCCFG_ACQ_16);
      ucState = AWAKE;
      NVIC_EnableIRQ(ADC0_IRQn);  
      
      AdcCnv(ADCCON_MOD_CONT,ADCCON_START_EN);  
    }
    if (ucState == PRINT) // return from ADC, print results and go back to sleep
    { 
      DioCfg(pADI_GP1,0x9); // port 1 configured for UART
      for (i = 0; i<usIndex; i++) // to usIndex in case the interrupt asserted before SIZE was reached
        printf("%2f\n", usAdcResult[i]*1.8/16384); 
      ucState = SLEEP;
      EiClr(EXTINT3);
      NVIC_ClearPendingIRQ(EINT3_IRQn);
      NVIC_EnableIRQ(EINT3_IRQn);
      DioCfg(pADI_GP1,0x0000); // port 1 configured for GPIO tristate
      PwrCfg(PWRMOD_MOD_HIBERNATE,SCR_SLEEPONEXIT_DIS);
      if (!SWACT_ACT) // not in debug mode
        while (!PWRMOD_WICENACK_BBA); // wait until WICENACK is set
      __DSB();  // wait for PWRMOD register to be written
      __WFI();  // Go to sleep!  */ 
    }
  }
}


///////////////////////////////////////////////////////////////////////////
// External Interrupt3 handler 
///////////////////////////////////////////////////////////////////////////
void Ext_Int3_Handler ()
{
  ucState = CONVERT;
  EiClr(EXTINT3);
  NVIC_DisableIRQ(EINT3_IRQn);
}

///////////////////////////////////////////////////////////////////////////
// ADC handler 
///////////////////////////////////////////////////////////////////////////
void ADC0_Int_Handler(void)
{   
  usAdcResult[usIndex] = AdcRd(0);
  usIndex++;
  if (usIndex >= SIZE) 
  {
    AdcPd(ADCCON_ENABLE_DIS);
    NVIC_DisableIRQ(ADC0_IRQn);
    DioSet(pADI_GP3,BIT4); // disconnect P3.4 from the potentiometer
    ucState = PRINT;
  }
  return;
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


