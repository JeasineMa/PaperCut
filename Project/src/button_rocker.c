/* 
* @Author: jeasinema
* @Date:   2015-12-06 22:25:48
* @Last Modified by:   jeasinema
* @Last Modified time: 2015-12-11 23:57:02
*/

#include "button_rocker.h"
#include "math.h"

/*
PC0---ADC1_IN10-DMA2S0C0
PC1---ADC1_IN11-DMA2S0C0
PC2---ADC1_IN12-DMA2S0C0
PC3---ADC1_IN13-DMA2S0C0
PC4---Button1
PC5---Button2
*/

void Rocker_Init(void)  //摇杆初始化
{

      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
      GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	  ADC_InitTypeDef ADC_InitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
	  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  	  /* DMA2 Stream0 channel0 configuration **************************************/
  	  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	  DMA_InitStructure.DMA_PeripheralBaseAddr = &ADC1->DR;
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_value;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	  DMA_InitStructure.DMA_BufferSize = 128;    //4*32
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;   
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //DR is a half-word register
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	  DMA_Cmd(DMA2_Stream0, ENABLE);

	  /* ADC Common Init **********************************************************/
	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	  ADC_CommonInit(&ADC_CommonInitStructure);
	  /* ADC1/2 Init ****************************************************************/
	  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfConversion = 1;
	  ADC_Init(ADC1, &ADC_InitStructure);
	  /* ADC3 regular channel12 configuration *************************************/
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_112Cycles);
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_112Cycles);
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_112Cycles);
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_112Cycles);
	 /* Enable DMA request after last transfer (Single-ADC mode) */
	  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	  /* Enable ADC3 DMA */
	  ADC_DMACmd(ADC1, ENABLE);
	  /* Enable ADC3 */
	  ADC_Cmd(ADC1, ENABLE);
		/* Start ADC3 Software Conversion */ 
	  ADC_SoftwareStartConv(ADC1);   //如果不是外部触发则必须软件开始转换

	  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn; 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	  NVIC_Init(&NVIC_InitStructure); 

	  //校准中间值
	  Delay(0xFFFF);
	  for (uint8_t i=0;i<4:++i)
	  {
		  ADC_middle+=ADC_value_filter[i]
	  }
	  ADC_middle>>=2;    //取中间值
}

void DMA2_Stream0_IRQHandler(void)    //DMA中断滤波
{
 	 if(DMA_GetITStatus(DMA2_IT_TC1) != RESET) 
   	 {  
		ADC_SoftwareStartConvCmd(ADC1, DISABLE);			
		uint32_t sum= 0; int count,i; 
		for(i=0;i<4;i++) 
		{
			for (count=0;count<32;count++) 
			{
				sum += ADC_value[count][i]; 
			}
		ADC_value_filter[i]=sum>>5; 
		sum=0;
		}	
	 }
	 DMA_ClearITPendingBit(DMA2_IT_TC1);
}

void ABS(int x)
{
	if (x>0)
		return x;
	else 
		return -x;
}


void Rocker_Cmd(CMD &cmd); //读取指令存入指令空间  TODO:滤波
{
	if(filter_flag==1)
	{
		filter_flag=0;   //已完成一次采样
		x1=ABS(ADC_value_filter[0]-ADC_middle);  //max=2048
		y1=ABS(ADC_value_filter[1]-ADC_middle);  
		x2=ABS(ADC_value_filter[2]-ADC_middle);
		y2=ABS(ADC_value_filter[3]-ADC_middle);
		//滤除小扰动
		if(x1>50&&y1[1]>50) 
		{
			cmd->mode=0;   //切换为手动模式
			cmd->insert=0;  //强制抬刀
			cmd->DCSpeed=0;  //强制停转电机
			cmd->speed=sqrt(x1*x1+y1*y1)/29;    
			if(x1>0)
				cmd->angle=atan((float)y1/x1);
			else if(x1<0)
				cmd->angle=atan((float)y1/x1)+PI;
			else if(y1>0)
				cmd->angle=PI/2;
			else if(y1<0)
				cmd->angle=3*PI/2;
			else
				cmd->angle=0;
		}

	}
	//TODO:sizeinput的Rocker部分
	if(GPIO_ReadInputDataBit(GPIOC , GPIO_Pin_4))
	{
		Stepper_Speed(stepper3 , 30);
		Delay(0xFF);  //需要调节点动操作时电机运动的时间
		Stepper_Speed(stepper3 , 0);
	}
	if(GPIO_ReadInputDataBit(GPIOC , GPIO_Pin_5))
	{
		Stepper_Speed(stepper3 , -30);
		Delay(0xFF);
		Stepper_Speed(stepper3 , 0);
	}




}
void Exec_Cmd(CMD &cmd);    //执行命令，底层调用cuttermove的函数
{

}