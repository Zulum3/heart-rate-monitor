#ifndef _DAC_H
#define _DAC_H
#include <stm32f4xx.h>

#define DAC_port	GPIOA
#define DAC1_pin		4
#define DAC2_pin		5

void init_DAC(void);
void output_dac1(unsigned short d);
void output_dac2(unsigned short d);

#endif