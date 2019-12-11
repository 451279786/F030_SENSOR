#include "dht11.h"


//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_Mode_Out_PP();
    DHT11_Dout_LOW();         //SET OUTPUT  ����
    HAL_Delay(18);    	//��������18ms
    DHT11_Dout_HIGH();        //DQ=1        ����
	delay_us(30);     	//��������20~40us
}
//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_Mode_IPU();//SET INPUT	 
    while (DHT11_Data_IN()&&retry<100)//�ȴ����ͣ�DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};
	
	if(retry>=100)
	{
		return 1;
	}
	else
	{
		retry=0;
	}
		
	
    while (!DHT11_Data_IN()&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
	{
		return 1;
	}    
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{

 	uint8_t retry=0;
	while(DHT11_Data_IN()&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_Data_IN()&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_Data_IN())return 1;
	else return 0;	
	   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else
	{
		return 1;
	}
	return 0;	    
}
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
uint8_t DHT11_Init(void)
{	 
	//SetPin_OutPut();
	DHT11_Rst();  //��λDHT11
	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
} 

void delay_us(uint32_t time)
{
	uint8_t i;

	while(time)
	{   
		for (i = 0; i < 10; i++)
		{
		  
		}
	time--;
	}
}

/* void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
} */


//������������
static void DHT11_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* �������蹦��GPIO���� */
  GPIO_InitStruct.Pin   = DHT11_Dout_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_Dout_PORT, &GPIO_InitStruct);
        
}

/**
  * ��������: ʹDHT11-DATA���ű�Ϊ�������ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void DHT11_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* �������蹦��GPIO���� */
  GPIO_InitStruct.Pin = DHT11_Dout_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT11_Dout_PORT, &GPIO_InitStruct);         
}



void Int_To_Str(int x,char *Str)
{
	int t;
	char *Ptr,Buf[5];//����һ���ַ���������ַ���ָ�룬
	int i = 0;  //���ڼ���
	Ptr = Str;  //�ڲ�ָ��ָ���ⲿָ�룬���в������ݣ�������Դ�������ݣ�ͨ����ַ����
	if(x < 10)		                 // ������С��10��ת��Ϊ0x��ʽ
	{
		*Ptr ++ = '0'; //��������ǰ�油��0��
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// ͨ����������ֱ��ASCII����ʽ
		}
		i -- ;
		for(;i >= 0;i --) 		// ���õ����ַ�������
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}
