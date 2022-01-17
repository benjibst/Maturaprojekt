#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "lcd.h"
#include <string.h>
#include <stdlib.h>


char strcmd[8];
int points=0;
//LED PB5

ISR(USART_RX_vect)
{
	uint8_t sreg = SREG;
	SREG&=~(1<<7);
	DDRB |= 1<<5;
	PORTB |=1<<5;
	points = _getch();
	for(int i=0;i<2*points;i++)
	{
		strcmd[i]=_getch();
	}
	strcmd[8]=0;
	_puts(strcmd);
	SREG = sreg;
}
int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts_P("asdas1111dasd");
	init_usart(9600);
	SREG|=(1<<7);
	while(1)
	{

	}

	return 0;
}
