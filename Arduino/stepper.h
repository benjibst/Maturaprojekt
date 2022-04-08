#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "defines.h"
#include "UART.h"

#pragma region PinPort
#define XPort PORTD
#define XDdr DDRD
#define YPort PORTB
#define YDdr DDRB

#define XDirPin  4
#define XStepPin 5
#define XEnPin 6

#define YDirPin  0
#define YStepPin 1
#define YEnPin 2
#pragma endregion PinPort

enum steptypes
{
	XGO=0b00,
	YGO=0b01,
	XCOME=0b10,
	YCOME=0b11,
};
typedef enum steptypes steptypes;

void step(steptypes s);
void move(steptypes s,uint16_t microsteps);
point pointfrompos(uint8_t x,uint8_t y);




