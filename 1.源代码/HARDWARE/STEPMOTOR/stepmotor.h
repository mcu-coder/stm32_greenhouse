#ifndef __STEPMOTOR_H
#define	__STEPMOTOR_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"



/***************根据自己需求更改****************/
// 步进电机 GPIO宏定义

#define	MOTOR_CLK				RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC

#define MOTOR_A 				GPIO_Pin_8
#define MOTOR_B 				GPIO_Pin_9
#define MOTOR_C 				GPIO_Pin_12
#define MOTOR_D 				GPIO_Pin_15
#define MOTOR_AB_PORT 			GPIOB
#define MOTOR_CD_PORT 			GPIOA

#define MOTOR_A_HIGH GPIO_SetBits(MOTOR_AB_PORT,MOTOR_A)
#define MOTOR_A_LOW GPIO_ResetBits(MOTOR_AB_PORT,MOTOR_A)
 
#define MOTOR_B_HIGH GPIO_SetBits(MOTOR_AB_PORT,MOTOR_B)
#define MOTOR_B_LOW GPIO_ResetBits(MOTOR_AB_PORT,MOTOR_B)

#define MOTOR_C_HIGH GPIO_SetBits(MOTOR_CD_PORT,MOTOR_C)
#define MOTOR_C_LOW GPIO_ResetBits(MOTOR_CD_PORT,MOTOR_C)

#define MOTOR_D_HIGH GPIO_SetBits(MOTOR_CD_PORT,MOTOR_D)
#define MOTOR_D_LOW GPIO_ResetBits(MOTOR_CD_PORT,MOTOR_D)

/*********************END**********************/

void MOTOR_Init(void);
void MOTOR_Rhythm_4_1_4(u8 step,u8 dly);
void MOTOR_Direction(u8 dir,u8 num,u8 dly);
void MOTOR_Direction_Angle(u8 dir,u8 num,u16 angle,u8 dly);
void MOTOR_STOP(void);
#endif



