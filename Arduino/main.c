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
	uint8_t len = _gets(strcoords);
	_puts(strcoords,len);
	/*targetcnt = _getch();
	uint8_t i;
	for(i=0;i<2*targetcnt;i++)
	strcoords[i]=_getch();
	strcoords[i]=0;
	_puts(strcoords,2*targetcnt);
	
	for(i=0;i<targetcnt;i++)
	{
		targets[i].x=strcoords[2*i];
		targets[i].y=strcoords[2*i+1];
		targets[i].stepsx=(targets[i].x/255.0f*FIELD_SX+OFFSETX)/42.25*400*8;
		targets[i].stepsy=(targets[i].y/255.0F*FIELD_SY+OFFSETY)/42.25*400*8 ;
	}*/
	SREG = sreg;
}

int main(void)
{
	uint16_t steps=0;
	uint16_t sollsteps=0;
	uint16_t tcntrev=0;
	float time =0;
	init_usart9600();
	stepperInit(&stepperX);
	TCCR1A = 0;
	TCCR1B=0b00001100;
	DDRB|=1<<4;
	TCNT1=0;
	while (1)
	{
		while(steps<2000)
		{
			time = TCNT1/15625.0;
			sollsteps=250*time*time;
			sollsteps>>=1;
			if(sollsteps>steps)
			{
				steps++;
				step(&stepperX,0);
			}
		}
		tcntrev=TCNT1;
		TCNT1=0;
		sollsteps=0;
		while(steps>0)
		{
			
			time = (tcntrev-TCNT1)/15625.0;
			sollsteps=250*time*time;
			sollsteps>>=1;
			if(sollsteps<steps)
			{
				steps--;
				step(&stepperX,0);
			}
		}
		steps=0;
		sollsteps=0;
		TCNT1=0;
		while(steps<2000)
		{
			time = TCNT1/15625.0;
			sollsteps=250*time*time;
			sollsteps>>=1;
			if(sollsteps>steps)
			{
				steps++;
				step(&stepperX,1);
			}
		}
		tcntrev=TCNT1;
		TCNT1=0;
		sollsteps=0;
		while(steps>0)
		{
			
			time = (tcntrev-TCNT1)/15625.0;
			sollsteps=250*time*time;
			sollsteps>>=1;
			if(sollsteps<steps)
			{
				steps--;
				step(&stepperX,1);
			}
		}
		steps=0;
		sollsteps=0;
		TCNT1=0;
	}
	

	return 0;
}
