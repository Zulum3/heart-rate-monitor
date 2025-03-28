#ifndef SEGMENT_H
#define SEGMENT_H

#include "stm32f429xx.h"

// GPIO Port and Pins
#define SEG_PORT GPIOB
#define DIGIT1   8   // Select Digit 1
#define DIGIT2   9   // Select Digit 2

// Define segment pins
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_DP 7

// Function Prototypes
void Segment_Init(void);
void Display_Temperature(uint16_t temp);
void Display_Digit(uint8_t digit, uint8_t value);

#endif
