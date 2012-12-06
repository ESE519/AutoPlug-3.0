#ifndef _ADC_H
#define _ADC_H

void wait20us(void);

void ADCInit(void);

UINT8 ADCConvertSingle(UINT8 channel);

#endif
