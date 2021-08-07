
#include "KB926.H"
#include "User.h"
					 
/***********************************************************************************
	串口中断服务声明
***********************************************************************************/
void ComInterruptionService(void) interrupt 4
{
	if(TI==1)	  					//	发送中断
	{
		TI=0;
	}

	if(RI==1)				 		//	接收中断
	{
		RI=0;
		SEND_ONE_BYTE(SBUF);
	}
}

						  
/***********************************************************************************
	定时计数器0中断服务声明
***********************************************************************************/
void TimerCount0InterruptionService(void) interrupt 1
{
	//--------------------------------------------------------------
	//	定义用户变量等
	static unsigned int cp_status = 0;			//	状态机状态代码
	unsigned int CP = 0;						//	状态机计数
	static unsigned int count = 0;				//	分频计数器
	
	//--------------------------------------------------------------
	if(TF0==1)	  					//	定时计数器0溢出中断
	{																			
		TH0 = (unsigned char)((TIMER_COUNT_VALUE/0x100)&0x0FF);				//	装载计数值	 
		TL0 = (unsigned char)(TIMER_COUNT_VALUE&0x0FF);						//	装载计数值
		TF0=0;					  	//	清空中断标志   

		//--------------------------------------------
		//	分频处理
		++count;				//	累计
		//	大于10表示已经走完10次了
		if(count > 10)
		{
			count = 0;
		}
		else
		{
			return ;
		}
		
		//--------------------------------------------
		//	LED闪灯状态机
		CP = 0;
		//----------------------------------0
		if(cp_status == CP++)
		{
			GPIOOUT.GPIOA15_8 = 0x01;
			++cp_status;			
		}
		//----------------------------------1
		else if(cp_status == CP++)
		{
			GPIOOUT.GPIOA15_8 = 0x02;			
			++cp_status;			
		}		
		//----------------------------------2
		else if(cp_status == CP++)
		{				   
			GPIOOUT.GPIOA15_8 = 0x04;			
			++cp_status;				
		}
		//----------------------------------3
		else if(cp_status == CP++)
		{				   			   
			GPIOOUT.GPIOA15_8 = 0x08;		
			++cp_status;				
		}
		//----------------------------------4
		else if(cp_status == CP++)
		{				   			   					   
			GPIOOUT.GPIOA15_8 = 0x04;		
			++cp_status;			  		
		}
		//----------------------------------5
		else if(cp_status == CP++)
		{				   			  						   
			GPIOOUT.GPIOA15_8 = 0x02;		
			cp_status = 0;			
		}
		//----------------------------------非法状态
		else
		{
			cp_status = 0;
			CP = 0;
		}
				
		//GPIOOUT.GPIOA15_8 = ~GPIOOUT.GPIOA15_8;
	}
}
	 
/***********************************************************************************
	ADC中断服务声明
***********************************************************************************/
void ADCInterruptionService(void) interrupt 28
{
	//	清楚ADC中断标志
	P3IF7 = 0;
	GPIOOUT.GPIOA15_8 = 0x0F;
	ADC_FLAG = 1;
}

