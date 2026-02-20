#ifndef	__MODULES_H_
#define __MODULES_H_

#include "stm32f10x.h"                  // Device header
#include "dht11.h"
#include "BH1750.h"
#include "TS.h"
#include "ph.h"
#include "adcx.h"
#include "usart3.h"

typedef struct
{
	uint8_t humi;
	uint8_t temp;
	uint8_t soil_temp;
	uint16_t lux;	
	uint8_t soil_Humi;
	uint16_t CO2;
	float ph;
	
}SensorModules;

typedef struct
{
	uint8_t humiValue;
	uint8_t tempValue;
	uint8_t soil_tempValue;
	uint16_t luxValue;	
	uint8_t soil_HumiValue;
	uint16_t CO2Value;	
	uint8_t ph_H;
	uint8_t ph_L;

	
}SensorThresholdValue;

typedef struct
{
	uint8_t LED_Flag;
	uint8_t Fan_Flag;
	uint8_t Bump_Flag;
	uint8_t Beep_Flag;
	uint8_t Curtain_Flag;
	uint8_t NOW_Curtain_Flag;
	
}DriveModules;
 	

extern SensorModules sensorData;			//声明传感器模块的结构体变量
extern SensorThresholdValue Sensorthreshold;	//声明传感器阈值结构体变量
extern DriveModules driveData;				//声明驱动器状态的结构体变量
void SensorScan(void);

#endif
