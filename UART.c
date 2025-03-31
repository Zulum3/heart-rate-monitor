#include "uart.h"
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

void init_USART(void) {
    // Enable Clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // Enable GPIO Port D
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART3 clock

    // Set TX (PD8) and RX (PD9) as Alternate Function Mode
    USART_PORT->MODER &= ~((3U << (2 * USART_TX_pin)) | (3U << (2 * USART_RX_pin))); 
    USART_PORT->MODER |=  ((2U << (2 * USART_TX_pin)) | (2U << (2 * USART_RX_pin)));

    // Configure Alternate Function for USART3 (AF7)
    USART_PORT->AFR[1] &= ~((0xF << (4 * (USART_TX_pin - 8))) | (0xF << (4 * (USART_RX_pin - 8))));
    USART_PORT->AFR[1] |=  ((7 << (4 * (USART_TX_pin - 8))) | (7 << (4 * (USART_RX_pin - 8))));

    // Ensure USART is disabled before configuration
    USART_MODULE->CR1 &= ~USART_CR1_UE; 

    // Set Baud Rate (assuming APB1 = 45MHz) 
	  uint32_t apb1_clock = SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> 10];
USART_MODULE->BRR = (apb1_clock + (230400 / 2)) / 230400;  // Proper rounding


    // Configure 8N1 (8-bit, No Parity, 1 Stop Bit)
    USART_MODULE->CR1 &= ~USART_CR1_M;     // 8-bit word length
    USART_MODULE->CR1 &= ~USART_CR1_PCE;   // No parity
    USART_MODULE->CR2 &= ~USART_CR2_STOP;  // 1 Stop Bit

    // Enable USART, TX, and RX
    USART_MODULE->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}

void USART_SendChar(unsigned char d) {
    while (!(USART_MODULE->SR & USART_SR_TC)); // Wait for TX buffer to be empty
    USART_MODULE->DR = d; // Send character
}

void USART_SendString(char *str) {
    while (*str) {
        USART_SendChar(*str++);
    }
}

void USART_SendNumber(uint16_t num) {
    char buffer[10];  // Buffer to hold converted number (max 5 digits for uint16_t)
    sprintf(buffer, "%u", num);  // Convert number to string
    USART_SendString(buffer);  // Send string over UART
}
