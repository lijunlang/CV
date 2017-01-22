#include "stm32f4xx.h"
#include "key.h"
#include "led.h"
#include "exti.h"
#include "uart.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
#include "spi.h"
#include "bmp.h"
#include "iic.h"
#include "sht20.h"
#include "hp6.h"
//#include "acc.h"
#include "mpu6050.h"
#include "rtc.h"
#include "ui.h"
#include "motor.h"

u32 n;
u8 page = 1;
u8 uiBuf[40];
u8 BpBuf[24] = {0};
u8 pulseBuf[24] = {0};
u8 mpuBuf[64];
TempHumiValue_t humi_temp_val ; 	//��ʪ��	

static u8 shift_flag = 0;  				//�����л�������ʾ�ı�־λ
float accX,accY,accZ;                 //�ԡ���/��^2��Ϊ��λ�ļ��ٶ�
short aacx,aacy,aacz;									//���ٶȴ�����ԭʼ����
void clock_page(void);
void TH_page(void);
void pulse_page(void);
void bp_page(void);
void step_page(void);
void kal_km_page(void);
void HP6_EN(void);


dateAndTime_t* Ui_getRTC(void)	 
{ 
  return RTC_getDateAndTime();
}

int main()//�������ṹ  
{
	
	//dateAndTime_t *dateAndTime; 		//ʱ��&����
	delay_init(100);
	led_init();
	key_init();
	IIC_init();
	SHT20_Init();
	Spi1_init();
	Oled_init();
  //�����HP6��EN�ܽ�ʹ�ܡ�����IIC���߲���������Ϊ������������⡣
  HP6_EN();
	
	RTC_init();
	
	init_exti0();
  init_uart1();
	TIM3_init(10000-1,5000-1);//10Khz�ļ���Ƶ�ʣ���10��Ϊ1ms,����10000��Ϊ1s 
	//TIM2_initPWM(500-1,50-1);	//1Mhz�ļ���Ƶ��,2Khz��PWM.  
	Spi1_init();
	Oled_init();
	IIC_init();
	SHT20_Init();
	Hp_6_init();       //��ʼ��HP6����Ѫѹ ģ��
	
	MPU_Init();

	Motor_init(); 
	
	MPU_Set_Accel_Fsr(0);
	
	Motor_ON(MOROT_INTENSITY_3,2);  //�����������2��

	OLED_drawBMP(0,0,128,8,(u8*)BMP_OPEN);
	delay_ms(2000);
	
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //�õ����ٶȴ���������
	
	
	
	while (1)
	{		
		switch(page)
		{
			case 1:
			  OLED_clear();
				clock_page();
				break;
			case 2:
			  OLED_clear();
				TH_page();
				break;
			case 3:
				OLED_clear();
				pulse_page();

				break;
			case 4:
				OLED_clear();
				bp_page();
				step_page();
				break;
			case 5:
				OLED_clear();
				step_page();
				kal_km_page();
				break;		
			case 6:
				OLED_clear();
				kal_km_page();
				break;	
      default:
        break;				
		}

	}
}

void clock_page(void)
{
  while(1)
	{
	  
		/////////////////ʱ��&����	////////////////////
			dateAndTime_t *dateAndTime; 		//ʱ��&����	
	
			

			dateAndTime = Ui_getRTC();
			dateAndTime = Ui_getRTC();
			sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute); 	  
			OLED_showString(24+3,5,uiBuf,24);
			
			sprintf((char*)uiBuf,"20%02d.%02d.%02d",dateAndTime->year,dateAndTime->month,dateAndTime->date); 
			OLED_showString(24,3,uiBuf,20);

			OLED_showCHinese(18,0,12);//
			OLED_showCHinese(36,0,13);//
			OLED_showString(50,0," & TIME",16);
		/////////////////////////////////////


		if(key_scan())
		{
			page = page +1;  //�л���page2
			Motor_ON(MOROT_INTENSITY_3,1);  //�����1��
			IIC_stop();
			break;  //������ǰҳ��
			
    }			
	}
}


void TH_page(void)
{

	TempHumiValue_t humi_temp_val ; 	//��ʪ��	
	u8 uiBuf[40];
  while(1)
	{
			OLED_drawBMP(0,0,64,7,(u8*)TUP1);
			OLED_showCHinese(72,0,9);//��
			OLED_showCHinese(90,0,10);//ʪ
			OLED_showCHinese(108,0,11);//��
			humi_temp_val = *SHT20_readTemAndHum();//��ȡ�¶�
			sprintf((char*)uiBuf,"%02dc %02d",(char)humi_temp_val.temperature,(char)humi_temp_val.humidity); // ��������ʾ
			OLED_showString(64+8,6,uiBuf,16); 	
			OLED_showString(64+8*7,6,"%",16);
		
		if(key_scan())
		{
			page = page +1; //�л���page3
			Motor_ON(MOROT_INTENSITY_3,1);  //�����1��
			IIC_stop();
			
			break;  //������ǰҳ��
    }	
	}
}


void pulse_page(void)
{
	HP_6_OpenRate();   //�����ʲ���
	

  while(1)
	{
  if(0 == shift_flag)
  {
		 shift_flag = 1;
     OLED_drawBMP(0,0,54,7,(u8*)Pulse_bmp_smll);	  //��ʾ����Сͼ
		 delay_ms(300);
  }
  else
  {
		shift_flag = 0;
    OLED_drawBMP(0,0,54,7,(u8*)Pulse_bmp_large);	  //��ʾ���ʴ�ͼ
		delay_ms(300);
  }
  	
		//OLED_drawBMP(0,0,54,7,(u8*)Pulse_bmp);
		OLED_showCHinese(90,0,2);//��
		OLED_showCHinese(108,0,3);//��
		//delay_ms(500);
		
		HP_6_GetRateResult();
		HP_6_GetResultData(pulseBuf);
		
		

		sprintf((char*)uiBuf,"%03d",pulseBuf[7]); // //byte 7�����ʽ��
		
		OLED_showString(64,4,uiBuf,16); 	
		OLED_showString(64+8*3,4,"bpm",16);
		//delay_ms(1000);
	

		if(key_scan())
		{
			page = page +1; //�л���page4
			Motor_ON(MOROT_INTENSITY_3,1);  //�����1��
			IIC_stop();
			HP_6_CloseRate();
			break;  //������ǰҳ��
    }	
	}
}
void bp_page(void)
{
	HP_6_CloseRate();
	delay_ms(100);
	HP_6_OpenBp();   //��Ѫѹ����
	
  while(1)
	{
			OLED_drawBMP(0,0,54,7,(u8*)BP_bmp); //D???
			OLED_showCHinese(90,0,4);//Ѫ
			OLED_showCHinese(108,0,5);///ѹ	
			
			delay_ms(500);
			HP_6_GetBpResult();                //��ȡѪѹ����״̬
			HP_6_GetResultData(BpBuf);
			if(0 == BpBuf[7])                   //byte 7��Ѫѹ������״̬:0�����У�1������ɡ�2����ʧ��
			OLED_showString(64,4,"testing",16);  
			else if(1 == BpBuf[7])  
				 {
					OLED_showString(64,4,"success",16);

					//Motor_ON(MOROT_INTENSITY_3,3);

					//MOROT_INTENSITY_0;
					sprintf((char*)uiBuf,"%03d/%03d ", BpBuf[10], BpBuf[11]);//byte 10��Ѫѹ��������ĸ�ѹ,byte 11��Ѫѹ��������ĵ�ѹ	
					HP_6_CloseBp();//2
					OLED_showString(64,6,uiBuf,16); 		 
				 }		 
			else if(2 == BpBuf[7])                  
				OLED_showString(64,4,"fail   ",16);
			//else 
			//do nothing
		//delay_ms(1000);
		
		
		if(key_scan())
		{
			page = page +1; //�л���page5
			Motor_ON(MOROT_INTENSITY_3,1);  //�����1��
			IIC_stop();
			HP_6_CloseBp();
			MPU_Init();
			OLED_clear();
			break;  //������ǰҳ��
    }	
	}
}


void step_page(void)
{
	HP_6_CloseBp();//2
  while(1)
	{



		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //�õ����ٶȴ���������
		//����������ת��Ϊ�ԡ���/��^2��Ϊ��λ�ļ��ٶ�
		accX = ((float)(int)aacx/16384) *10;
		accY = ((float)(int)aacy/16384) *10;
		accZ = ((float)(int)aacz/16384) *10; 
		if(accX>=0)
		{ 
			sprintf((char*)mpuBuf,"X:%02d ",(char)accX); // ��������ʾ
			OLED_showString(72,1,mpuBuf,16);				
		}
	  else
		{
			sprintf((char*)mpuBuf,"X:-%02d",(char)(-accX)); // ��������ʾ
			OLED_showString(72,1,mpuBuf,16);				
		}

		
		if(accY>=0)
		{
			sprintf((char*)mpuBuf,"Y:%02d ",(char)accY); // ��������ʾ
			OLED_showString(72,3,mpuBuf,16);				
		}
	  else
		{
			sprintf((char*)mpuBuf,"Y:-%02d",(char)(-accY)); // ��������ʾ
			OLED_showString(72,3,mpuBuf,16);				
		}
		
		
		if(accZ>=0)
		{
			sprintf((char*)mpuBuf,"Z:%02d ",(char)accZ); // ��������ʾ
			OLED_showString(72,5,mpuBuf,16);				
		}
	  else
		{
			sprintf((char*)mpuBuf,"Z:-%02d",(char)(-accZ)); // ��������ʾ
			OLED_showString(72,5,mpuBuf,16);				
		}		
	
	if(0 == shift_flag)
  {
		 shift_flag = 1;
     OLED_drawBMP(0,0,54,7,(u8*)step_right_first);	  
		 delay_ms(300);
  }
  else
  {
		shift_flag = 0;
    OLED_drawBMP(0,0,54,7,(u8*)step_left_first);	  
		delay_ms(300);
  }
	////////////////////////////////
		if(key_scan())
		{
			page = page+1;  //�л���page1
			Motor_ON(MOROT_INTENSITY_3,1);  //�����1��
			
			IIC_stop();
			OLED_clear();
			break;  //������ǰҳ��
    }	
	}
}
void kal_km_page(void)
{
	
  while(1)
	{
				
	OLED_clear();

	OLED_showString(64,1,"Pretend",16);
	OLED_showString(64,3," to be ",16);
	OLED_showString(64,5,"steping",16);

	if(0 == shift_flag)
  {
		 shift_flag = 1;
     OLED_drawBMP(0,0,54,7,(u8*)step_right_first);	  
		 delay_ms(300);
  }
  else
  {
		shift_flag = 0;
    OLED_drawBMP(0,0,54,7,(u8*)step_left_first);	  
		delay_ms(300);
  }
	////////////////////////////////

		if(key_scan())
		{
			page = 1; //�л���page5
			Motor_ON(MOROT_INTENSITY_3,1);  //�����1��
			IIC_stop();
			break;  //������ǰҳ��
    }	
	}
}
void HP6_EN(void)
{
	RCC->AHB1ENR|=1<<1;//ʹ��PORTBʱ�� 
	GPIOB->MODER &= ~(3 << 30); 	//32bit,����
	GPIOB->MODER |= ((unsigned int )1 << 30);    	//����PB15 IO�ڣ�Ϊͨ���������
	
	GPIOB->OTYPER &= ~(1 << 15);	  //����PB15Ϊ�������
	
	GPIOB->OSPEEDR &= ~(3 << 30);
	GPIOB->OSPEEDR |= 2 << 15;  	//����PB15������ٶ� 50MHZ	
  GPIOB->ODR &= ~(1<<15);			  //PB15����͵�ƽ	
}

