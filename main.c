#include <stm32f4xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
#include <stdlib.h>
#include <stdio.h>
#include "LED.h"
#include "ADC.h"
#include "UART.h"
#include "TIMERS.h"
#include "lcd.h"
#include "PLL_Config.c"
#include "DAC.h"



int main(void)
{
	//PLL_Config();
	SystemCoreClockUpdate();
	// Enable GPIO Ports
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable GPIOD clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // Enable GPIOE clock
	
	// Configure PB14 as Open-Drain Output
	GPIOB->MODER |= (1 << (2 * 14)); // Set PB14 mode
	GPIOB->OTYPER |= (1 << 14);  // Configure PB14 as open-drain
	GPIOB->PUPDR |= (1 << (2 * 14));  // Enable pull-up on PB14
	
	/*GPIOC->MODER |= (1 << (2 * 14)); // Set PC14 mode
	GPIOC->OTYPER |= (1 << 14);  // Configure PC14 as open-drain
	GPIOC->PUPDR |= (1 << (2 * 14));  // Enable pull-up on PC14*/

	// Configure PB0
	GPIOB->MODER |= (2 << (2 * 0));  // Set PB0 mode
	GPIOB->OSPEEDR |= (2 << (2 * 0)); // Set speed on PB0
	GPIOB->OTYPER |= (1 << 0);  // Configure PB0 as open-drain
	GPIOB->PUPDR |= (1 << (2 * 0));  // Enable pull-up on PB0
	
	// Configure Open-Drain for PB7
	GPIOB->MODER |= (1 << (2 * 7)); // Set PB7 mode
	GPIOB->OTYPER |= (1 << 7);  // Configure PB7 as open-drain
	GPIOB->PUPDR |= (1 << (2 * 7));  // Enable pull-up on PB7
	GPIOC->MODER &= ~(3U << (2 * 7));     // Clear mode bits for PC7
	GPIOC->MODER |= (1U << (2 * 7));      // Set PC7 to output mode
	
//  GPIOE->MODER |= 0x55550000;         // Set PE8-PE15 as output
//  GPIOE->ODR |= (0xFF << 8);          // Set all segments HIGH (OFF in active LOW)

	// Initialize ADC and DAC 
  init_ADC();
	init_DAC();
	Timer_Init();
	initLCD();
	LED_Init();
	
	
	// Initialize USART
 init_USART();
 
	
	// Initialize peripherals
 // Segment_Init();  // Initialize 7-segment display GPIOs
		
	//ADC1->CR2 |= ADC_CR2_ADON;  // Enable ADC1
	//ADC1->CR2 |= ADC_CR2_SWSTART; // Start ADC conversion


	while(1)					//ENTER 'FOREVER' LOOP - THIS LOOP WILL NEVER EXIT (otherwise CPU resets)
	{
		cmdLCD(LCD_LINE1);
		
		//putLCD('A');
		LCD_SendString("Heart rate = 80");
		cmdLCD(LCD_LINE2);
		LCD_SendString("Oxygen lvl= 96%");
		output_dac1(0);
		output_dac2(4000);
	//	blink_led();
		
//		set_PC7_on();
//		Display_Number(0);
		
		//ADC_DATA = read_adc(13);			//read value from ADC Channel 13
	// output_dac1(ADC_DATA);			//send straight to DAC (DAC pin should replicate ADC pin)
		//generateComplexWave;
//value = adc();
//send_usart(value);
	 while (1){
        compute_min_max(); // Compute min and max ADC values

       // Send formatted data over USART
       USART_SendString("\r\nMin Value: ");
       USART_SendNumber(min_value);
       USART_SendString("\r\nMax Value: ");
        USART_SendNumber(max_value);
       USART_SendString("\r\n------------\r\n");
		 	for (volatile int j = 0; j < 1000000; j++); // Delay
		 
		   // while (1) {
        //USART_SendString("Hello, World!\r\n");  
        //for (volatile int i = 0; i < 1000000; i++); // Small delay
    }

       // for (volatile int i = 0; i < 1000000; i++); // Simple delay
    }

       LED_PORT->ODR ^= (1U << LED_PIN);  // Toggle LED
		 
     for (uint8_t i = 0; i < 10; i++) {
				LED_Display(i); // Display numbers 0-9
				for (volatile int j = 0; j < 1000000; j++); // Delay
		 }
		
	}

	




				
				




	
	