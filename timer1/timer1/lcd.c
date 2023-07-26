/*
 * lcd.c
 *
 * Created: 3/8/2023 10:09:36 PM
 *  Author: abdelrhman mattar
 */ 

#include "lcd.h"
#define F_CPU 8000000ul
#include <util/delay.h>
#define bitmode4
void lcd_init()
{
	#if defined bitmode8
	_delay_ms(200);
	PORTC=0xff;
	dio_setport('A',0xff);
	dio_setpin('B',0,1);
	dio_setpin('B',1,1);
	dio_writepin('B',1,0);
	lcd_cmd(returnhome);
	_delay_ms(10);
	lcd_cmd(EIGHT_BITS);
	_delay_ms(1);
	lcd_cmd(CURSOR_ON_DISPLAN_ON);
	_delay_ms(1);
	lcd_cmd(clearscreen);
	_delay_ms(1);
	lcd_cmd(ENTRY_MODE);
	_delay_ms(1);
	#elif defined bitmode4
	dio_setpin('A',4,1);
	dio_setpin('A',5,1);
	dio_setpin('A',6,1);
	dio_setpin('A',7,1);
	dio_setpin('B',0,1);
	dio_setpin('B',1,1);
	dio_writepin('B',1,0);
	lcd_cmd(returnhome);
	_delay_ms(2);
	lcd_cmd(FOUR_BITS);
	_delay_ms(2);
	lcd_cmd(CURSOR_ON_DISPLAN_ON);
	_delay_ms(2);
	lcd_cmd(clearscreen);
	_delay_ms(2);
	lcd_cmd(ENTRY_MODE);
	_delay_ms(2);
	#endif
}
void enable()
{
	dio_writepin('B',0,1);
	_delay_ms(2);
	dio_writepin('B',0,0);
	_delay_ms(2);
}

void lcd_cmd(char cmd)
{
	#if defined bitmode8
	PORTA=cmd;
	dio_writepin('B',1,0);
	enable();
	if (cmd==clearscreen)
	{
		_delay_ms(10);
	}
	else _delay_ms(2);
	#elif defined bitmode4
	write_high_nibble('A',cmd>>4);
	dio_writepin('B',1,0);
	enable();
	write_high_nibble('A',cmd);
	dio_writepin('B',1,0);
	enable();
	_delay_ms(1);
	#endif
}
void movecurse(char row , char colum)
{
	
	char data ;
	if(row>2||row<1||colum>16||colum<1)
	{
		data=0x80;
	}
	else if (row ==1)
	{
		data=0x80+colum-1;
	}
	else if (row==2)
	{
		data=0xc0+colum-1;
	}
	lcd_cmd(data);
	_delay_ms(1);
}
void lcd_char(char data)
{
	#if defined bitmode8
		PORTA=data;
		dio_writepin('B',1,1);
		enable();
		_delay_ms(1);
	#elif defined bitmode4
		write_high_nibble('A',data>>4);
		dio_writepin('B',1,1);
		enable();
		write_high_nibble('A',data);
		dio_writepin('B',1,1);
		enable();
		_delay_ms(1);
	#endif
}
void lcd_string(char * data)
{
	
	while(*data != '\0')
	{
		lcd_char(*data);
		data++;
	}
}

