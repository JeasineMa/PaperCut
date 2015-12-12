/**********************************
文件名：usart1.h
作用说明：
	定义了有关串口蓝牙接收的函数
使用说明：
	1.建议不要修改本文件
	2.可调用函数：
		无
函数调用说明：
	无
**********************************/
#ifndef __USART1_H
#define	__USART1_H

#include "All_Include.h"
#include <stdio.h>


#define USART1_DR_Base  0x40013804
#define SENDBUFF_SIZE 5000
	
extern char LastCommand,CommandUpdate;
void Bluetooth_Init(void);
int fputc(int ch, FILE *f);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void NVIC_Config(void);

#endif
