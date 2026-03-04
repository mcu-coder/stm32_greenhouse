#ifndef __PH_H
#define	__PH_H
#include "stm32f10x.h"
#include "adcx.h"
#include "delay.h"
#include "math.h"



#define PH_READ_TIMES	10  //PH传感器ADC循环读取次数

/***************根据自己需求更改****************/
// PH GPIO宏定义

#define		PH_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	PH_GPIO_PORT							GPIOA
#define 	PH_GPIO_PIN								GPIO_Pin_1
#define   ADC_CHANNEL_PH								ADC_Channel_1	// ADC 通道宏定义


/*********************END**********************/


void PH_Init(void);
float PH_GetData(void);

#endif /* __PH_H */

