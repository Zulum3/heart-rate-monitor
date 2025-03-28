


#include "ADC.h"
#include "UART.h"

unsigned short adc_result_current = 0;  // Initialize to 0 or an appropriate value
unsigned short buffer_index = 0;        // Initialize buffer index
unsigned short adc_average[5] = {0};    // Buffer for 5 samples
unsigned long adc_sum = 0;              // Sum for averaging



void init_ADC(void)
{
    // GPIOC Clock Enable for ADC1_Channel10
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    ADC1_input_port->MODER |= (3u << (2 * ADC1_input_pin));  // PC0 in Analog mode

    // GPIOA Clock Enable for ADC1_Channel0 and ADC1_Channel3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    ADC2_input_port->MODER |= (3u << (2 * ADC2_input_pin));  // PA0 in Analog mode
    ADC3_input_port->MODER |= (3u << (2 * ADC3_input_pin));  // PA3 in Analog mode

    // ADC1 Clock Enable
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
		// Configure ADC1 for 12-bit resolution
    ADC1->CR1 &= ~ADC_CR1_RES; // Clear resolution bits
    ADC1->CR1 |= (0 << ADC_CR1_RES_Pos); // 12-bit resolution
	
		// Set ADC conversion sequence length to 1 (single channel)
    ADC1->SQR1 &= ~ADC_SQR1_L;
	
    ADC1->CR2 |= ADC_CR2_ADON;  // Enable ADC1
		
		  // Delay to ensure ADC is ready
    for (volatile int i = 0; i < 10000; i++);
}

		unsigned short read_adc(uint8_t channel)
			{
    // Select the corresponding ADC channel
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;   // Clear previous selection
    ADC1->SQR3 |= (channel << ADC_SQR3_SQ1_Pos); // Set new channel

    // Start ADC conversion by software
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for conversion to complete
    while (!(ADC1->SR & ADC_SR_EOC));

    // Return ADC result
    return ADC1->DR;
			}
		

void init_Timer2(void)
{
    // Enable TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Correct clock enable for TIM2

    // Configure TIM2 for 1 kHz trigger
    TIM2->PSC = 16000 - 1;  // Prescaler: 16 MHz / 16,000 = 1 kHz
    TIM2->ARR = 1 - 1;      // Auto-reload value: Triggers every 1 ms (1 kHz)
    
    // Configure TIM2 TRGO to generate update event
    TIM2->CR2 &= ~TIM_CR2_MMS;
    TIM2->CR2 |= (0b010 << TIM_CR2_MMS_Pos); // Update event as TRGO

    // Start Timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

	
void convert_adc_result(void) {
    unsigned long voltage_mv = ((unsigned long)adc_result_current * 3300) / 4095;
    unsigned short integer_part = (unsigned short)(voltage_mv / 1000);
    unsigned short decimal_part = voltage_mv % 1000;

    char PULSE[7]; // Correct declaration

    PULSE[0] = (char)(integer_part + '0');  // Single digit integer part
    PULSE[1] = '.';
    PULSE[2] = (char)((decimal_part / 100) + '0');
    PULSE[3] = (char)(((decimal_part / 10) % 10) + '0');
    PULSE[4] = (char)((decimal_part % 10) + '0');
    PULSE[5] = 'V';
    PULSE[6] = '\0';  // Null termination

    USART_SendString(PULSE); // Function to send string via USART
}

void ADC_sample(void) {
    adc_result_current = 1234; // Replace with actual ADC result

    // Update sum and average buffer
    adc_sum -= adc_average[buffer_index];  // Subtract the old value from sum
    adc_average[buffer_index] = adc_result_current;  // Store the new result in the buffer
    adc_sum += adc_result_current;  // Add the new value to sum

    buffer_index = (buffer_index + 1) % 5;  // Update buffer index in circular manner
}
		
		
		
