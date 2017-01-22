
#include "stm32f4xx.h"
#include "delay.h"
#include "oled.h"
#include "spi.h"
#include "iic.h"
#include "mpu6050.h"

static u8 shift_flag = 0;  //�����л�������ʾ�ı�־λ
int acc()//�������ṹ  
{
	u32 n;
	u8 uiBuf[40];
	
	float accX,accY,accZ;                 //�ԡ���/��^2��Ϊ��λ�ļ��ٶ�
	short aacx,aacy,aacz;									//���ٶȴ�����ԭʼ����
	delay_init(100);
	IIC_init();
	MPU_Init();
	Spi1_init();
	Oled_init();
  
	while (1)
	{		
    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //�õ����ٶȴ���������
		//����������ת��Ϊ�ԡ���/��^2��Ϊ��λ�ļ��ٶ�
		accX = ((float)(int)aacx/16384) *10;
		accY = ((float)(int)aacy/16384) *10;
		accZ = ((float)(int)aacz/16384) *10; 
		
		
		if(accX>=0)
		{ 
			sprintf((char*)uiBuf,"X:%02d ",(char)accX); // ��������ʾ
			OLED_showString(64,1,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"X:-%02d",(char)(-accX)); // ��������ʾ
			OLED_showString(64,1,uiBuf,16);				
		}

		
		if(accY>=0)
		{
			sprintf((char*)uiBuf,"Y:%02d ",(char)accY); // ��������ʾ
			OLED_showString(64,3,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"Y:-%02d",(char)(-accY)); // ��������ʾ
			OLED_showString(64,3,uiBuf,16);				
		}
		
		
		if(accZ>=0)
		{
			sprintf((char*)uiBuf,"Z:%02d ",(char)accZ); // ��������ʾ
			OLED_showString(64,5,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"Z:-%02d",(char)(-accZ)); // ��������ʾ
			OLED_showString(64,5,uiBuf,16);				
		}		
	
		
	}
			
}



