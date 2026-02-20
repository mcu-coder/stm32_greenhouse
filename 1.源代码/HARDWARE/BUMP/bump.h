#ifndef __BUMP_H
#define	__BUMP_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"


/***************根据自己需求更改****************/
// 水泵模块 GPIO宏定义

#define	BUMP_CLK							RCC_APB2Periph_GPIOB

#define BUMP_GPIO_PIN 				GPIO_Pin_3

#define BUMP_GPIO_PROT 				GPIOB

#define BUMP_ON 		GPIO_SetBits(BUMP_GPIO_PROT,BUMP_GPIO_PIN)
#define BUMP_OFF 		GPIO_ResetBits(BUMP_GPIO_PROT,BUMP_GPIO_PIN)

/*********************END**********************/

void BUMP_Init(void);

#endif



