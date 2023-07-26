/*
 * timer1.c
 *
 * Created: 7/12/2023 6:30:11 PM
 * Author : abdelrhman mattar
 */ 

#include <avr/io.h>
#define F_CPU 8000000ul
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include "timer_0.h"
#include "std_macros.h"
#include "UART.h"
#include "servo.h"
volatile uint32_t a=0,b=0,c=0,distance;
volatile int8_t x=0;

int main(void)
{
	UART_Vinit(9600);
	_delay_ms(100);
	sei();
	int16_t i=0;
	
	SET_BIT(DDRD,5);
	SET_BIT(DDRB,0);
	SET_BIT(DDRB,1);
	SET_BIT(DDRB,2);
	SET_BIT(DDRD,7);
	
	servo_int();
	servo_speed(90);
	_delay_ms(1000);
	char arr[10];
	char brr[10],cbb[10];	
    while (1) 
    {
		
		distance=0;
		for (i=0;i<=175;i+=x)
		{
			servo_int();
			servo_speed(i);
			
			_delay_ms(25);		
			//trigger
			ultra_init();
			SET_BIT(PORTB,1);
			_delay_us(50);
			CLR_BIT(PORTB,1);
			
			
			//distance 
			/*
			TCCR1A = 0;
			TIFR = (1<<ICF1);  	// Clear ICF (Input Capture flag)  
			SET_BIT(PORTB,1);
			_delay_us(50);
			CLR_BIT(PORTB,1);
	
			TCCR1B = 0xc1;  	// Rising edge, no prescaler , noise canceler
			while ((TIFR&(1<<ICF1)) == 0);
			a = ICR1;  		// Take value of capture register
			TIFR = (1<<ICF1);  	// Clear ICF flag 
			
			
			TCCR1B = 0x81;  	// Falling edge, no prescaler ,noise canceler
			while ((TIFR&(1<<ICF1)) == 0);
			b = ICR1;  		// Take value of capture register 
			TIFR = (1<<ICF1);  	// Clear ICF flag 
			
			
			TCNT1=0;
			TCCR1B = 0;
			
			TOG_BIT(PORTB,0);
			_delay_ms(1000);*/
			
			a=input_capture_rising();
			b=input_capture_falling();
			STOP_TIMER();
	
			c=b-a;
			distance=((c*34200)/(F_CPU*2));
			if(distance>0 && distance<=40)
			{
				sprintf(arr,"%d",distance); // distance
				sprintf(brr,"%d",i); // angle
				
				strcpy(cbb , arr);
				strcat(cbb," ");
				strcat(cbb , brr);
				strcat(cbb,"\n");
				
				UART_SEND_STRING(cbb);
			}
			else
			{
				sprintf(brr,"%d",i);
				strcpy(cbb,"-1 ");
				strcat(cbb,brr);
				strcat(cbb , "\n");
				
				UART_SEND_STRING(cbb);
			}
			_delay_ms(200);
			
		}
		
		
		for (i=180;i>=5;i-=x)
		{
			servo_int();
			servo_speed(i);
			
			_delay_ms(25);			
			//trigger
			ultra_init();
			SET_BIT(PORTB,1);
			_delay_us(50);
			CLR_BIT(PORTB,1);			

			a=input_capture_rising();
			b=input_capture_falling();
			STOP_TIMER();
			
	
			c=b-a;
			distance=((c*34200)/(F_CPU*2));
			if(distance>0 && distance<=40)
			{
				sprintf(arr,"%d",distance); // distance
				sprintf(brr,"%d",i); // angle
				
				strcpy(cbb , arr);
				strcat(cbb," ");
				strcat(cbb , brr);
				strcat(cbb,"\n");
				
				UART_SEND_STRING(cbb);
			}
			else
			{
				sprintf(brr,"%d",i);
				strcpy(cbb,"-1 ");
				strcat(cbb,brr);
				strcat(cbb , "\n");
				
				UART_SEND_STRING(cbb);
			}
			_delay_ms(200);
			
		}		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/*for (i=180;i>=0;i-=5)
		{
			//trigger
			SET_BIT(PORTB,7);
			_delay_us(50);
			CLR_BIT(PORTB,7);
			servo_speed(i);
			//distance
			a=input_capture_rising();
			b=input_capture_falling();
			STOP_TIMER();
			
			c=b-a;
			distance=((c*34600)/(F_CPU*2));
			if(distance>0 && distance<80)
			{
				sprintf(arr,"%d",distance); // distance
				sprintf(brr,"%d",i); // angle
				
				strcpy(cbb , arr);
				strcat(cbb," ");
				strcat(cbb , brr);
				
				UART_SEND_STRING(cbb);
				
				_delay_us(100);
			}
			else
			{
				UART_SEND_STRING("-1 -1");
			}
			
		}*/
		
		
		/*ICR1=0;
		TCNT1=0;
		movecurse(1,1);

		a=input_capture_rising();
		b=input_capture_falling();
		c=input_capture_rising();
		STOP_TIMER();
		
		b-=a;
		c-=a;

		
		if(b>c )
		{
			b=0;
		}
		
		
		sprintf(brr,"%d",b);
		sprintf(cbb,"%d",c);
		
		
		lcd_string(brr);
		_delay_ms(100);
		movecurse(2,1);
		lcd_string(cbb);
		_delay_ms(1000);*/
    }
}
ISR(USART_RXC_vect)
{
	CLR_BIT(UCSRB ,TXEN );
	CLR_BIT(UCSRB ,RXEN );
	if (x!=0)x=0;
	else x=5;
	SET_BIT(UCSRB ,TXEN );
	SET_BIT(UCSRB ,RXEN );
}


