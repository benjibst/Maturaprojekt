
#include "lcd.h"
#include "avr/io.h"
int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("asd1");
	while(1);
}