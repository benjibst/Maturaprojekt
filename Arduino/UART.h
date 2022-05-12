#pragma once
#include <avr/io.h>

void init_usart9600();

void _putch(char ch);

void _putslen(char st[],uint8_t len);

void _puts(char st[]);

char _getch(void);

uint8_t _gets(char* st);

void _newline();