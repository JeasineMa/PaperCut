/**********************************
文件名：TIM6_interrupts.c
作用说明：
        实现了有关系统中断控制的函数
使用说明：
        1.建议不要修改本文件
        2.可调用函数：无
函数调用说明：
        无
**********************************/
#include "TIM6_interrupt.h"


u16 TIM_counter[4][2]={{0,0},{0,0},{0,0},{0,0}};//TIM_counter[][0]=LASTcounter,TIM_counter[][1]=CURRENTcounter,

/*************************************
函数名：SysInterrupts_Init(...)
参数：无
作用：控制执行中断初始化（定时器中断初始化）
*************************************/
void SysInterrupts_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x2A; //  APB1=42Mhz
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //5000'
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); 
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);

	TIM_Cmd(TIM6,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**************************************************

***************************************************/

void calcSpeed()
{
	
	int temp[4]={0,0,0,0};     
	temp[0]=TIM_GetCounter(TIM2);
	temp[1]=TIM_GetCounter(TIM3);
	temp[2]=TIM_GetCounter(TIM4);
	temp[3]=TIM_GetCounter(TIM5);
	for(int i=0;i<4;i++)
	{
       TIM_counter[i][1]=temp[i];
	   if((abs(temp[i]-TIM_counter[i][0])<50000)){
	   		RealSpeed[i]=TIM_counter[i][1]-TIM_counter[i][0];	
	   }
	   TIM_counter[i][0]=TIM_counter[i][1];
    }

}


/****************************************************

*****************************************************/
void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)!= RESET)
	{
		//printf("asasdd\r\n");
		calcSpeed();
		if(MOT1_PID.control)
			MOT_SpeedPID(MOT1);
		if(MOT2_PID.control)
			MOT_SpeedPID(MOT2);
		if(MOT3_PID.control)
			MOT_SpeedPID(MOT3);
		if(MOT4_PID.control)
			MOT_SpeedPID(MOT4);
	}
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
}