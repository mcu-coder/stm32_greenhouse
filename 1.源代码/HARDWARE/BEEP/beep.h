#ifndef	__BEEP_H
#define	__BEEP_H

#include "stm32f10x.h"                  // Device header

#define BEEP_GPIO_PROT		GPIOC
#define BEEP_GPIO_PIN	  	GPIO_Pin_13

void BEEP_Init(void);
void BEEP_Toggle(void);
void BEEP_On(void);
void BEEP_Off(void);
void BEEP_Twinkle(void);
#endif
