#include <avr/io.h>

void init_usart(uint32_t baud);

void _putch(char ch);

void _puts(char st[]);

char _getch(void);

void _gets(char* st);

void _newline();

void itoa8(uint16_t val,char *stringa);