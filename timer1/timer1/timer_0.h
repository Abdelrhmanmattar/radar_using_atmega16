/*
 * timer_0.h
 *
 * Created: 7/5/2023 8:01:53 PM
 *  Author: abdelrhman mattar
 */ 


#ifndef TIMER_0_H_
#define TIMER_0_H_
typedef enum {
	Normal_Mode=0,
	PWM,
	CTC_Mode,
	Fast_PWM,
}
TIMER0_MODES;


typedef enum {
	STOP=0,
	PRESCALAR_1,
	PRESCALAR_8,
	PRESCALAR_64,
	PRESCALAR_256,
	PRESCALAR_1024,
	EXTERNAL_FALLING_EDGE,
	EXTERNAL_RISINGING_EDGE,
}
TIMER0_PRESCALAR;

typedef enum {
	STOP2=0,
	PRESCALAR2_1,
	PRESCALAR2_8,
	PRESCALAR2_32,
	PRESCALAR2_64,
	PRESCALAR2_128,
	PRESCALAR2_256,
	PRESCALAR2_1024,
}
TIMER2_PRESCALAR;


typedef enum{
	Dis_Connect=0,
	Togle=1,
	Clear=2,
	Set=3,
	non_inverting_mode=2,
	inverting_mode=3,
}
OC_MODE;




typedef enum{
	timer0_overflow=0,
	timer0_CTC,
	timer1_overflow,
	timer1_CTC_B,
	timer1_CTC_A,
	timer1_capature,
	timer2_overflow,
	timer2_CTC,
}
interrupt_enable;



void TIMER_INTERUPPT_ENABLE(interrupt_enable EN);
void TIMER_INTERUPPT_DISABLE(interrupt_enable EN);


void Timer0_Vinit(TIMER0_MODES t_mode , TIMER0_PRESCALAR scale ,OC_MODE mode, uint8_t timer_data);


void Timer2_Vinit(TIMER0_MODES t_mode , TIMER2_PRESCALAR scale ,OC_MODE mode, uint8_t timer_data);

void real_time_clock(TIMER0_MODES t_mode , TIMER2_PRESCALAR scale ,OC_MODE mode, uint8_t timer_data);
//void timer2_overflow_init_interrupt(void);


//timer_1


typedef enum{
	NORMAL=0,
	PWN_8BIT,
	PWN_9BIT,
	PWN_10BIT,
	CTC_OCR1,
	F_PWM_8BIT,
	F_PWM_9BIT,
	F_PWM_10BIT,
	PWM_PH_FR_CORRECT_ICR,
	PWM_PH_FR_CORRECT_OCR1,
	PWN_PH_16BIT_ICR,
	PWN_PH_16BIT_OCR1,
	CTC_ICR,
	NONE,
	F_PWM_16_ICR1,
	F_PWM_16_OCR1,
}
TIMER1_MODES;

//PRESCALAR_0
//MODE_0


//void timer1_A_UNIT(TIMER1_MODES TMODE , TIMER0_PRESCALAR scale , OC_MODE mode , uint8_t timer_data);


//void timer1_B_UNIT(TIMER1_MODES TMODE , TIMER0_PRESCALAR scale , OC_MODE mode , uint8_t timer_data);


uint16_t input_capture_rising();

uint16_t input_capture_falling();
void STOP_TIMER();
void ultra_init();

#endif /* TIMER_0_H_ */