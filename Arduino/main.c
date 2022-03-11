#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "lcd.h"
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

ISR(TIMER1_COMPA_vect)
{
	PORTB^=1<<4;
	stepscnt++;
	if(stepscnt<=2000)
		calcocr = calcocr-(calcocr*2)/((stepscnt*4)+1);
	if(stepscnt>=10000)
		calcocr = calcocr-(calcocr*2)/(((12000-stepscnt)*4)+1);
	if(stepscnt>=12000)
	{
		stepscnt=0;
		calcocr=700;
	}
	OCR1A=calcocr;
	//if(stepscnt<2000)
		//
	//if(stepscnt>10000)
		//calcocr=calcocr-(calcocr*2)/(((12000-stepscnt)*4)+1);
	//if(stepscnt==12000)
	//{
		//stepscnt=0;
		//calcocr=73;
	//}
	//OCR1A=(uint16_t)calcocr;
	
}

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
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts_P("asdas1111dasd");
	init_usart9600();
	TCCR1A = 0;
	TCCR1B=0b00001100;
	DDRB|=1<<4;
	SREG|=(1<<7);
	TIMSK1|=1<<OCIE1A;
	OCR1A=700;
	TCNT1=0;
	while(1)
	{
		currenttarget=0;
	}

	return 0;
}
