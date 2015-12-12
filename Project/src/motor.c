/* 
* @Author: jeasinema
* @Date:   2015-12-06 22:21:57
* @Last Modified by:   jeasinema
* @Last Modified time: 2015-12-11 21:48:07
*/
#include "motor.h"

void Motor_Init(void)								   //初始化所有电机
{
	//Init DCBL
	GPIO_InitTypeDef GPIO_InitStructure;  

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//注意要打开SYSCFG时钟

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_DOWN;      //需配置为内部下拉       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    //PWM Output
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;    //PWM Output
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;    //PWM Output
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;    //功能引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);   //TIM1_CH1 DCBL
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);	//TIM2_CH1 Stepper1-x
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);  //TIM3_CH1 Stepper2-y
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);	//TIM4_CH1 Stepper3-Drill

	GPIO_SetBits(GPIOA,GPIO_Pin_0);   
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);

    EXTI_InitTypeDef EXTI_InitStructure;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);    
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);    
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);    
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	  /*************************************/

	/*Drills*/	        
	TIM_TimeBaseInitTypeDef  TIM1_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM1_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   	   
	TIM1_TimeBaseStructure.TIM_Prescaler = 0x0A;
	TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM1_TimeBaseStructure.TIM_Period = 100;
	TIM1_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM1_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseStructure);
	TIM_OCStructInit(&TIM1_OCInitStructure);   
	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
	TIM1_OCInitStructure.TIM_Pulse = 0;  
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM1,&TIM1_OCInitStructure);
	TIM_ARRPreloadConfig(TIM1, DISABLE);      //if the Preloading is ENABLE,all the setting will not be set to the register.
	TIM1->BDTR |= 1<<15;  
	TIM1->CNT   = 0x00;
	TIM_SetCompare1(TIM1,0);    //开机电机关闭
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 

	/*Stepper1*/
	TIM_TimeBaseInitTypeDef  TIM2_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM2_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   	   
	TIM2_TimeBaseStructure.TIM_Prescaler = 0x0A;
	TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM2_TimeBaseStructure.TIM_Period = 100;
	TIM2_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM2_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM2,&TIM2_TimeBaseStructure);
	TIM_OCStructInit(&TIM2_OCInitStructure);   
	TIM2_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	TIM2_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
	TIM2_OCInitStructure.TIM_Pulse = 0;  
	TIM2_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM2,&TIM2_OCInitStructure);
	TIM_ARRPreloadConfig(TIM2, DISABLE);      //if the Preloading is ENABLE,all the setting will not be set to the register.
	TIM2->BDTR |= 1<<15;  
	TIM2->CNT   = 0x00;
	TIM2->ARR=0;   //开机电机关闭
	TIM_Cmd(TIM2,ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE); 

	/*Stepper2*/
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM3_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   	   
	TIM3_TimeBaseStructure.TIM_Prescaler = 0x0A;
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM3_TimeBaseStructure.TIM_Period = 100;
	TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);
	TIM_OCStructInit(&TIM3_OCInitStructure);   
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
	TIM3_OCInitStructure.TIM_Pulse = 0;  
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM3,&TIM3_OCInitStructure);
	TIM_ARRPreloadConfig(TIM3, DISABLE);      //if the Preloading is ENABLE,all the setting will not be set to the register.
	TIM3->BDTR |= 1<<15;  
	TIM3->CNT   = 0x00;
	TIM3->ARR=0;    //开机电机关闭
	TIM_Cmd(TIM3,ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE); 

	/*Stepper3*/
	TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM4_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   	   
	TIM4_TimeBaseStructure.TIM_Prescaler = 0x0A;
	TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM4_TimeBaseStructure.TIM_Period = 100;
	TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);
	TIM_OCStructInit(&TIM4_OCInitStructure);   
	TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
	TIM4_OCInitStructure.TIM_Pulse = 0;  
	TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM4,&TIM4_OCInitStructure);
	TIM_ARRPreloadConfig(TIM4, DISABLE);      //if the Preloading is ENABLE,all the setting will not be set to the register.
	TIM4->BDTR |= 1<<15;  
	TIM4->CNT   = 0x00;
	TIM4->ARR=0;   //开机电机关闭
	TIM_Cmd(TIM4,ENABLE);
	TIM_CtrlPWMOutputs(TIM4, ENABLE); 
}


void Stepper_Speed(STEPPER stepperx , float speed)    //控制单个电机旋转
{
	switch(stepperx)
	{
		case stepper1:
		{
			if(speed==0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
			}
			if(speed<0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				speed=-speed;
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
			}
			TIM2->ARR=(101-speed)*100;    
			break;
		}
		case stepper2:
		{
			if(speed==0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3);
			}
			if(speed<0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				speed=-speed;
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_2);
			}
			TIM3->ARR=(101-speed)*100;    
			break;
		}
		case stepper3:
		{
			if(speed==0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_5);
			}
			if(speed<0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);
				speed=-speed;
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_4);
			}
			TIM4->ARR=(101-speed)*100;    
			break;
		}
		default:break;
	}
}

void DCMotor_Speed(float speed)   //TODO: 3.3->5 
{
	if(speed>=0)
	{
		TIM_SetCompare1(TIM1,speed);
	}
	else
	{
		TIM_SetCompare1(TIM1,-speed);
	}
}


void Delay(u32 nCount)
{
   for(; nCount != 0; nCount--);
}





