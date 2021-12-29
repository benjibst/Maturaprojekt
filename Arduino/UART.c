#include "UART.h"

void init_usart(uint32_t baud)
{
    UBRR0L = (16000000ul/(16ul*baud))-1;
    UBRR0H = 0;
    UCSR0B |= (1<<TXEN0) | (1<<RXEN0)|(1<<RXCIE0);           // Sender und Empfänger aktivieren
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);         // 8-Bit Übertragung
 // Asynchrone Übertragung (UMSEL00=0 und UMSEL01=0)
}

void _putch(char ch)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = ch;
}

void _puts(char st[])
{
    uint8_t i=0;
    while(st[i])
        _putch(st[i++]);
}

char _getch(void)
{
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

void _gets(char* st)
{
    do
    {
        *st=_getch();
    } while (*st++);  // die Funktion WriteLine von C# schickt einen String,
}

void _newline()
{
    _putch(13);   // CR
    _putch(10);   // LF
}

void itoa8(uint16_t val,char *stringa)
{
    stringa[0]=(val%10000)/1000+48;
    stringa[1]=(val%1000)/100+48;
    stringa[2]=(val%100)/10+48;
    stringa[3]=(val%10)+48;
    stringa[4]=0;
}