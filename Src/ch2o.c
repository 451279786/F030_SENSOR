#include "ch2o.h"
#include "usart.h"
#include <string.h>

//STM32F030使用float、printf、sprintf会导致DHT11无法采集到数据

uint16_t Ch2oProtocolAnalysis(uint8_t *buff,uint8_t len)
{
	uint16_t fch2o;
	if(buff[0] == 0xFF)         /*如果是网络数据*/
	{
		fch2o = ((buff[4] * 256) + buff[5])*125/100;
//		printf ("buff[4] = 0x%x\n",buff[4]);
//		printf ("buff[5] = 0x%x\n",buff[5]);
	}
	return fch2o;
}



char *F2S(float d,char *str)
{
	char str1[40];
	int j = 0,k,i;
	i = (int)d;//浮点数的整数部分
	while(i > 0)
	{
		str1[j++] = i % 10 + '0';
		i = i / 10;
	}
	for(k = 0;k < j;k++)
	{
		str[k] = str1[j-1-k];//被提取的整数部分正序存放到另一个数组
	}
 
 
	str[j++] = '.';
 
	d = d - (int)d;//小数部分提取
	for(i = 0;i < 10;i++)
	{
		d = d*10;
		str[j++] = (int)d + '0';
		d = d - (int)d;
	}
	while(str[--j] == '0');
	str[++j] = '\0';
	return str;
}

uint16_t ch2o = 0;
uint16_t Ch2oSensor(void)//1秒发一次数据,如果不调用此函数，一直不清空buff，会导致死机
{
	if(Usart1type.Usart1RecFlag == 1)
	{
		ch2o = Ch2oProtocolAnalysis(Usart1type.Usart1RecBuff,Usart1type.Usart1RecLen);
		memset(Usart1type.Usart1RecBuff,0,Usart1type.Usart1RecLen);
		Usart1type.Usart1RecFlag = 0; 
		Usart1type.Usart1RecLen = 0;
	}
	return ch2o;
}


