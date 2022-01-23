#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "lcd.h"
#include <string.h>
#include <stdlib.h>

#define FIELD_SX 250 //Field dimensions in cm
#define FIELD_SY 250
#define OFFSETX 30	//Vector to the bottom left corner of the field in mm
#define OFFSETY 60
#define FMAX 6400	//Stepper motor = 200steps/360°turn. MS resolution is 1/32 => (200steps/turn)*(1turn/s)*32microsteps/step) = 6400microsteps/s

struct target
{
	uint8_t x;
	uint8_t y;
	uint16_t stepsx;
	uint16_t stepsy;
};
char strcoords[21];
uint8_t points=0;
//LED PB5

ISR(TIMER0_COMPA_vect)
{
	
}

ISR(TIMER1_COMPA_vect)
{
	
}

ISR(USART_RX_vect)
{
	uint8_t sreg = SREG;
	SREG&=~(1<<7);
	points = _getch();
	uint8_t i;
	for(i=0;i<2*points;i++)
	{
		strcoords[i]=_getch();
	}
	
	strcoords[i]=0;
	_puts(strcoords,2*points);
	SREG = sreg;
}

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts_P("asdas1111dasd");
	init_usart9600();
	
	SREG|=(1<<7);
	while(1)
	{

	}

	return 0;
}
