

#include "stm32f4xx.h"

void delay_ms(uint32_t ms);
void GPIO_Config(void);
void Display_7Segment(uint8_t digit);
void LED_Bar_Display(uint16_t adcValue);

int main(void) {
    GPIO_Config();

    uint16_t adcValue = 0; // Simulated ADC value for testing

    while (1) {
        for (uint8_t digit = 0; digit <= 9; digit++) {
            Display_7Segment(digit);
            delay_ms(500); // Delay to visualize each digit
        }

        adcValue = (adcValue + 100) % 4096; // Simulated ADC ramp
        LED_Bar_Display(adcValue);
        delay_ms(100);
    }
}

void GPIO_Config(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN;

    // Configure GPIOE for 7-segment display (PE0-PE7 for segments, PE10, PE11 for latches, PE15 for IOE)
    GPIOE->MODER |= 0x55555500; // Set PE0-PE7, PE10, PE11, PE15 as output

    // Configure GPIOF for LED bars (PF0-PF14 for R, G, and B LEDs)
    GPIOF->MODER |= 0x55555555; // Set PF0-PF14 as output
}

void Display_7Segment(uint8_t digit) {
    // Segment encoding for digits 0-9
    static const uint8_t segmentMap[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    // Set the segment data
    GPIOE->ODR &= ~0xFF; // Clear PE0-PE7
    GPIOE->ODR |= segmentMap[digit];

    // Toggle latches
    GPIOE->BSRR = GPIO_BSRR_BS10; // Set LE1
    GPIOE->BSRR = GPIO_BSRR_BR10; // Reset LE1

    GPIOE->BSRR = GPIO_BSRR_BS11; // Set LE2
    GPIOE->BSRR = GPIO_BSRR_BR11; // Reset LE2

    // Enable IOE
    GPIOE->BSRR = GPIO_BSRR_BS15; // Set IOE
}

void LED_Bar_Display(uint16_t adcValue) {
    // Scale ADC value to 16 levels (0-15)
    uint8_t level = (adcValue * 16) / 4096;

    // Set RED LEDs (PF7-PF14)
    GPIOF->ODR &= ~0xFF80; // Clear PF7-PF14
    GPIOF->ODR |= (level << 7);

    // Set GREEN LEDs (PF0-PF6)
    GPIOF->ODR &= ~0x007F; // Clear PF0-PF6
    GPIOF->ODR |= (level & 0x7F);

    // Set BLUE LEDs (PE6-PE13)
    GPIOE->ODR &= ~0x3FC0; // Clear PE6-PE13
    GPIOE->ODR |= ((level & 0x3F) << 6);
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) {
        __NOP();
    }
}
