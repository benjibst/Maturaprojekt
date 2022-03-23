#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "defines.h"
#include "stepper.h"
#include <util/delay.h>

char strcoords[20]; //21 byte für anzahl der punkte und 2 byte pro punkt mit max. 10 punkte
target targets[10];
uint8_t targetcnt=0;
uint8_t pointsreceived = 0;
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
	pointsreceived=1;
	SREG = sreg;
}

int main(void)
{
	sei();
	init_usart9600();
	DDRD|=0b11<<3;
	DDRB|=1<<PORTB5;
	while (1)
	{
		if(pointsreceived)
		{
			for(uint8_t i=0;i<targetcnt;i++)
			{
				move(XGO,targets[i].pos.x);
				move(YGO,targets[i].pos.y);
				move(YCOME,targets[i].pos.y);
				move(XCOME,targets[i].pos.x);
			}
		}
	}
}

