#include "button_rocker.h"
#include "cutterMove.h"
#include "motor.h"
#include "sizeInput.h"


int main()
{
	CMD *cmd=(*CMD)malloc(sizeof(CMD));  
	LOCATION *selflocation=(*LOCATION)malloc(sizeof(LOACTION)); 
	SIZE *size=(*SIZE)malloc(sizeof(SIZE));
	Motor_Init();
	Rocker_Init();
	Input_Init();
	Cutter_Init();   //回零点
	while (1)
	{   
  		//TODO：计算route
  		if(Cutter_Control_Flag==0)   //触发了导轨越界的外部中断
  		{
  			cmd->mode=0;
  		}
		//运动分自动和手动两种模式，自动模式下刀头只会走直线，且不需要连续提供指令。手动模式下则需要连续提供速度方向和大小的指令。
		Auto_Cmd(cmd,size);     //根据包装外形编程计算控制指令，仅在Cutter_Control_Flag为1的时候方使能
		//Auto_Cmd中判断当前位置与x/y_target之间的距离，从而判断是否需要切换至route中的下一个点
		Rocker_Cmd(cmd);   //摇杆及按键输入更新控制指令。手动模式拥有最高的优先级。手动模式使能时必须抬刀，关闭直流电机，清除路径。
		//注：手动模式使能的时机有两种：1.摇杆发现有输入 2.触发了导轨边缘的外部中断线
		Exec_Cmd(cmd , selflocation);	   //执行指令  TODO：auto结束后回零点
		Update_Location(selflocation);  //更新位置，计算从上一次到现在的距离
	}
}



