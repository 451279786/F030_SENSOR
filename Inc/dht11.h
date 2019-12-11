#ifndef __DHT11_H
#define __DHT11_H 

#include "stm32f0xx_hal.h"
 
#define DHT11_Dout_PORT        GPIOA
#define DHT11_Dout_PIN         GPIO_PIN_0

#define DHT11_Dout_LOW()   HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_RESET)
#define DHT11_Dout_HIGH()  HAL_GPIO_WritePin(DHT11_Dout_PORT,DHT11_Dout_PIN,GPIO_PIN_SET)
#define DHT11_Data_IN()    HAL_GPIO_ReadPin(DHT11_Dout_PORT,DHT11_Dout_PIN)



uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11  

void delay_us(uint32_t nus);
	
void DHT11_Mode_IPU(void);
void DHT11_Mode_Out_PP(void);
void Int_To_Str(int x,char *Str);
#endif















