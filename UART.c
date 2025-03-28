#include "stm32f4xx.h"
#include "stdio.h"

#define USART_MODULE USART3
#define USART_PORT   GPIOD
#define USART_TX_pin 8
#define USART_RX_pin 9
#define BAUDRATE 9600

void USART_SendChar(unsigned char d) {
    while (!(USART_MODULE->SR & USART_SR_TXE)); // Wait until TX buffer is empty
    USART_MODULE->DR = d; // Transmit character
}

void init_USART(void) {
    // Enable GPIOD clock for USART3 TX/RX pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // Enable clock for GPIOD
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART3 clock

    // Configure TX and RX pins as Alternate Function
    USART_PORT->MODER &= ~((3U << (2 * USART_TX_pin)) | (3U << (2 * USART_RX_pin))); 
    USART_PORT->MODER |= ((2U << (2 * USART_TX_pin)) | (2U << (2 * USART_RX_pin)));  // Set pins as AF

    // Configure Alternate Function for USART TX (AF7) and RX (AF7) for USART3
    USART_PORT->AFR[1] &= ~((0xF << (4 * (USART_TX_pin - 8))) | (0xF << (4 * (USART_RX_pin - 8)))); 
    USART_PORT->AFR[1] |= ((7 << (4 * (USART_TX_pin - 8))) | (7 << (4 * (USART_RX_pin - 8))));  // Set AF7 for USART3

    // USART Configuration
    USART_MODULE->CR1 &= ~USART_CR1_M;  // 8-bit data length
    USART_MODULE->CR1 &= ~USART_CR1_PCE; // No parity
    USART_MODULE->CR2 &= ~USART_CR2_STOP; // 1 Stop bit

    // Set Baud Rate (PCLK1 is typically SystemCoreClock / 2)
    uint32_t pclk = SystemCoreClock / 2; // Assuming the default PCLK1 frequency is SystemCoreClock / 2
    USART_MODULE->BRR = pclk / BAUDRATE; // Set baud rate register

    // Enable USART: TX, RX, and USART itself
    USART_MODULE->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}

void USART_SendString(char *str) {
    while (*str) {
        USART_SendChar(*str++);
    }
}

void USART_SendChar1(char ch) {
    while (!(USART3->SR & USART_SR_TXE));  // Wait until TX buffer is empty
    USART3->DR = ch;  // Send character
}
   

	//while(1)
	//{
	//	send_usart('A');
	//	for(i=0; i<1000; i++)		// leaves a gap between transmissions ... NOT NEEDED for normal operation
	//	{
	//		__NOP();
	//	}
//	}
