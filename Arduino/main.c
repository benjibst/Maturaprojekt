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

uint8_t mode=0;
//LED PB5

void init_adc();
uint8_t read_adc();

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
ISR(TIMER0_COMPA_vect)
{
	uint8_t sreg = SREG;
	int8_t  target_xvel = read_adc(0)-128;
	int8_t target_yvel = read_adc(1)-128;
	uint8_t target_xdir = target_xvel & 128;
	uint8_t target_ydir = target_yvel & 128;
	uint8_t target_xfreq = abs(target_xvel)*30;
	uint8_t target_yfreq = abs(target_yvel)*30;
	SREG = sreg;
}
ISR(TIMER1_COMPA_vect) //stepper x routine
{
	
}
ISR(TIMER2_COMPA_vect) //stepper y routine
{
	
}

int main(void)
{
	DDRC=0;
	PORTC|=1<<JOYSTICK_SW;
	
	DDRD|=0b111<<3;
	DDRB|=0b111;
	
	mode = PINC>>2&1;
	
	sei();
	if (mode)
		goto manual;
		
	#pragma region AutoMode
	init_usart9600();
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
			pointsreceived=0;
		}
	}
	#pragma endregion
	#pragma region ManualMode
	manual:
		TCCR0A = 2;
		TCCR0B = 0b101; //prescaler 1/1024
		OCR0A = 156;
		TIMSK0 |=1<<OCIE0A;
		TCCR1A = 0;
		TCCR1B = 0b00001101;
		TIMSK1 |= 1<<OCIE1A;
		TCCR2A = 2;
		TCCR2B=0b111;
		TIMSK2 |=OCIE2A;
		
	#pragma endregion
}
void init_adc()
{
	ADCSRA=0x87;
	ADMUX=0x60;
}
uint8_t read_adc(uint8_t pinmsk)
{
	ADMUX &= ~ (0b1111);
	ADMUX |= pinmsk;
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	return ADCH;
}

