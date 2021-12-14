#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include <string.h>
#include <stdlib.h>


char strcmd[6];

//LED PB5

ISR(USART_RX_vect)
{
	uint8_t sreg = SREG;
	SREG&=~(1<<7);
	DDRB |= 1<<5;
	PORTB |=1<<5;
	_gets(strcmd);
	strcmd[0]= 'a';
	strcmd[1]='a';
	strcmd[2]=0;
	_puts(strcmd);
	_newline();
	SREG = sreg;
}
int main(void)
{
	DDRD =0b00000100;
	init_usart(9600);
	SREG|=(1<<7);
	while(1)
	{

	}

	return 0;
}
