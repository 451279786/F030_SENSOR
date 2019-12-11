#include "PWMCapture.h"

//#define CO2   1
#define PM2_5   1


uint32_t capture_Buf[3] = {0};   //PWM输入捕获 存放计数值
uint8_t capture_Cnt = 0;         //状态标志位
uint32_t high_time;              //高电平时间


//具体流程：
//1.设置TIM3 CH1为输入捕获功能

//2.设置上升沿捕获

//3.使能TIM2?CH1捕获功能

//4.捕获到上升沿后，定时器当前计数值存入capture_buf[0]，改为捕获下降沿

//5.捕获到下降沿后，定时器当前计数值存入存入capture_buf[1]，关闭TIM3 CH1捕获功能；??capture_Cnt=3；

//6.高电平时间： capture_buf[1] - capture_buf[0] 发送到上位机,重新启动输入捕获
//
//capture_buf[0]上升沿
//capture_buf[1]下降沿
//
//
//
uint32_t PWMCapture(void)
{
	switch (capture_Cnt)
	{
		case 0:
			
			capture_Cnt++;
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//设置为上升沿捕获
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);	//启动输入捕获       或者: __HAL_TIM_ENABLE(&htim5)，函数用于使能定时器某一通道的输入捕获功能，并使能相应的中断
		
			
			break;
		case 3:
			if(capture_Buf[1] > capture_Buf[0])
				high_time = capture_Buf[1]- capture_Buf[0];    //高电平时间
			else
				high_time = 0xFFFF + capture_Buf[1] - capture_Buf[0];    //高电平时间

#if CO2
			high_time = ((high_time / 10) - 2)*10;
			printf("high_time = %dPPM",high_time);
#elif PM2_5
			
			high_time = ((1000 - (high_time / 10))/10);
			printf("PM2.5 = %d%%",high_time);
			
#endif		
			capture_Cnt = 0;  //清空标志位
			return high_time;
				
	}
	return high_time;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
	if(TIM3 == htim->Instance)
	{
		switch(capture_Cnt){
			case 1:
				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
				//TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1);   //一定要先清除原来的捕获极性！！
				//TIM_SET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器5通道1设置为下降沿捕获（重设捕获极性）
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //1.设置为下降沿捕获,(不是函数，而是底层操作的一个宏定义)                                                                    
				capture_Cnt++;                                                               //2.里面有两个函数，第一个为清除清除原来的捕获极性，第二个为设置通道捕捉极性
				break;                                                                       //3.在修改定时器某一通道的输入捕获极性时，一定要先清除该通道之前捕获极性
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
				HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5) ，和开启功能相反，是关闭定时器某一通道的输入捕获功能和相应中断
				capture_Cnt++;   

		}
	}
}







