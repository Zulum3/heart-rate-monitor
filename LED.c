#include <stm32f4xx.h>		//INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
#include "led.h"

// Segment patterns for numbers 0-9 (Common Cathode)
const uint8_t segmentMap[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Initialize GPIO for 7-segment display on Port E (PE0-PE7)
void LED_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // Enable clock for GPIOE

    GPIOE->MODER &= ~(0xFFFF);  // Clear mode bits for PE0-PE7
    GPIOE->MODER |= 0x55550000;         // Set PE8-PE15 as output
}


// Display a number (0-9) on 7-segment
void LED_Display(uint8_t num) {
    if (num > 9) return; // Only allow 0-9

    GPIOE->ODR &= ~(0xFF);       // Clear previous value
    GPIOE->ODR |= segmentMap[num]; // Set new value
}