/**********************************
文件名：myspeedcontrol.c
作用说明：
        实现了有关电机控制的函数和容器
使用说明：
        1.建议不要修改本文件
        2.可调用函数：无
函数调用说明：
        无
**********************************/

#include "myspeedcontrol.h"
int RealSpeed[4]={0,0,0,0};//the maxium of speed is about 144

#define PI 3.14159

PID MOT1_PID={0,0,PID_KP,0,0,0,0,0,1},MOT2_PID={1,0,PID_KP,0,0,0,0,0,1},MOT3_PID={2,0,PID_KP,0,0,0,0,0,1},MOT4_PID={3,0,PID_KP,0,0,0,0,0,1};
/****************************************************

****************************************************/
 /*Motors' pins should be linked to PA8-PE11 PE13-14, PA15-PB3*/

void GPIO_Motor_Config(void){
		
   	  GPIO_InitTypeDef GPIO_InitStructure;  
	  
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	  GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_DOWN;      //需配置为内部下拉       
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_0|GPIO_Pin_4;  
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_11|GPIO_Pin_9;;  
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;   //初始化电机使能脚
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);


	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);   //TIM1_CH1
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);	//TIM1_CH2
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);  //TIM1_CH3
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);	//TIM1_CH4
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);	//TIM8_CH1   
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);	//TIM8_CH2
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);	//TIM8_CH3
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);	//TIM8_CH4

      GPIO_SetBits(GPIOC,GPIO_Pin_0);    //开机电机使能
      GPIO_SetBits(GPIOA,GPIO_Pin_3);
      GPIO_SetBits(GPIOC,GPIO_Pin_4);
      GPIO_SetBits(GPIOA,GPIO_Pin_5);

}

void TIM1_Init(void){
		
		TIM_TimeBaseInitTypeDef  TIM1_TimeBaseStructure; 
		TIM_OCInitTypeDef  TIM1_OCInitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   	   
		TIM1_TimeBaseStructure.TIM_Prescaler = 0xFF;
		TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM1_TimeBaseStructure.TIM_Period = 4000;
		TIM1_TimeBaseStructure.TIM_ClockDivision = 0x0;
		TIM1_TimeBaseStructure.TIM_RepetitionCounter = 0; 

	    TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseStructure);

	         
	    TIM_OCStructInit(&TIM1_OCInitStructure);   
	    TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	    
  
	/*Motor 1*/
		TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
		TIM1_OCInitStructure.TIM_Pulse = 0;  
		TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
		TIM_OC1Init(TIM1,&TIM1_OCInitStructure);

    /*Motor 1R*/
		TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
		TIM1_OCInitStructure.TIM_Pulse = 0;  
		TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		 
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
		TIM_OC2Init(TIM1,&TIM1_OCInitStructure);

    /*Motor 2*/
        TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM1_OCInitStructure.TIM_Pulse = 0; 
	    TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
        TIM_OC3Init(TIM1, &TIM1_OCInitStructure); 

    /*Motor 2R*/
		TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM1_OCInitStructure.TIM_Pulse = 0;
		TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable); 
		TIM_OC4Init(TIM1, &TIM1_OCInitStructure);
	
		TIM_ARRPreloadConfig(TIM1, DISABLE);      //if the Preloading is ENABLE,all the setting will not be set to the register.
		
		
		TIM1->BDTR |= 1<<15;  
		TIM1->CNT   = 0x00;

		TIM_Cmd(TIM1,ENABLE);
		TIM_CtrlPWMOutputs(TIM1, ENABLE); 
}

void TIM8_Init(){
   
		TIM_TimeBaseInitTypeDef  TIM8_TimeBaseStructure; 
		TIM_OCInitTypeDef  TIM8_OCInitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   	   
		TIM8_TimeBaseStructure.TIM_Prescaler = 0xFF;
		TIM8_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM8_TimeBaseStructure.TIM_Period = 4000;
		TIM8_TimeBaseStructure.TIM_ClockDivision = 0x0;

		TIM_TimeBaseInit(TIM8,&TIM8_TimeBaseStructure);
       
		TIM_OCStructInit(& TIM8_OCInitStructure); 
		TIM8_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		
		
		/*Motor 3*/
		TIM8_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           
		TIM8_OCInitStructure.TIM_Pulse = 0;  
		TIM8_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	 
		TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Disable);
		TIM_OC1Init(TIM8,&TIM8_OCInitStructure);	

		/*Motor 3R*/
		TIM8_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM8_OCInitStructure.TIM_Pulse = 0; 
		TIM8_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	
		TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Disable); 
		TIM_OC2Init(TIM8, &TIM8_OCInitStructure);

		/*Motor 4*/
		TIM8_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           
		TIM8_OCInitStructure.TIM_Pulse = 0;  
		TIM8_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	 
		TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Disable);
		TIM_OC3Init(TIM8,&TIM8_OCInitStructure);	

		/*Motor 4R*/
		TIM8_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM8_OCInitStructure.TIM_Pulse = 0; 
		TIM8_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	
		TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Disable); 
		TIM_OC4Init(TIM8, &TIM8_OCInitStructure);
		
		TIM_ARRPreloadConfig(TIM8, DISABLE);

		TIM8->BDTR |= 1<<15; 
		TIM8->CNT   = 0x00;
		
		TIM_Cmd(TIM8,ENABLE);
		TIM_CtrlPWMOutputs(TIM8, ENABLE);
}     


/*************************************
函数名：Motor_Init(...)
参数：无
作用：初始化电机（IO，定时器初始化）
*************************************/  
void Motor_Init(void)
{
	GPIO_Motor_Config();
	TIM1_Init();
	TIM8_Init();
}      


/*********************************************

**********************************************/	
void MOTAll_PIDReset()
{
	
	TIM_Cmd(TIM6,DISABLE);
	TIM_ITConfig(TIM6,TIM_IT_Update,DISABLE); 
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,0);
	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare4(TIM8,0);
	RealSpeed[0]=0;
	RealSpeed[1]=0;
	RealSpeed[2]=0;
	RealSpeed[3]=0;
	MOT1_PID.PWM=0;
	MOT2_PID.PWM=0;
	MOT3_PID.PWM=0;
	MOT4_PID.PWM=0;
	MOT1_PID.targetValue=0;
	MOT1_PID.integrationError=0;
	MOT1_PID.lastError=0;
	MOT2_PID.targetValue=0;
	MOT2_PID.integrationError=0;
	MOT2_PID.lastError=0;
	MOT3_PID.targetValue=0;
	MOT3_PID.integrationError=0;
	MOT3_PID.lastError=0;
	MOT4_PID.targetValue=0;
	MOT4_PID.integrationError=0;
	MOT4_PID.lastError=0;
	TIM_counter[0][0]=0x7FFF;
	TIM_counter[0][1]=0x7FFF;
	TIM_counter[1][0]=0x7FFF;
	TIM_counter[1][1]=0x7FFF;
	TIM_counter[2][0]=0x7FFF;
	TIM_counter[2][1]=0x7FFF;
	TIM_counter[3][0]=0x7FFF;
	TIM_counter[3][1]=0x7FFF;
	TIM2->CNT =0x7FFF;
	TIM3->CNT =0x7FFF;
	TIM4->CNT =0x7FFF;
	TIM5->CNT =0x7FFF;
	//delay_ms(0xFFFF);	
}

/****************************************************

****************************************************/
void MOT_PWMSET(PID *MOTx,int PWM)
{
		if (MOTx->ID==0)
		{
			if(PWM>=0)
			{
				TIM_SetCompare2(TIM1,PWM);
			    TIM_SetCompare1(TIM1,0);
			}
			if(PWM<0)
			{
				TIM_SetCompare2(TIM1,0);
				TIM_SetCompare1(TIM1,-PWM);
			}
		}
			if (MOTx->ID==1)
			{
				if(PWM>=0)
				{
					TIM_SetCompare4(TIM1,PWM);
					TIM_SetCompare3(TIM1,0);
				}
				if(PWM<0)
				{
					TIM_SetCompare4(TIM1,0);
				    TIM_SetCompare3(TIM1,-PWM);
				}
			}
			if (MOTx->ID==2)
			{
				if(PWM>=0)
				{
					TIM_SetCompare2(TIM8,PWM);
					TIM_SetCompare1(TIM8,0);
				}
				if(PWM<0)
				{
					TIM_SetCompare2(TIM8,0);
					TIM_SetCompare1(TIM8,-PWM);
				}
			}
			if (MOTx->ID==3)
			{
				if(PWM>=0)
				{
					TIM_SetCompare4(TIM8,PWM);
					TIM_SetCompare3(TIM8,0);
				}
				if(PWM<0)
				{
					TIM_SetCompare4(TIM2,0);
					TIM_SetCompare3(TIM2,-PWM);
				}
			}
}

/****************************************************

*****************************************************/
void MOT_SpeedPID(PID *MOTx)
{
	  calcPID(MOTx,RealSpeed[MOTx->ID]);
	  MOT_PWMSET(MOTx,MOTx->PWM);
}


/****************************************************

*****************************************************/	
void MOT_SpeedSET(PID *MOTx,int Speed)
{
		MOTx->targetValue =Speed;
}