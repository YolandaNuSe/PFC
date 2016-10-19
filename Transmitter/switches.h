//switches.h
#include "leds.h"
#include "ADUCRF101.h"

//Time to eliminate the rebound of the buttons
#define DELETE_REBOUND 0x7000;

//CHANNELS
#define CH1		0x0001
#define CH2		0x0002
#define CH3		0x0003
#define CH4		0x0004
#define CH5		0x0005
#define CH6		0x0006
#define CH7		0x0007
#define CH8		0x0008
#define CH9		0x0009
#define CH10	0x000A
#define B_ON	0xFFFF

void Config_External_Interrup (void);
void Ext_Int4_Handler (void);
