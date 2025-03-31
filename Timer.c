#include "timers.h"
#include "stm32f429xx.h"
#include "UART.h"
#include "ADC.h"
//#include "PLL_Config.c"
unsigned short ADC_DATA;
volatile uint16_t adc_buffer[ADC_BUFFER_SIZE] = {0};  // ADC Storage Buffer
volatile uint16_t adc_index = 0;  // Buffer index




void Timer_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   // Enable clock for GPIOD
    LED_PORT->MODER |= (1U << (LED_PIN * 2));  // Set LED pin as output
    
     // Enable Timer 2 clock
    TIM4->PSC = 9000 - 1;  // Prescaler for 10kHz
    TIM4->ARR = 1000 - 1;  // Auto-reload for 10Hz LED toggle
	  TIM4->CNT=0;
	  NVIC->ISER[0]=(1u<<28);

   // TIM2->EGR |= TIM_EGR_UG;// triggers an udate event for timer 2
    TIM4->DIER |= TIM_DIER_UIE;  // Enable update interrupt
    NVIC_EnableIRQ(TIM4_IRQn);  // Enable IRQ
    TIM4->CR1 |= TIM_CR1_CEN;  // Start Timer 2
}
void TIM4_IRQHandler(void) {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;  // Clear interrupt flag
       //LED_PORT->ODR ^= (1U << LED_PIN);  // Toggle LED
		//	USART_SendChar('H');
			 if (ADC1->SR & ADC_SR_EOC) {  // Check if ADC conversion is complete
        adc_buffer[adc_index] = ADC1->DR;  // Read ADC data
        adc_index = (adc_index + 1) % ADC_BUFFER_SIZE; // Wrap around properly
    
    }
//		ADC_DATA = read_adc(13);			//read value from ADC Channel 13
	 // output_dac(ADC_DATA);			//send straight to DAC (DAC pin should replicate ADC pin)

   
}


			}
