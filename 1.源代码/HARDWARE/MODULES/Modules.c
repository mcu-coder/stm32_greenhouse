# include "Modules.h"


/**
  * @brief  传感器数据扫描
  * @param  无
  * @retval 无
  */

void SensorScan(void)
{
	DHT11_Read_Data(&sensorData.temp, &sensorData.humi);//读取DHT11温湿度数值
	sensorData.ph = PH_GetData();
	sensorData.lux = bh_data_read();//读取光敏电阻数值
	sensorData.soil_Humi = TS_GetData();//读取土壤湿度传感器数值
	CO2GetData(&sensorData.CO2);//读取JW01二氧化碳数值
	
}


