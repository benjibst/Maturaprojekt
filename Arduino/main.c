#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "defines.h"
#include "stepper.h"
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

char strcoords[20]; //21 byte für anzahl der punkte und 2 byte pro punkt mit max. 10 punkte
target targets[10];
uint8_t currenttarget=0;
uint8_t targetcnt=0;
uint16_t stepscnt=1;
float calcocr=700;
//LED PB5



ISR(USART_RX_vect)
{
	uint8_t sreg = SREG;
	SREG&=~(1<<7);
	targetcnt = _getch();
	uint8_t i;
	for(i=0;i<2*targetcnt;i++)
		strcoords[i]=_getch();
	strcoords[i]=0;
	_puts(strcoords,2*targetcnt);
	
	for(i=0;i<targetcnt;i++)
	{
		targets[i].x=strcoords[2*i];
		targets[i].y=strcoords[2*i+1];
		targets[i].pos=pointfrompos(targets[i].x,targets[i].y);
	}
	SREG = sreg;
}

int main(void)
{
	_delay_ms(3000);
	init_usart9600();
	_puts("awd",3);
	_putch(0);
	DDRD|=0b11<<3;
	DDRB|=1<<PORTB5;
	while (1)
	{
		PORTB|=1<<5;
		move(YGO,(uint16_t)10000);
		PORTB&=~(1<<5);
		move(YCOME,(uint16_t)5000);
	}
}

