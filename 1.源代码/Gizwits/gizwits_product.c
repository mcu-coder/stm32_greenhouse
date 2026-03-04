/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart2.h"
#include "fan.h"
#include "bump.h"
#include "led.h"
#include "stepmotor.h"
#include "Modules.h"
#include "key.h"
#include "OLED.h"
#include "ph.h"
static uint32_t timerMsCount;
extern int mode;
extern u8 KeyNum;
extern uint8_t count_m;
static uint32_t timerMsCount;
/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;
    
#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_fan:
            currentDataPoint.valuefan = dataPointPtr->valuefan;
            GIZWITS_LOG("Evt: EVENT_fan %d \n", currentDataPoint.valuefan);
            if(0x01 == currentDataPoint.valuefan)
            {
            //user handle
							count_m = 2;
							KeyNum = 3;
            }
            else
            {
            //user handle   
							count_m = 2;
							KeyNum = 4;							
            }
            break;
        case EVENT_water_pump:
            currentDataPoint.valuewater_pump = dataPointPtr->valuewater_pump;
            GIZWITS_LOG("Evt: EVENT_water_pump %d \n", currentDataPoint.valuewater_pump);
            if(0x01 == currentDataPoint.valuewater_pump)
            {
            //user handle
							count_m = 3;
							KeyNum = 3;
            }
            else
            {
            //user handle   
							count_m = 3;
							KeyNum = 4;							
            }
            break;
        case EVENT_ventilation_opening:
            currentDataPoint.valueventilation_opening = dataPointPtr->valueventilation_opening;
            GIZWITS_LOG("Evt: EVENT_ventilation_opening %d \n", currentDataPoint.valueventilation_opening);
            if(0x01 == currentDataPoint.valueventilation_opening)
            {
            //user handle
							count_m =4;
							KeyNum = 3;
            }
            else
            {
            //user handle
							count_m =4;
							KeyNum = 4;
            }
            break;
        case EVENT_LED:
            currentDataPoint.valueLED = dataPointPtr->valueLED;
            GIZWITS_LOG("Evt: EVENT_LED %d \n", currentDataPoint.valueLED);
            if(0x01 == currentDataPoint.valueLED)
            {
            //user handle
							count_m = 1;
							KeyNum = 3;
            }
            else
            {
            //user handle
							count_m = 1;
							KeyNum = 4;
            }
            break;

        case EVENT_Model:
            currentDataPoint.valueModel = dataPointPtr->valueModel;
            GIZWITS_LOG("Evt: EVENT_Model %d\n", currentDataPoint.valueModel);
            switch(currentDataPoint.valueModel)
            {
            case Model_VALUE0:
                //user handle
								mode = 0;
								OLED_Clear();
                break;
            case Model_VALUE1:
                //user handle
								mode = 1;
								OLED_Clear();
                break;
            default:
                break;
            }
            break;



        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
 
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M:
 
            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",gprsInfoData->Type);
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
        break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 currentDataPoint.valuebuzzer = driveData.Beep_Flag;//Add Sensor Data Collection
    currentDataPoint.valueTemperature = sensorData.temp;//Add Sensor Data Collection
    currentDataPoint.valueHumidity = sensorData.humi;//Add Sensor Data Collection
    currentDataPoint.valueph = sensorData.ph;
    currentDataPoint.valuesoil_humidity = sensorData.soil_Humi;//Add Sensor Data Collection
    currentDataPoint.valueLUX = sensorData.lux;//Add Sensor Data Collection
    currentDataPoint.valueCO2 = sensorData.CO2;//Add Sensor Data Collection

    currentDataPoint.valuefan = driveData.Fan_Flag;
    currentDataPoint.valuewater_pump = driveData.Bump_Flag;
    currentDataPoint.valueventilation_opening = driveData.NOW_Curtain_Flag;
	  currentDataPoint.valueLED = driveData.LED_Flag;
	  currentDataPoint.valueModel = mode;
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 

    currentDataPoint.valuefan = 0;
    currentDataPoint.valuewater_pump = 0;
    currentDataPoint.valueventilation_opening = 0;
    currentDataPoint.valueLED = 0;
    currentDataPoint.valuebuzzer = 0;
    currentDataPoint.valueModel = 0;
    currentDataPoint.valueTemperature = 0;
    currentDataPoint.valueHumidity = 0;
    currentDataPoint.valueph = 0;
    currentDataPoint.valuesoil_humidity = 0;
    currentDataPoint.valueLUX = 0;
    currentDataPoint.valueCO2 = 0;

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
		__set_FAULTMASK(1);
		NVIC_SystemReset();
	
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
         USART_SendData(USART2, buf[i]);//STM32 test demo
			  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        //Serial port to achieve the function, the buf[i] sent to the module
        if(i >=2 && buf[i] == 0xFF)
        {
					USART_SendData(USART2,0x55);
					while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
          //Serial port to achieve the function, the 0x55 sent to the module
          //USART_SendData(UART, 0x55);//STM32 test demo
        }
    }


    
    return len;
}


