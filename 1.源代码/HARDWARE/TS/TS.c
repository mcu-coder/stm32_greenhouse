#include "TS.h"



void TS_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (TS_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
		GPIO_InitStructure.GPIO_Pin = TS_GPIO_PIN;					// 配置 ADC IO 引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
		
		GPIO_Init(TS_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO

		ADCx_Init();
}

uint16_t TS_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
	return ADC_GetValue(ADC_CHANNEL, ADC_SampleTime_55Cycles5);
}

uint16_t TS_GetData(void)
{
	uint32_t  tempData = 0;
	for (uint8_t i = 0; i < TS_READ_TIMES; i++)
	{
		tempData += TS_ADC_Read();
		delay_ms(5);
	}

	tempData /= TS_READ_TIMES;
	return 100 - (float)tempData/40.96;
}



