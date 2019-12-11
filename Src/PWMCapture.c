#include "PWMCapture.h"

//#define CO2   1
#define PM2_5   1


uint32_t capture_Buf[3] = {0};   //PWM���벶�� ��ż���ֵ
uint8_t capture_Cnt = 0;         //״̬��־λ
uint32_t high_time;              //�ߵ�ƽʱ��


//�������̣�
//1.����TIM3 CH1Ϊ���벶����

//2.���������ز���

//3.ʹ��TIM2?CH1������

//4.���������غ󣬶�ʱ����ǰ����ֵ����capture_buf[0]����Ϊ�����½���

//5.�����½��غ󣬶�ʱ����ǰ����ֵ�������capture_buf[1]���ر�TIM3 CH1�����ܣ�??capture_Cnt=3��

//6.�ߵ�ƽʱ�䣺 capture_buf[1] - capture_buf[0] ���͵���λ��,�����������벶��
//
//capture_buf[0]������
//capture_buf[1]�½���
//
//
//
uint32_t PWMCapture(void)
{
	switch (capture_Cnt)
	{
		case 0:
			
			capture_Cnt++;
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//����Ϊ�����ز���
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);	//�������벶��       ����: __HAL_TIM_ENABLE(&htim5)����������ʹ�ܶ�ʱ��ĳһͨ�������벶���ܣ���ʹ����Ӧ���ж�
		
			
			break;
		case 3:
			if(capture_Buf[1] > capture_Buf[0])
				high_time = capture_Buf[1]- capture_Buf[0];    //�ߵ�ƽʱ��
			else
				high_time = 0xFFFF + capture_Buf[1] - capture_Buf[0];    //�ߵ�ƽʱ��

#if CO2
			high_time = ((high_time / 10) - 2)*10;
			printf("high_time = %dPPM",high_time);
#elif PM2_5
			
			high_time = ((1000 - (high_time / 10))/10);
			printf("PM2.5 = %d%%",high_time);
			
#endif		
			capture_Cnt = 0;  //��ձ�־λ
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
				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
				//TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ���Ĳ����ԣ���
				//TIM_SET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��5ͨ��1����Ϊ�½��ز������貶���ԣ�
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //1.����Ϊ�½��ز���,(���Ǻ��������ǵײ������һ���궨��)                                                                    
				capture_Cnt++;                                                               //2.������������������һ��Ϊ������ԭ���Ĳ����ԣ��ڶ���Ϊ����ͨ����׽����
				break;                                                                       //3.���޸Ķ�ʱ��ĳһͨ�������벶����ʱ��һ��Ҫ�������ͨ��֮ǰ������
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
				HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1); //ֹͣ����   ����: __HAL_TIM_DISABLE(&htim5) ���Ϳ��������෴���ǹرն�ʱ��ĳһͨ�������벶���ܺ���Ӧ�ж�
				capture_Cnt++;   

		}
	}
}







