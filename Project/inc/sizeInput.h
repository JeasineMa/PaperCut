#ifndef _SIZEINPUT_H
#define _SIZEINPUT_H

#include "motor.h"
#include "cutterMove.h"
#include "button_rocker.h"

typedef struct size
{
	float x;
	float y;
	float z;
	NODE *route;
}SIZE;

NODE *route;


void Input_Init(void);
void Input_Display(SIZE &size);   //TODO:显示当前路径的步骤数
void Calculate_Route(SIZE &size);
void Auto_Cmd(CMD &cmd, SIZE &size);

#endif