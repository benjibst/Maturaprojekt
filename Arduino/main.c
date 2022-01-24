#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "lcd.h"
#include "defines.h"
#include <string.h>
#include <stdlib.h>

char strcoords[21]; //21 byte für anzahl der punkte und 2 byte pro punkt mit max. 10 punkte
uint8_t points=0;
uint16_t stepscnt=1;
//LED PB5

ISR(TIMER1_COMPA_vect)
{
	stepscnt++;
	PORTB^=1<<4;
	FMAX
	float freq = stepscnt;
	uint16_t ocrval = (uint16_t)(15625/freq);
	OCR1A = ocrval;
	if(stepscnt==5000)
		stepscnt=1;
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
	TCCR1A = 0;
	TCCR1B=0b00001101;
	DDRB|=1<<4;
	PORTB|=1<<4;
	SREG|=(1<<7);
	TIMSK1|=1<<OCIE1A;
	OCR1A=15624;
	TCNT1=0;
	while(1)
	{

	}

	return 0;
}
