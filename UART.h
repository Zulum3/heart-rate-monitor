#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

// Define USART Peripheral (Change to USART2 if needed)
#define USART_MODULE USART3

// Define GPIO Port and Pins for USART3 TX/RX (Change if using different USART)
#define USART_PORT   GPIOD
#define USART_TX_pin 8
#define USART_RX_pin 9

// Define Baud Rate
#define BAUDRATE 9600

// Function Prototypes
void init_USART(void);
void USART_SendChar(unsigned char d);
void USART_SendString(char *str);
void USART_SendChar1(char ch);
#endif // USART_H
