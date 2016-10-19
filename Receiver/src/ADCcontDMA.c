/**
 *****************************************************************************
   @example  ADCcontDMA.c
   @brief    Demonstrate the use of the uDMA with ADC in continuous mode.
             Conversion is done on ADC1 in single ended mode.
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


void UARTInit(int);

typedef struct ctrl_cfg 
{
   unsigned int cycle_ctrl:3;
   unsigned int next_useburst:1;
   unsigned int n_minus_1:10;
   unsigned int r_power:4;
   unsigned int src_prot_ctrl:3;
   unsigned int dst_prot_ctrl:3;
   unsigned int src_size:2;
   unsigned int src_inc:2;
   unsigned int dst_size:2;
   unsigned int dst_inc:2;
} CtrlCfg;

typedef struct dma_desc // Define the structure of a DMA descriptor.
{
   unsigned int srcEndPtr;
   unsigned int destEndPtr;
   CtrlCfg ctrlCfg;
   unsigned int reserved4Bytes;
} DmaDesc;


#define CCD_SIZE 16
#define DMACHAN_DSC_ALIGN 0x200

#if defined(__ARMCC_VERSION) || defined(__GNUC__)
DmaDesc dmaChanDesc     [CCD_SIZE * 2] __attribute__ ((aligned (DMACHAN_DSC_ALIGN)));      
#endif

#ifdef __ICCARM__
#pragma data_alignment=(DMACHAN_DSC_ALIGN)
DmaDesc dmaChanDesc     [CCD_SIZE * 2];
#endif


DmaDesc *adcDmaDesc;

#define DMA_TRAN_COUNT 1024
unsigned short dataBlk[DMA_TRAN_COUNT];    // Array of ADC results
volatile unsigned char ucFlag;
unsigned char ucTimeout;

int main (void) 
{
  unsigned int i;

  WdtGo(T3CON_ENABLE_DIS);
  UARTInit(19200);

  ucFlag = 0;
  
  adcDmaDesc = &dmaChanDesc[11];	
  adcDmaDesc->srcEndPtr = (unsigned int) &pADI_ADC0->ADCDAT;
  adcDmaDesc->destEndPtr = (unsigned int)(dataBlk + DMA_TRAN_COUNT -1 ); 
  adcDmaDesc->ctrlCfg.dst_inc = 1;   // halfword incr.
  adcDmaDesc->ctrlCfg.dst_size = 1;  // halfword data
  adcDmaDesc->ctrlCfg.src_inc = 3;   // no increment as the scr is a reg 
  adcDmaDesc->ctrlCfg.src_size = 1;  // halfword data
  adcDmaDesc->ctrlCfg.dst_prot_ctrl = 0;
  adcDmaDesc->ctrlCfg.src_prot_ctrl = 0;
  adcDmaDesc->ctrlCfg.r_power = 0;
  adcDmaDesc->ctrlCfg.n_minus_1 = DMA_TRAN_COUNT - 1;
  adcDmaDesc->ctrlCfg.next_useburst = 0;
  adcDmaDesc->ctrlCfg.cycle_ctrl = 1; //  Basic DMA transfer
 
  pADI_DMA->DMACFG = 1;   // Enable the  uDMA
  pADI_DMA->DMAPDBPTR = (unsigned int) &dmaChanDesc; // Setup the DMA base pointer.
  pADI_DMA->DMAENSET = DMAENSET_ADC; // Enable ADC DMA channel  
  NVIC_EnableIRQ(DMA_ADC0_IRQn);


  // ADC initialisation, using GP timer 1.
  AdcPd(ADCCON_ENABLE_EN);        // Enable ADC and reference
  ucTimeout = 0;
  GptLd (pADI_TM1, 0x9C); // wait ~2.5ms
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE|TCON_MOD_PERIODIC);
  GptClrInt(pADI_TM1,TCLRI_TMOUT);
  NVIC_EnableIRQ(TIMER1_IRQn);
  while (!ucTimeout){}
  ucTimeout = 0;
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE_DIS|TCON_MOD_PERIODIC);
  AdcInit(ADCCON_REFBUF_EN,  ADCCON_IEN_DIS, ADCCON_DMA_EN);  // Enable reference buffer 
  GptLd (pADI_TM1, 0x79); // wait ~1.95ms
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE|TCON_MOD_PERIODIC);
  GptClrInt(pADI_TM1,TCLRI_TMOUT);
  while (!ucTimeout){}
  GptCfg(pADI_TM1, TCON_CLK_UCLK, TCON_PRE_DIV256, TCON_ENABLE_DIS|TCON_MOD_PERIODIC);
  NVIC_DisableIRQ(TIMER1_IRQn);
  // ADC power up sequence complete
    
  AdcCfg(ADCCFG_CHSEL_ADC1, ADCCFG_REF_INTERNAL125V, ADCCFG_CLK_FCOREDIV16, ADCCFG_ACQ_8 ); 
  AdcCnv(ADCCON_MOD_CONT, ADCCON_START_EN); //start ADC in continuous mode 

 
  while (!ucFlag)  {}

  for (i = 0; i<DMA_TRAN_COUNT; i++)
    printf("%2d\n",dataBlk[i]); 
  
  while(1)
  {}
  
}

///////////////////////////////////////////////////////////////////////////
// DMA ADC Interrupt handler 
///////////////////////////////////////////////////////////////////////////
void DMA_ADC_Int_Handler() 
{
  AdcCnv(ADCCON_MOD_CONT,ADCCON_START_DIS); // stop ADC
  NVIC_DisableIRQ(DMA_ADC0_IRQn);
  ucFlag = 1;
  return;
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
  //UrtDma(0,COMIEN_EDMAT);
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
