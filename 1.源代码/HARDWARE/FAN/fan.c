#include "fan.h"




void FAN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(FAN_CLK, ENABLE ); //≈‰÷√ ±÷”
	
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FAN_GPIO_PROT,&GPIO_InitStructure);

	FAN_OFF;
}

