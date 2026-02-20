#include "stm32f10x.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "delay.h"
#include "dht11.h"
#include "BH1750.h"
#include "TS.h"
#include "ph.h"
#include "oled.h"
#include "key.h"
#include "Modules.h"
#include "TIM2.h"
#include "TIM3.h"
#include "adcx.h"
#include "flash.h"
#include "stepmotor.h"
#include "fan.h"
#include "bump.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
#include "stm32f10x_iwdg.h"
 

#define KEY_Long1	11

#define KEY_1	1
#define KEY_2	2
#define KEY_3	3
#define KEY_4	4

#define FLASH_START_ADDR	0x0801f000	//写入的起始地址

//传感器变量声明
u8 temp;//温度变量
u8 humi;//湿度变量
u16 lux;//光照值变量
u16 soil_Humi;//土壤湿度变量
u16 CO2;//CO2变量


SensorModules sensorData;								//声明传感器数据结构体变量
SensorThresholdValue Sensorthreshold;		//声明传感器阈值结构体变量
DriveModules driveData;									//声明驱动器状态结构体变量

int mode = 0;	//系统模式  1自动  2手动  3设置

//系统静态变量
static uint8_t count_a = 1;  //自动模式按键数
 uint8_t count_m = 1;  //手动模式按键数
static uint8_t count_s = 1;	 //设置模式按键数

extern volatile uint8_t motor_busy;  // 电机忙标志
/**
  * @brief  显示菜单内容
  * @param  无
  * @retval 无
  */
enum 
{
	AUTO_MODE = 0,
	MANUAL_MODE,
	SETTINGS_MODE
	
}MODE_PAGES;

/**
  * @brief  显示菜单1的固定内容
  * @param  无
  * @retval 无
  */
void OLED_autoPage1(void)		//自动模式菜单第一页
{
	//显示“环境温度：  ℃”
	OLED_ShowChinese(0,0,28,16,1); 	//环
	OLED_ShowChinese(16,0,29,16,1);	//境
	OLED_ShowChinese(32,0,0,16,1);	//温
	OLED_ShowChinese(48,0,2,16,1);	//度
	OLED_ShowChar(64,0,':',16,1);
	 
	
	OLED_Refresh();
	
}
void OLED_autoPage2(void)   //自动模式菜单第二页
{
	
	//显示“光照强度：  Lux”
	OLED_ShowChinese(0,0,3,16,1); //光
	OLED_ShowChinese(16,0,4,16,1);//照
	OLED_ShowChinese(32,0,5,16,1);//强
	OLED_ShowChinese(48,0,2,16,1);//度
	OLED_ShowChar(64,0,':',16,1);
	 
	OLED_Refresh();
}

void SensorDataDisplay1(void)		//传感器数据显示第一页
{
	uint8_t ph_point = sensorData.ph*10; 
	//显示温度数据
	OLED_ShowNum(90,0,sensorData.temp,2,16,1);
	//显示湿度数据
	OLED_ShowNum(90,16,sensorData.humi,2,16,1);
	//显示ph数据
	OLED_ShowNum(90,32,sensorData.ph,1,16,1);
	OLED_ShowString(98,32,".",16,1);//"."
	OLED_ShowNum(106,32,ph_point%10,1,16,1);
	//显示土壤湿度数据
	OLED_ShowNum(90,48,sensorData.soil_Humi,2,16,1);

}

void SensorDataDisplay2(void)		//传感器数据显示第二页
{

	//显示光照强度数据
	OLED_ShowNum(90,0,sensorData.lux,4,16,1); 

}

/**
  * @brief  显示手动模式设置界面1
  * @param  无
  * @retval 无
  */
void OLED_manualPage1(void)
{
	//显示“灯光：”
	OLED_ShowChinese(16,0,18,16,1);	
	OLED_ShowChinese(32,0,19,16,1);	
	OLED_ShowChar(64,0,':',16,1);

	 
}



/**
  * @brief  显示手动模式设置参数界面1
  * @param  无
  * @retval 无
  */
void ManualSettingsDisplay1(void)
{
	if(driveData.LED_Flag)
	{
		OLED_ShowChinese(96,0,24,16,1); 	//开
	}
	else
	{
		OLED_ShowChinese(96,0,26,16,1); 	//关
	}
	 
}


/**
  * @brief  显示系统阈值设置界面1
  * @param  无
  * @retval 无
  */
void OLED_settingsPage1(void)
{
	//显示“温度阈值”
	OLED_ShowChinese(16,0,0,16,1);	
	OLED_ShowChinese(32,0,2,16,1);	
	OLED_ShowChinese(48,0,16,16,1);	
	OLED_ShowChinese(64,0,17,16,1);	
	OLED_ShowChar(80,0,':',16,1);
 
	
}

void OLED_settingsPage2(void)
{
 
	//显示“PH下限”
	OLED_ShowString(16,32,"PH",16,1);
	OLED_ShowChinese(32,32,43,16,1);	
	OLED_ShowChinese(48,32,42,16,1);	
	OLED_ShowChar(80,32,':',16,1);
}

void SettingsThresholdDisplay1(void)
{
	//显示温度阈值数值
	OLED_ShowNum(90,0, Sensorthreshold.tempValue, 2,16,1); 
}

void SettingsThresholdDisplay2(void)
{
	//显示CO2浓度阈值数值
	OLED_ShowNum(90,0, Sensorthreshold.CO2Value, 4,16,1);
	 
	
		
}
	
/**
  * @brief  记录自动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetAuto(void)  
{
	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_a++;
		if (count_a > 2)
		{
			count_a = 1;
		}
		OLED_Clear();
	}
	return count_a;
}

/**
  * @brief  记录手动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetManual(void)  
{

	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_m++;
		if (count_m > 4)  		//一共可以控制的外设数量
		{
			count_m = 1;
		}
	}
	return count_m;
}

/**
  * @brief  记录阈值界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetSelection(void)
{
	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_s++;
		if (count_s == 5)
		{
			OLED_Clear();
		}
		else if (count_s > 7)
		{
			OLED_Clear();
			count_s = 1;
		}
	}
	return count_s;
}


/**
  * @brief  显示手动模式界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_manualOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
	 
		default: break;
	}
}

/**
  * @brief  显示阈值界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_settingsOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
		case 2:	
			OLED_ShowChar(0, 0,' ',16,1);
			OLED_ShowChar(0,16,'>',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
	 
		default: break;
	}
}

/**
  * @brief  自动模式控制函数
  * @param  无
  * @retval 无
  */
void AutoControl(void)
{
	if(sensorData.temp>Sensorthreshold.tempValue||sensorData.humi>Sensorthreshold.humiValue)
		driveData.Fan_Flag = 1;
	else
		driveData.Fan_Flag = 0;
	
	 
	
}

/**
  * @brief  手动模式控制函数
  * @param  无
  * @retval 无
  */
void ManualControl(uint8_t num)
{
	switch(num)
	{
		case 1:	
			//显示外设开关
			if(KeyNum == KEY_3)
				driveData.LED_Flag = 1;
			if(KeyNum == KEY_4)
				driveData.LED_Flag = 0;
			break;
		case 2:	
			if(KeyNum == KEY_3)
				driveData.Fan_Flag = 1 ;
			if(KeyNum == KEY_4)
				driveData.Fan_Flag = 0;
			break;
		case 3:	
			if(KeyNum == KEY_3)
				driveData.Bump_Flag = 1;
			if(KeyNum == KEY_4)
				driveData.Bump_Flag = 0;
			break;
		case 4:	
			if(KeyNum == KEY_3)
				driveData.NOW_Curtain_Flag = 1;
			if(KeyNum == KEY_4)
				driveData.NOW_Curtain_Flag = 0;
			break;
	
		default: break;
	}

}

/**
  * @brief  控制函数
  * @param  无
  * @retval 无
  */
void Control_Manager(void)
{
	if(driveData.LED_Flag)
	{
		LED_On();
	}
	else
	{
		LED_Off();
	}

	
	if(driveData.Fan_Flag)
	{
		FAN_ON;
	}
	else
	{
		FAN_OFF;
	}
	
	if(driveData.Bump_Flag)
	{
		BUMP_ON;
	}
	else
	{
		BUMP_OFF;
	}
	
	if(driveData.Beep_Flag)
	{
		BEEP_On();
	}
	else
	{
		BEEP_Off();
	}
	
	if(driveData.NOW_Curtain_Flag!=driveData.Curtain_Flag)
	{
		driveData.Curtain_Flag=driveData.NOW_Curtain_Flag;
		if(driveData.NOW_Curtain_Flag)
		{
			MOTOR_Direction_Angle(1,0,90,1);
		}
		else
		{
			MOTOR_Direction_Angle(0,0,90,1);
		}
	
	}
}

/**
  * @brief  阈值设置函数
  * @param  无
  * @retval 无
  */
void ThresholdSettings(uint8_t num)
{
	switch (num)
	{
		case 1:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.tempValue += 1;
				if (Sensorthreshold.tempValue > 40)
				{
					Sensorthreshold.tempValue = 20;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				Sensorthreshold.tempValue -= 1;
				if (Sensorthreshold.tempValue < 20)
				{
					Sensorthreshold.tempValue = 40;
				}				
			}
			break;
			
		case 2:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.humiValue += 1;
				if (Sensorthreshold.humiValue > 80)
				{
					Sensorthreshold.humiValue = 20;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				Sensorthreshold.humiValue -= 1;
				if (Sensorthreshold.humiValue < 20)
				{
					Sensorthreshold.humiValue = 80;
				}				
			}	
			break;
		case 3:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.soil_HumiValue+= 1;
				if (Sensorthreshold.soil_HumiValue > 50)
				{
					Sensorthreshold.soil_HumiValue = 10;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				Sensorthreshold.soil_HumiValue -= 1;
				if (Sensorthreshold.soil_HumiValue < 10)
				{
					Sensorthreshold.soil_HumiValue = 50;
				}				
			}
			break;
			
		case 4:
			if (KeyNum == KEY_3)
			{
				KeyNum = 0;
				Sensorthreshold.luxValue += 10;
				if (Sensorthreshold.luxValue > 500)
				{
					Sensorthreshold.luxValue = 0;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				Sensorthreshold.luxValue -= 10;
				if (Sensorthreshold.luxValue < 1)
				{
					Sensorthreshold.luxValue = 500;
				}				
			}
			break;
		case 5:
			if (KeyNum == KEY_3)
			{ 
				KeyNum = 0;
				Sensorthreshold.CO2Value += 20;
				if (Sensorthreshold.CO2Value > 2000)
				{
					Sensorthreshold.CO2Value = 300;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				Sensorthreshold.CO2Value -= 20;
				if (Sensorthreshold.CO2Value < 300)
				{
					Sensorthreshold.CO2Value = 2000;
				}				
			}
			break;
			case 6:
			if (KeyNum == KEY_3)
			{ 
				KeyNum = 0;
				Sensorthreshold.ph_H += 1;
				if (Sensorthreshold.CO2Value > 12)
				{
					Sensorthreshold.CO2Value = 12;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				if (Sensorthreshold.ph_H > Sensorthreshold.ph_L+1)
				{
					Sensorthreshold.ph_H-=1;
				}				
			}
			break;
			case 7:
			if (KeyNum == KEY_3)
			{ 
				KeyNum = 0;
				if (Sensorthreshold.ph_L < Sensorthreshold.ph_H-1)
				{
					Sensorthreshold.ph_L+=1;
				}
			}
			else if (KeyNum == KEY_4)
			{
				KeyNum = 0;
				Sensorthreshold.ph_L-= 1;
				if (Sensorthreshold.ph_L < 1)
				{
					Sensorthreshold.ph_L = 1;
				}				
			}
			break;
		default: break;		
	}
}

void ScanGizwitsMode(void)
{
	if(!KEY3)
	{
		delay_ms(20);
		if(!KEY3)
		{
			//显示“热点配网”
	    OLED_ShowChinese(32,16,35,16,1); 	//热
	    OLED_ShowChinese(48,16,36,16,1);	//点                               
	    OLED_ShowChinese(64,16,39,16,1);	//配
	    OLED_ShowChinese(80,16,40,16,1);  //网
			gizwitsSetMode(WIFI_SOFTAP_MODE);
			OLED_Refresh();
		}
	}
	else if(!KEY4)
	{
		delay_ms(20);
		if(!KEY4)
		{
			//显示“一键配网”
	    OLED_ShowChinese(32,16,37,16,1); 	//一
	    OLED_ShowChinese(48,16,38,16,1);	//键
	    OLED_ShowChinese(64,16,39,16,1);	//配
	    OLED_ShowChinese(80,16,40,16,1);  //网
			gizwitsSetMode(WIFI_AIRLINK_MODE);
			OLED_Refresh();
		}
	}
}


int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	delay_init(72);
	
	//传感器初始化
	DHT11_Init();//DHT11模块初始化
	BH1750_Init();//BH1750传感器初始化
	TS_Init();//土壤湿度传感器初始化
	PH_Init();
	//执行器初始化
	LED_Init();//大功率LED灯初始化
	FAN_Init();//风扇初始化
	BUMP_Init();//水泵初始化
	BEEP_Init();//蜂鸣器初始化
	MOTOR_Init();//步进电机初始化
	
	Key_Init();//按键初始化
	OLED_Init();//OLED初始化
	
	Sensorthreshold.tempValue  = 35;
	Sensorthreshold.humiValue  = 70;
	Sensorthreshold.soil_HumiValue = 40;
	Sensorthreshold.luxValue  = 100;
	Sensorthreshold.CO2Value  = 800;
	Sensorthreshold.ph_H = 9;
	Sensorthreshold.ph_L = 4;

	USART1_Config();//串口1初始化
	USART2_Config();//串口2初始化
  USART3_Config();//串口3初始化
  TIM2_Init(72-1,1000-1);
	TIM3_Int_Init(1000-1,72-1);		//定时1ms中断 
		
	OLED_Clear();
	userInit();
	gizwitsInit();
	delay_ms(200);
	ScanGizwitsMode();
  delay_ms(500);
	OLED_Clear();
  while (1)
  {


		SensorScan();	//获取传感器数据
		userHandle();//上报
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//下发
		switch(mode)
		{
			case AUTO_MODE:
				if(SetAuto() ==1)
				{
					OLED_autoPage1();	//显示主页面1固定信息
					SensorDataDisplay1();	//显示传感器1数据
				}
				else
				{
					OLED_autoPage2();	//显示主页面2固定信息
					SensorDataDisplay2();	//显示传感器2数据
				}
				AutoControl();
				/*按键1按下时切换模式*/
				if (KeyNum == KEY_1)   //系统模式mode  1自动  2手动  3设置
				{
					KeyNum = 0;
					mode = MANUAL_MODE;
					count_m = 1;
					OLED_Clear();
				}
				
				if (KeyNum == KEY_Long1)
				{
					KeyNum = 0;
					mode = SETTINGS_MODE;
					count_s = 1;
					OLED_Clear();
				}
				
				Control_Manager();
				userHandle();//上报
				gizwitsHandle((dataPoint_t *)&currentDataPoint);//下发
				break;
				
			case MANUAL_MODE:
				OLED_manualOption(SetManual());
				ManualControl(SetManual());
				OLED_manualPage1();
				ManualSettingsDisplay1();
				
				if (KeyNum == KEY_1)   //系统模式mode  0手动  1自动（默认）
				{
					KeyNum = 0;
					mode = AUTO_MODE;
					count_a = 1;
					OLED_Clear();
				}
				Control_Manager();
				userHandle();//上报
				gizwitsHandle((dataPoint_t *)&currentDataPoint);//下发
				break;
				
			case SETTINGS_MODE:

				OLED_settingsOption(SetSelection());	//实现阈值设置页面的选择功能
				ThresholdSettings(SetSelection());	//实现阈值调节功能	
					
				if (SetSelection() <= 4)		
				{				
					OLED_settingsPage1();	//显示阈值设置界面1固定信息
					SettingsThresholdDisplay1();	//显示传感器阈值1数据	

				}
				else	//否则显示设置页面2
				{			
					OLED_settingsPage2();	//显示阈值设置界面2固定信息
					SettingsThresholdDisplay2();	//显示传感器阈值2数据	
				}
					//判断是否退出阈值设置界面
					if (KeyNum == KEY_1)
					{
						KeyNum = 0;
						mode = AUTO_MODE;	//跳转到主界面
						count_a = 1;
						OLED_Clear();	//清屏

					}
					userHandle();//上报
					gizwitsHandle((dataPoint_t *)&currentDataPoint);//下发
				break;
				default: break;
		}
		userHandle();//上报
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//下发


  }
}
