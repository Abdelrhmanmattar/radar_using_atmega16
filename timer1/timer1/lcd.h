/*
 * lcd.h
 *
 * Created: 3/8/2023 10:09:22 PM
 *  Author: abdelrhman mattar
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "DIO.h"
#include "std_macros.h"
#include <avr/io.h>
#define clearscreen 0x01
#define returnhome 0x02
#define clearscreen 0x01
#define CURSOR_ON_DISPLAN_ON 0x0e
#define ENTRY_MODE 0x06
#define FOUR_BITS 0x28
#define EIGHT_BITS 0x38
void enable();
void lcd_cmd(char cmd);
void lcd_char(char data);
void lcd_string(char * data);
void movecurse(char row , char colum);
void lcd_init();
#endif /* LCD_H_ */