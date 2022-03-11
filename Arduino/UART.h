#include <avr/io.h>

void init_usart9600();

void _putch(char ch);

void _puts(char st[],uint8_t len);

char _getch(void);

uint8_t _gets(char* st);

void _newline();

void itoa8(uint16_t val,char *stringa);