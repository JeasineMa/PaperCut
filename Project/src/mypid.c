/**********************************
文件名：mypid.c
作用说明：
        实现了有关电机pid控制的函数
使用说明：
        1.建议不要修改本文件
        2.可调用函数：无
函数调用说明：
        无
**********************************/
#include "mypid.h"
#define MAX_INTEGRATION_ERROR                2500
#define MAX_PID_OUTPUT                       4000
/****************************************************

****************************************************/
void calcPID(PID *pid, int input)
{
int output,error;
error=pid->targetValue-input;
if (pid->Ki != 0)
        {
                pid->integrationError += error;
                // Limit the maximum integration error
                if (pid->integrationError > MAX_INTEGRATION_ERROR)
                {
                        pid->integrationError = MAX_INTEGRATION_ERROR;
                }
                else if (pid->integrationError < -MAX_INTEGRATION_ERROR)
                {
                        pid->integrationError = -MAX_INTEGRATION_ERROR;
                }
        }

        output = pid->Kp * error + pid->Ki * pid->integrationError + pid->Kd * (error - pid->lastError);

        // Limit the maximum output
        if (output > MAX_PID_OUTPUT)
        {
                output = MAX_PID_OUTPUT;
        }
        else if (output < -MAX_PID_OUTPUT)
        {
                output = -MAX_PID_OUTPUT;
        }

        pid->lastError = error;
	    pid->PWM+=output;
         if ( pid->PWM> MAX_PID_OUTPUT)
        {
                pid->PWM = MAX_PID_OUTPUT;
        }
        else if (pid->PWM < -MAX_PID_OUTPUT)
        {
                pid->PWM = -MAX_PID_OUTPUT;
        }

}
