#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"

#define ADC_CHANNEL 0   // Modify based on your STM32 setup
#define NUM_SAMPLES 10  // Number of ADC samples for min/max calculation

extern uint16_t min_value;  // Declare globally accessible min/max values
extern uint16_t max_value;

void init_ADC(void);
uint16_t read_ADC(void);
void compute_min_max(void);

#endif
