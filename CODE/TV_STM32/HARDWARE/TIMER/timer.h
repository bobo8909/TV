#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define CAPPWM 1
#define ARR_200Hz 9999
#define PSC_200Hz 35

#define ARR_1KHz 35999
#define PSC_1KHz 1
#if CAPPWM
#define ARR_2KHz 720
#define PSC_2KHz 0

#else
#if 1
#define ARR_2KHz 35999
#define PSC_2KHz 0
#else
#define ARR_2KHz 5999
#define PSC_2KHz 5
#endif
#endif
#define ARR_2140Hz 33643
#define PSC_2140Hz 0

#define EncoderArr (5000*4-1)
#define EncoderPSC 0
typedef union
{
	u8 Byte;
	struct 
	{
		u8 CANFlag : 1;
		u8 SpeedFlag : 1;
		u8 LedFlag : 1;
		u8 TIMAngSensorAFlag : 1;
		u8 TIMAngSensorBFlag : 1;
		u8 EncoderFlag : 1;
		u8 DACSendFlag : 1;
		u8 b7 : 1;
	}bits;
}STRUCT_TIMFLAG;
extern STRUCT_TIMFLAG g_TIMFlag;
#if 0
extern u16 AngSensorAMinus;
extern u16 AngSensorBMinus;

extern int PWM3OUT2;
extern int PWM3OUT4; 
extern int PWM4OUT2; 				
extern int PWM2OUT4; 

extern u8 PWM3OUT2Flag;

#else
extern int TIM4Flag;
extern int iHeightCountTIM4;
extern int iPeriodTIM4;

#endif
//extern u8 g_TIM2PluseCount;
//extern u8 g_CANSpeedFlag;
void TIM_INIT(void);
//u8 TIM6PluseFunc(void);

#endif
