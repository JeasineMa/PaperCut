/**********************************
文件名：macros.c
作用说明：
	实现了用户接口函数
使用说明：
	1.除CAR_SpeedSET(...)函数的实现外，建议不要修改本文件
	2.可调用函数：CAR_SpeedSET(...)
函数调用说明：
	请自行修改CAR_SpeedSET(...)函数
**********************************/
#include "macros.h"

#define MOT1 &MOT1_PID
#define MOT2 &MOT2_PID
#define MOT3 &MOT3_PID
#define MOT4 &MOT4_PID

extern PID MOT1_PID,MOT2_PID,MOT3_PID,MOT4_PID;   

/*************************************
函数名：All_Init(...)
参数：无
作用：系统初始化函数，请勿修改
*************************************/
void All_Init()
{
	SystemCoreClockUpdate();
	Bluetooth_Init();
	Motor_Init();
	Encoder_Init();
	SysInterrupts_Init();
	TIM_Cmd(TIM6,DISABLE);
	TIM_ITConfig(TIM6,TIM_IT_Update,DISABLE);
}

/*************************************
函数名：MOT_Speed(...)
参数：motorx , speed
作用：设置电机的转速
*************************************/
void MOT_Speed(Motor motor , float speed)
{
	float realSpeed;
	realSpeed=map(speed , Speed_Min_Sys , Speed_Max_Sys , Speed_Min , Speed_Max);
	switch(motor)
	{
		case motor1:
		{
			MOT_SpeedSET(MOT1,realSpeed);
			break;
		}
		case motor2:
		{
			MOT_SpeedSET(MOT2,realSpeed);
			break;
		}
		case motor3:
		{
			MOT_SpeedSET(MOT3,realSpeed);
			break;
		}
		case motor4:
		{
			MOT_SpeedSET(MOT4,realSpeed);
			break;
		}
	}
}

/*************************************
函数名：MOT_Stop(...)
参数：motorx 
作用：强行停止某个电机的转动
*************************************/
void MOT_Stop(Motor motor)
{
	switch(motor)
	{
		case motor1:
		{
			MOT1_PID.control=false;
			MOT_Speed(motor1,0);
			TIM_SetCompare1(TIM1,4000);
			TIM_SetCompare2(TIM1,4000);
			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		}
		case motor2:
		{
			MOT2_PID.control=false;
			MOT_Speed(motor2,0);
			TIM_SetCompare3(TIM1,4000);
			TIM_SetCompare4(TIM1,4000);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		}
		case motor3:
		{
			MOT3_PID.control=false;
			MOT_Speed(motor3,0);
			TIM_SetCompare1(TIM8,4000);
			TIM_SetCompare2(TIM8,4000);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
		}
		case motor4:
		{
			MOT4_PID.control=false;
			MOT_Speed(motor4,0);
			TIM_SetCompare3(TIM1,4000);
			TIM_SetCompare4(TIM1,4000);
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		}
	}
}

/*************************************
函数名：MOT_Release(...)
参数：motorx 
作用：将某个电机由刹车状态恢复
*************************************/
void MOT_Release(Motor motor)
{
	switch(motor)
	{
		case motor1:
		{
			MOT1_PID.control=true;
			TIM_SetCompare1(TIM1,0);
			TIM_SetCompare2(TIM1,0);
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
		}
		case motor2:
		{
			MOT2_PID.control=true;
			TIM_SetCompare3(TIM1,0);
			TIM_SetCompare4(TIM1,0);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
		}
		case motor3:
		{
			MOT3_PID.control=true;
			TIM_SetCompare1(TIM8,0);
			TIM_SetCompare2(TIM8,0);
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
		}
		case motor4:
		{
			MOT4_PID.control=true;
			TIM_SetCompare3(TIM8,0);
			TIM_SetCompare4(TIM8,0);
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		}
	}
}

/****************************************************
函数名：CAR_SpeedSET(...)
参数：
	CAR_Speed:车辆的线速度
	angle:车辆的前进方向
	angular_speed:车辆的角速度
作用：根据速度设定直接控制电机转速，需用户重写.
*****************************************************/	
void CAR_SpeedSET(float CAR_Speed,float angle,int angular_speed)
{
		//put your codes here
}

/***************************************************/

float map(float value , float premin , float premax , float min , float max)
{
	if(value < 0)
	{
		return (value*(min/premin));
	}
	else if(value >= 0)
	{
		return (value*(max/premax));
	}
}

void wait_Cmd()
{
	while(USART_FLAG!=1);
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_Cmd(USART1, DISABLE); 
}

void end_Cmd()
{
	USART_FLAG=0;
	TIM_Cmd(TIM6,ENABLE);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE); 
}