#ifndef __TS_H
#define	__TS_H
#include "stm32f10x.h"
#include "adcx.h"
#include "delay.h"
#include "math.h"



#define TS_READ_TIMES	10  //土壤湿度ADC循环读取次数


/***************根据自己需求更改****************/
// TS GPIO宏定义

#define		TS_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	TS_GPIO_PORT							GPIOA
#define 	TS_GPIO_PIN								GPIO_Pin_0
#define   ADC_CHANNEL               ADC_Channel_0	// ADC 通道宏定义

/*********************END**********************/


void TS_Init(void);
uint16_t TS_GetData(void);

#endif /* __ADC_H */

