#include "KB926.H"		
#include "User.h"
#include <stdio.h>
#include <string.h>

#define DELAY_TIME		100	 
#define F_DPLL			32768000L

#define KEY1_STATUS		((GPIOIN.GPIOA7_0|(~0x80))==(~0x80))
#define KEY2_STATUS		((GPIOIN.GPIOA7_0|(~0x40))==(~0x40))
#define KEY3_STATUS		((GPIOIN.GPIOA7_0|(~0x20))==(~0x20))
#define KEY4_STATUS		((GPIOIN.GPIOA7_0|(~0x10))==(~0x10))


unsigned int ADC_FLAG = 0;
unsigned int adc_val = 0;


void SoftwaveDelay(unsigned int in)
{
	unsigned int i,j;
	for(i=0;i<in;i++)
	{
		for(j=0;j<327;j++)
		{
		}
	}
}

int main()
{			
	char str[100];// _at_ 0xF400;
	unsigned int add = 0x1234;// = &str;
	STRUCT16BIT aa;					   

//	PCON2 |= 1<<4;		  					//	�������Ƭ��ģ��
//	PCON2 &= ~(1<<3);						//	����жϱ�־	 
//	XBICS |= 1<<3;						   	//	ʹ��XRAM
//	ECMISC |= 1<<7;							//	8051 state set as Normal state


	aa.WORD = 0x1234;
	//add = &str;
//	unsigned int tmp = (unsigned int)((F_DPLL/115200)-1);
	
		 


	CLKCFG |= 1<<6;							//	Flashʱ���趨Ϊȫ��ģʽ
	CLKCFG |= 3<<2;							//	�趨8051ʱ�Ӻ���Χ�豸��ʱ��Ϊ����ٶ�
	CLKCFG |= 1<<5;							//	ʹ��DPLL�Ĳ����趨
	
	//	�趨DPLLΪ0x3E8(1000)��Ƶ
	PLLFRH = 0x3E;
	PLLFRL &= 0x0F;
	PLLFRL |= 0x80;

	ComInit(115200L,F_DPLL);
	
	//	����Ĭ�ϵ����״̬
	GPIOOUT.GPIOA7_0 = 0X00;					//	Ĭ�����Ϊ�͵�ƽ

	//GPIOFS.GPIOA15_8 = 0x00; 				//	����GPIO0.0Ϊͨ��IO
	//GPIOFS.GPIOA7_0 = 0x00; 				//	����GPIO0.0Ϊͨ��IO
	GPIOOE.GPIOA15_8 = 0x0F;				//	ʹ��GPIOA��11~8Ϊ��� 
	//GPIOOE.GPIOA7_0 = (1<<0)|(1<<2);		//	ʹ��GPIOA��0��2Ϊ��� 		Ϊ����ʾ���������ֵ�λ���õ�
	GPIOIE.GPIOA7_0 = 0xF0;					//	ʹ��GPIOA��7~4Ϊ����

	//Timer0_16bitInit(0xFFFF-1000+1);		//	�����ʱ������ʱ�ӵ�1000��Ƶ  
	//Timer0_16bitInit((unsigned int)(65536L - (F_DPLL*0.001)/12L));		//	500ms
	Timer0_16bitInit(0);		//	500ms
	
	//--------------------------------------------------
	//	�������
	GPIOOUT.GPIOA7_0 &= ~(1<<0);
	GPIOOUT.GPIOA7_0 &= ~(1<<2);
	//GPIOOUT.GPIOA7_0 |= (1<<0);
	//GPIOOUT.GPIOA7_0 |= (1<<2);
	
	//--------------------------------------------------
	//	ADC��ʼ������
	//	Ӳ������ΪGPIO38/AD0���ӵ���λ�������뷶Χ0.1*AVCC��0.9*AVCC
	GPIOIE.GPIO3.WORD = 0x0000;
	ADDAEN |= 1<<0;
	P3IF7 = 0;
	//P3IE7 = 1;	//	ʹ��ADC�ж�	
	//EA = 1;
	ADCTRL = 1;			

	while(1)
	{
//		GPIOOUT.GPIOA15_8 = 0x01;			
//		SoftwaveDelay(DELAY_TIME);							   
//		GPIOOUT.GPIOA15_8 = 0x02;		
//		SoftwaveDelay(DELAY_TIME);							   
//		GPIOOUT.GPIOA15_8 = 0x04;		
//		SoftwaveDelay(DELAY_TIME); 							   
//		GPIOOUT.GPIOA15_8 = 0x08;		
//		SoftwaveDelay(DELAY_TIME);								   
//		GPIOOUT.GPIOA15_8 = 0x04;		
//		SoftwaveDelay(DELAY_TIME); 	  							   
//		GPIOOUT.GPIOA15_8 = 0x02;		
//		SoftwaveDelay(DELAY_TIME);
				 
	//	SEND_ONE_BYTE(0x5A);
	//	SEND_ONE_BYTE(*(((unsigned char *)(&tmp))+0));		   
	//	SEND_ONE_BYTE(*(((unsigned char *)(&tmp))+1));
	//	SEND_ONE_BYTE(0xA5);	  
//		SEND_ONE_BYTE('N');
//		SEND_ONE_BYTE('R');	  
//		SEND_ONE_BYTE('Y');	
//		SEND_ONE_BYTE('\r');
//		SEND_ONE_BYTE('\n');
//		memset(str,0,sizeof(str));
//		sprintf(str,"ADD = 0x%04X\r\n",add);
		//sprintf(str,"aa.WORD=0x%04X, aa.BYTE[0]=0x%02X,aa.BYTE[1]=0x%02X\r\n",aa.WORD,(unsigned int)aa.BYTE[0],(unsigned int)aa.BYTE[1]);
//		SendString(str);		
//		SoftwaveDelay(1000);	   

//		aa = (GPIOIN.GPIOA7_0&0xF0)>>4;
//		GPIOOUT.GPIOA15_8 = aa;
		
//		if(RI == 1)
//		{
//			RI = 0;
//			SEND_ONE_BYTE(SBUF);
//			GPIOOUT.GPIOA15_8 = ~GPIOOUT.GPIOA15_8;
//		}

//		if(tmp == 0x11B) GPIOOUT.GPIOA15_8 = 0x0F;
//		else GPIOOUT.GPIOA15_8 = 0x00;

	//------------------------------------------------------
	//	�������ܲ���
	#define DELAY_VALUE		10
	if(KEY1_STATUS)
	{
		SoftwaveDelay(50);							//	ǰ������
		sprintf(str,"KEY1 UpDown!\r\n");
		SendString(str);
		
		//	���ֵ�λ��ģ��	
		GPIOOUT.GPIOA7_0 |= (1<<0);
		SoftwaveDelay(DELAY_VALUE);			
		GPIOOUT.GPIOA7_0 |= (1<<2);	
		SoftwaveDelay(DELAY_VALUE);			
		GPIOOUT.GPIOA7_0 &= ~(1<<0);
		SoftwaveDelay(DELAY_VALUE);			
		GPIOOUT.GPIOA7_0 &= ~(1<<2);
		SoftwaveDelay(DELAY_VALUE);		
		
		while(KEY1_STATUS) ;						//	�ȴ������ͷ�
		SoftwaveDelay(50);							//	��������
			
	}
	if(KEY2_STATUS)
	{
		SoftwaveDelay(50);							//	ǰ������
		sprintf(str,"KEY2 UpDown!\r\n");
		SendString(str);
		
		//	���ֵ�λ��ģ��	
		GPIOOUT.GPIOA7_0 |= (1<<2);
		SoftwaveDelay(DELAY_VALUE);			
		GPIOOUT.GPIOA7_0 |= (1<<0);	
		SoftwaveDelay(DELAY_VALUE);			
		GPIOOUT.GPIOA7_0 &= ~(1<<2);
		SoftwaveDelay(DELAY_VALUE);			
		GPIOOUT.GPIOA7_0 &= ~(1<<0);
		SoftwaveDelay(DELAY_VALUE);	
		
		while(KEY2_STATUS) ;						//	�ȴ������ͷ�
		SoftwaveDelay(50);							//	��������
		
	}
	if(KEY3_STATUS)
	{
		SoftwaveDelay(50);							//	ǰ������
		sprintf(str,"KEY3 UpDown!\r\n");
		SendString(str);
		while(KEY3_STATUS) ;						//	�ȴ������ͷ�
		SoftwaveDelay(50);							//	��������
	}
	if(KEY4_STATUS)
	{
		SoftwaveDelay(50);							//	ǰ������
		sprintf(str,"KEY4 UpDown!\r\n");
		SendString(str);
		while(KEY4_STATUS) ;						//	�ȴ������ͷ�
		SoftwaveDelay(50);							//	��������
	}
	
	
	//------------------------------------------------------
	//	ѭ����ʱ
	SoftwaveDelay(100);	
	
	
	//GPIOIE.GPIO3.WORD = 1<<8;
	//ADDAEN |= 1<<0;
	
	//	���ò�ѯ��ʽʹ��ADC
	ADCTRL = 1;
	SoftwaveDelay(100);	
	adc_val = (ADCDAT*4)+ ((ECIF>>6)&0x03);
	sprintf(str,"val = 0x%04X\r\n",adc_val);
	SendString(str);
	
	//	�����жϷ�ʽʹ��ADC
	//	���ADC��־
	if(ADC_FLAG == 1)
	{
		SendString("ADC Interruption! ");
		adc_val = (ADCDAT*4)+ ((ECIF>>6)&0x03);
		sprintf(str,"val = 0x%04X\r\n",adc_val);
		SendString(str);
		ADC_FLAG = 0;
		ADCTRL = 1;
	}

	}
}
