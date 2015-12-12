#ifndef _CUTTERMOVE_H
#define _CUTTERMOVE_H

#include "motor.h"

/*location有两重含义：
1.储存当前刀头所在的位置及当前的运动速度
2.作为route的一个节点，此时x,y为目标坐标，speed为移动的速度。
*/
typedef struct location
{
	float x;
	float y;
	float speed;
}LOCATION;

typedef LOCATION NODE;   //也指route的节点

typedef struct move
{
	float angle;
	float speed;
}MOVE;

void Cutter_Move_Manually(float speed , float angle , float DCSpeed);   //传入速度角度和大小控制运动
void Cutter_Move_Drill(unsigned char flag);       		//1----down 2-----up
void Cutter_Init();						//回原点

void Update_Location(LOCATION &loc);

#endif