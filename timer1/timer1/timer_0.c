/*
 * timer_0.c
 *
 * Created: 7/5/2023 8:02:08 PM
 *  Author: abdelrhman mattar
 */ 
#include <avr/io.h>
#include "timer_0.h"
#include "std_macros.h"


void TIMER_INTERUPPT_ENABLE(interrupt_enable EN)
{
		SET_BIT(TIMSK,EN);
}
void TIMER_INTERUPPT_DISABLE(interrupt_enable EN)
{
	CLR_BIT(TIMSK,EN);
}


void Timer0_Vinit(TIMER0_MODES t_mode , TIMER0_PRESCALAR scale, OC_MODE mode ,uint8_t timer_data)
{
	//TIMER MODE
	if(t_mode == Normal_Mode)TCNT0 = timer_data;
	else OCR0=timer_data;
		
	switch (t_mode)
	{
		case Normal_Mode:
		CLR_BIT(TCCR0 , WGM00);
		CLR_BIT(TCCR0 , WGM01);
		break;
		
		case PWM:
		SET_BIT(TCCR0 , WGM00);
		CLR_BIT(TCCR0 , WGM01);
		break;
		
		case  CTC_Mode:
		CLR_BIT(TCCR0 , WGM00);
		SET_BIT(TCCR0 , WGM01);
		break;
		
		case Fast_PWM:
		SET_BIT(TCCR0 , WGM00);
		SET_BIT(TCCR0 , WGM01);
		SET_BIT(DDRB,3);
		break;
	}
	
	//OC-mode
	CLR_BIT(TCCR0,COM00);
	CLR_BIT(TCCR0,COM01);
	TCCR0 |=(mode<<4);
	
	//PRESCALAR
	TCCR0 &= 0XF8;
	TCCR0|= scale;
		

}


void Timer2_Vinit(TIMER0_MODES t_mode , TIMER2_PRESCALAR scale ,OC_MODE mode, uint8_t timer_data)
{
		//TIMER MODE
		if(t_mode == Normal_Mode)TCNT2 = timer_data;
		else OCR2=timer_data;
		
		switch (t_mode)
		{
			case Normal_Mode:
			CLR_BIT(TCCR2 , WGM20);
			CLR_BIT(TCCR2 , WGM21);
			break;
			
			case PWM:
			SET_BIT(TCCR2 , WGM20);
			CLR_BIT(TCCR2 , WGM21);
			SET_BIT(DDRD,7);
			break;
			
			case  CTC_Mode:
			CLR_BIT(TCCR2 , WGM20);
			SET_BIT(TCCR2 , WGM21);
			break;
			
			case Fast_PWM:
			SET_BIT(TCCR2 , WGM20);
			SET_BIT(TCCR2 , WGM21);
			SET_BIT(DDRD,7);
			break;
		}
		
		//OC-mode
		CLR_BIT(TCCR2,COM20);
		CLR_BIT(TCCR2,COM21);
		TCCR2 |=(mode<<4);
		
		//PRESCALAR
		TCCR2 &= 0XF8;
		TCCR2|= scale;
}



void real_time_clock(TIMER0_MODES t_mode , TIMER2_PRESCALAR scale ,OC_MODE mode, uint8_t timer_data)
{
	SET_BIT(ASSR,AS2);
	Timer2_Vinit(Normal_Mode,PRESCALAR2_128,Togle,0);
}

/*void timer2_overflow_init_interrupt(void)
{
		SET_BIT(ASSR,AS2);
		Timer2_Vinit(Normal_Mode,PRESCALAR2_128,Togle,0);
	
	/* select timer clock */
	/*SET_BIT(ASSR,AS2);
	TCCR2 |=(1<<4);
	/* select timer pre scalar
	TCCR2|=(PRESCALAR2_128);
}*/
	
	
	
	
	
	
	
	
/*void timer1_A_UNIT(TIMER1_MODES TMODE , TIMER0_PRESCALAR scale , OC_MODE mode , uint16_t timer_data)
{
	if (TMODE==NORMAL)
	{
		TCNT1=timer_data;
	}
	else OCR1A=timer_data;
	
	switch(TMODE)
	{
		case NORMAL:
		
		break;
		
		case F_PWM_8BIT:
		
		break;
		
		case F_PWM_9BIT:
		
		break;
		
		case F_PWM_10BIT:
		
		break;
		
		case CTC_OCR1:
		break;
		
		case F_PWM_8BIT:
		break;
		
		
		
		
	}
	
	
}*/

uint16_t input_capture_rising()
{
	uint16_t X;
	TCCR1B=0xc1;
	while(READ_BIT(TIFR , ICF1)==0)
	{
		SET_BIT(PORTB,0);
	}
	X=ICR1;	
	return X;
}

void ultra_init()
{
	TCCR1A=0;
	TIFR=(1<<ICF1);
}
uint16_t input_capture_falling()
{
	SET_BIT(TIFR,ICF1);
	uint16_t A;
	
	TCCR1B=0x81;
	CLR_BIT(PORTB,2);
	while(READ_BIT(TIFR , ICF1)==0)
	{
		SET_BIT(PORTB,2);
	}
	A=ICR1;	
	return A;
}
void STOP_TIMER()
{
	TCNT1=0;
	TCCR1B=0;
}