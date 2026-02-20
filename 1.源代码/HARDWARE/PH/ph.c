#include "ph.h"

/*****************辰哥单片机设计******************
											STM32
 * 文件			:	PH传感器c文件                   
 * 版本			: V1.0
 * 日期			: 2024.2.4
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥

**********************BEGIN***********************/

void PH_Init(void)
{
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (PH_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
		GPIO_InitStructure.GPIO_Pin = PH_GPIO_PIN;					// 配置 ADC IO 引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
		
		GPIO_Init(PH_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO

		ADCx_Init();
	}

}


uint16_t PH_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
	return ADC_GetValue(ADC_CHANNEL_PH, ADC_SampleTime_55Cycles5);
}

float PH_GetData(void)
{
	float  tempData = 0;
	float PH_DAT;

	for (uint8_t i = 0; i < PH_READ_TIMES; i++)
	{
		tempData += PH_ADC_Read();
		delay_ms(5);
	}
	tempData /= PH_READ_TIMES;
	
	PH_DAT = (tempData/4096.0)*3.3;
	PH_DAT = -5.7541*PH_DAT+16.654;
	
	if(PH_DAT>14.0)PH_DAT=14.0;
	if(PH_DAT<0)PH_DAT=0.0;
	
	return PH_DAT;

}



