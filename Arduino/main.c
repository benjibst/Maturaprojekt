#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "defines.h"
#include "stepper.h"
#include <stdlib.h>


char strcoords[20]; //21 byte für anzahl der punkte und 2 byte pro punkt mit max. 10 punkte
target targets[10];
uint8_t targetcnt=0;
uint8_t pointsreceived = 0;
steptypes man_dirx;
steptypes man_diry;
char stringx[7];
char stringy[7];
uint8_t mode=1;
//LED PB5

void init_adc();
uint8_t read_adc(uint8_t pinmsk);

ISR(USART_RX_vect)
{
	uint8_t sreg = SREG;
	SREG&=~(1<<7);
	targetcnt = _getch();
	uint8_t i;
	for(i=0;i<2*targetcnt;i++)
		strcoords[i]=_getch();
	strcoords[i]=0;
	_putslen(strcoords,2*targetcnt);
	
	for(i=0;i<targetcnt;i++)
	{
		targets[i].x=strcoords[2*i];
		targets[i].y=strcoords[2*i+1];
		targets[i].pos=pointfrompos(targets[i].x,targets[i].y);
	}
	pointsreceived=1;
	SREG = sreg;
}
ISR(TIMER0_OVF_vect)
{
	uint8_t sreg = SREG;
	cli();
	int8_t target_xvel = read_adc(0)-128;
	int8_t target_yvel = read_adc(1)-128;
	
	if(abs(target_xvel)<3)
		target_xvel=0;
	if(abs(target_yvel)<3)
		target_yvel=0;
	
	if(target_xvel>0)
		man_dirx  = XGO;
	else 
		man_dirx = XCOME;
	if(target_yvel>0)
		man_diry  = YGO;
	else
		man_diry = YCOME;
		
	float target_xfreq = (pow(2,abs(target_xvel)/28.0f)-1)*300.0f;
	float target_yfreq = (pow(2,abs(target_yvel)/28.0f)-1)*300.0f;
	
	itoa((int)target_xfreq,stringx,10);
	itoa((int)target_yfreq,stringy,10);
	_putch('X');
	_puts(stringx);
	_putch('Y');
	_puts(stringy);
	_newline();
	
	uint16_t ocrx_target=(uint16_t)15625/target_xfreq;
	uint16_t ocry_target=(uint16_t)15625/target_yfreq;
	uint8_t ocrx = ocrx_target<=255?ocrx_target:255;
	uint8_t ocry = ocry_target<=255?ocry_target:255;
	
	
	if(target_xvel==0)
		TCCR1B&=0b11111000;
	else
	{
		OCR1AL=ocrx;
		OCR1AH=0;
		if(TCNT1>ocrx)
			TCNT1 = ocrx -1;
		TCCR1B|=0b101;
	}
	if(target_yvel==0)
		TCCR2B&=0b11111000;
	else
	{
		OCR2A=ocry;
		if(TCNT2>ocry)
			TCNT2 = ocry-1;
		TCCR2B|=0b111;
	}
	SREG = sreg;
}
ISR(TIMER1_COMPA_vect) //stepper x routine
{
	uint8_t sreg = SREG;
	cli();
	step(man_dirx);
	SREG = sreg;
}
ISR(TIMER2_COMPA_vect) //stepper y routine
{
	uint8_t sreg = SREG;
	cli();
	step(man_diry);
	SREG = sreg;
}

int main(void)
{
	DDRC=0; //joystick port as input
	PORTC|=1<<JOYSTICK_SW; //enable pull up
	DDRD=
		(1<<XDirPin)|
		(1<<XStepPin)|
		(1<<XEnPin); //stepper pins as output
	
	DDRB = 
		(1<<MAGNET_PIN)|
		(1<<YDirPin)|
		(1<<YStepPin)|
		(1<<YEnPin); //stepper pins and magnet output pins
	DDRB |=1<<5; //builtin led
	mode =1;
	if (mode)
		goto manual;
	
	SREG|=(1<<7);
		
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
				PORTB|=(1<<MAGNET_PIN);
				_delay_ms(200);
				
				move(YCOME,targets[i].pos.y);
				move(XCOME,targets[i].pos.x);
				PORTB&=~(1<<MAGNET_PIN);
			}
			pointsreceived=0;
		}
	}
	#pragma endregion
	#pragma region ManualMode
	manual:
		init_adc();
		init_usart9600();
		__asm__ volatile ("sei");
		uint8_t portc_old=0;
		uint8_t portc_now=0;
		TCCR0A = 0;
		TCCR0B = 0b101; //prescaler 1/1024
		TIMSK0 |=1<<TOIE0;
			
		TCCR1A = 0;
		TCCR1B = (1<<WGM12);
		TIMSK1 = (1<<OCIE1A);
		TCCR2A = 1<<WGM21;
		TCCR2B=0;
		TIMSK2 =1<<OCIE2A;
		while (1)
		{
			portc_now = !(PINC&(1<<JOYSTICK_SW));
			if(portc_now&&!portc_old)
			{
				PORTB^=1<<5;
				PORTB ^= (1<<MAGNET_PIN);
				_delay_ms(5);
			}
			portc_old=portc_now;
			
		}
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

