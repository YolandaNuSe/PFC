/**
 *****************************************************************************
   @example  ADCTempSensor.c
   @brief    Demonstrate the use of the on-chip Temperature Sensor.
             The ADC is configured to convert on the temperature channel.
             Results are sent via UART.
             For more accurate readings, the ADuCRF101 should be placed in HIBERNATE
             mode before measuring the temperature.
             Characterisation of the temperature sensor is required to confirm conversion formula
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
void UARTInit(int);
unsigned char ucTimeout;


//*****************************************************************************
// Function    :
// Description : .
//*****************************************************************************/
int main (void)
{   
  WdtGo(T3CON_ENABLE_DIS);
  UARTInit(19200);
  
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
  
  AdcCfg(ADCCFG_CHSEL_TEMP, ADCCFG_REF_INTERNAL125V, ADCCFG_CLK_FCOREDIV16, ADCCFG_ACQ_8 );
  NVIC_EnableIRQ(ADC0_IRQn);  
  AdcCnv(ADCCON_MOD_CONT,ADCCON_START_EN); // start ADC conversion in continuous mode
  while (1){}  
}

///////////////////////////////////////////////////////////////////////////
// ADC handler 
///////////////////////////////////////////////////////////////////////////
void ADC0_Int_Handler(void)
{
   float temperature;
   usAdcResult = AdcRd(0);
   printf("ADC_reading = %2d\n", usAdcResult); 
   temperature = usAdcResult * 0.056 - 275;  // characterisation required to confirm formula
   printf("temperature = %2f\n", temperature); 

}

//*****************************************************************************
// Function    :
// Description : .
//*****************************************************************************/
void UARTInit(int uiBaud)
{
  UrtCfg(0,uiBaud,COMLCR_WLS_8BITS,COMLCR_STOP_DIS);
  // Enable the UART functionality on P1.0\P1.1
  pADI_GP1->GPCON &= ~(GP1CON_CON0_MSK     | GP1CON_CON1_MSK    );
  pADI_GP1->GPCON |=  (GP1CON_CON0_UART0RXD| GP1CON_CON1_UART0TXD);
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

