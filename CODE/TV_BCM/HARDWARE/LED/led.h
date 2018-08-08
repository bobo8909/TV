#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#if STM32_EVALUATION
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	
#else
#define LED0 PCout(13)// PC13
#endif
#define LED_ON 1
#define LED_OFF 0
void LED_Init(void);//≥ı ºªØ
void Led_task(void);

		 				    
#endif
