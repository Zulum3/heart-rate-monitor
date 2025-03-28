
#ifndef TIMERS_H
#define TIMERS_H
#include <stm32f4xx.h>
#define ADC_BUFFER_SIZE 1000  // Store 1000 samples (100ms worth at 10kHz)
#define LED_PORT GPIOE
#define LED_PIN  12


extern volatile uint16_t adc_buffer[ADC_BUFFER_SIZE];
extern volatile uint16_t adc_index;


void Timer_Init(void);
void TIM2_IRQHandler(void);
extern unsigned short ADC_DATA;

#endif /* TIMERS_H */