#ifndef _MOTOR_H
#define _MOTOR_H


#include "stm32f4xx.h"


 /*
	  PA0---Stepper1.Enable
	  PA1---Stepper1.Direction
	  PA2---Stepper2.Enable
	  PA3---Stepper2.Direction
	  PA4---Stepper3.Enable
	  PA5---Stepper3.Direction
	  PE9---DCBL   _Pulse
	  PA15--Stepper1.Pulse
	  PB4--Stepper2.Pulse
	  PB6--Stepper3.Pulse
	  PD1---X-EXTI-start
	  PD2---X-EXTI-stop
	  PD3---Y_EXTI-start
	  PD4---Y_EXTI-stop
	  */
	  
typedef	enum stepper
{
	stepper1=0,    //X方向(导轨较短的方向)
	stepper2=1,    //Y方向(导轨较长的方向)
	stepper3=2,	   //刀头抬起
}STEPPER;

void Motor_Init(void);								   //初始化所有电机
void Stepper_Speed(STEPPER stepperx , float speed);    //控制单个电机旋转  speed~[0,100]
void DCMotor_Speed(float speed);					   //控制刀头旋转      speed~[0,100]
void Delay(u32 nCount);

uint8_t Cutter_Init_Flag=0;  //判断是否回零点
uint8_t Cutter_Control_Flag=0;   //0---Manuallly 1---Automatically
#endif 