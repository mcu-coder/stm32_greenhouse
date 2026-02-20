#ifndef __FAN_H
#define	__FAN_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"



/***************根据自己需求更改****************/
// 风扇模块 GPIO宏定义

#define	FAN_CLK							RCC_APB2Periph_GPIOB

#define FAN_GPIO_PIN 				GPIO_Pin_1

#define FAN_GPIO_PROT 			GPIOB

#define FAN_ON 		GPIO_SetBits(FAN_GPIO_PROT,FAN_GPIO_PIN)
#define FAN_OFF 	GPIO_ResetBits(FAN_GPIO_PROT,FAN_GPIO_PIN)


/*********************END**********************/

void FAN_Init(void);

#endif



