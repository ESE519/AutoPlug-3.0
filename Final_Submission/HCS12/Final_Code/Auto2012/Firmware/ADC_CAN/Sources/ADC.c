#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h> /* derivative information */
#include "common.h"
#include "ADC.h"
#pragma LINK_INFO DERIVATIVE "mc9s12c128"

void ADCInit(void)
{
    ATDCTL2 = 0xE0;
    wait20us();
    ATDCTL3 = 0x0A; // Seq length = 1, finish current conversion before freeze
    ATDCTL4 = 0x80; // 8-bit, Sample time = 2 cycles, Prescaler = /2
}

UINT8 ADCConvertSingle(UINT8 channel)
{
    UINT8 result;

    if(channel > 7)
        return 0;

    ATDCTL5 = 0x80 | channel; // Right justified, unsigned, single conversion
    while(!ATDSTAT0_SCF) ;    // Wait for ADC conversion
    result = ATDDR0L;
    return result;
}

void wait20us(void)
{
    TSCR1 = 0x90;
    TSCR2 = 0;
    TIOS  = 0x01;
    TC0   = TCNT + 480;
    while(!TFLG1_C0F) ;
}
