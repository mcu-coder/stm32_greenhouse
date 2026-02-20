#include "bump.h"



void BUMP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BUMP_CLK, ENABLE ); //≈‰÷√ ±÷”
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BUMP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BUMP_GPIO_PROT,&GPIO_InitStructure);

	BUMP_OFF;
}


