
#include "KB926.H"
					 
/***********************************************************************************
	GPIO寄存器相关声明
***********************************************************************************/
GPIO_S_TypeDef xdata GPIOFS _at_ GPIOFS_BASE;	 				//	定义GPIO的功能选择寄存器组 
GPIO_A_TypeDef xdata GPIOOE _at_ GPIOOE_BASE;  					//	定义GPIO的输出使能寄存器组  
GPIO_A_TypeDef xdata GPIOOUT _at_ GPIOOUT_BASE;					//	定义GPIO的输出数据寄存器组
GPIO_A_TypeDef xdata GPIOIN _at_ GPIOIN_BASE;	      		   	//	定义GPIO的输入数据寄存器组 
GPIO_S_TypeDef xdata GPIOPU _at_ GPIOPU_BASE;	  				//	定义GPIO的上拉使能寄存器组
GPIO_S_TypeDef xdata GPIOOD _at_ GPIOOD_BASE;	  				//	定义GPIO的漏极开路使能寄存器组 
GPIO_A_TypeDef xdata GPIOIE _at_ GPIOIE_BASE;	   				//	定义GPIO的输入使能寄存器组	
unsigned char xdata GPIO_MISC _at_ GPIO_MISC_BASE;	   	   	    //	定义GPIO杂项控制寄存器  	 
																						 
/***********************************************************************************
	时钟寄存器相关声明
***********************************************************************************/
unsigned char xdata CLKCFG _at_ 0xFF0D;							//	定义EC时钟配置寄存器   
unsigned char xdata PLLFRH _at_ 0xFF16;							//	定义EC PLL频率配置高字节寄存器 
unsigned char xdata PLLFRL _at_ 0xFF17;							//	定义EC PLL频率配置高字节寄存器

/***********************************************************************************
	串口操作相关声明
***********************************************************************************/
void ComInit(unsigned long baud,unsigned long dpll)						//	初始化串口，参数baud为波特率，参数dpll为芯片DPLL时钟频率（Hz单位）
{	
	unsigned long tmp = (dpll/baud)-1;
	//	设置相关IO口		
	GPIOFS.GPIO1.BYTE[0] |= 1<<6;				//	设定GPIO16 TXD为其他功能输出	
	GPIOOE.GPIO1.BYTE[0] |= 1<<6;				//	使能GPIO16 TXD输出		
	GPIOIE.GPIO1.BYTE[0] |= 1<<7;				//	使能GPIO17 RXD输入	
	GPIOPU.GPIO1.BYTE[0] |= 1<<7;				//	使能位上拉输入	  	
	
	//	配置串口工作方式
	SCON &= ~(1<<7);			   				//	设定工作方式为8位异步串口
	SCON |= 1<<6;	 							//	设定工作方式为8位异步串口
	SCON |= 1<<4;								//	使能接收

	//	计算波特率
	SCON2 = (unsigned char)((tmp/0x100)&0x0FF);
	SCON3 = (unsigned char)(tmp%0x100);	

	ES = 1;										//	开启串口中断
	EA = 1;										//	开启总中断
}
	
void SEND_ONE_BYTE(unsigned char in)									//	发送一个字节，独占CPU的方式
{		  				  
	SBUF = in;	   														//	发送数据
	while(TI==0) ;														//	等待发送完成(有TI事件产生)
	TI=0;														   		//	清除事件标志
}

void SendString(char *p) 												//	发送一个字符串
{
	unsigned int i =0;
	while(*(p+i) != '\0')
	{
		SEND_ONE_BYTE(*(p+i));
		i++;
	}
}	 


/***********************************************************************************
	定时计数器相关声明
	注意，进入定时器的时钟频率为DPLL的12分频 
	装载值为溢出率，即定时器是不断自加的
***********************************************************************************/
unsigned int TIMER_COUNT_VALUE;											//	当作为定时器的时候，用于重新装载。
void Timer0_16bitInit(unsigned int ARR)									//	作为16位定时器初始化
{
	TIMER_COUNT_VALUE = ARR;					//	设置装载值
	TMOD &= ~(1<<1);							//	设定为16位定模式  
	TMOD |= 1<<0;								//	设定为16位定模式   
	TMOD &= ~(1<<2);							//	设定为定时模式	   
	TMOD &= ~(1<<3);							//	不需要INIT0#的参与即可使能工作
	TH0 = (unsigned char)((TIMER_COUNT_VALUE/0x100)&0x0FF);				//	装载计数值	 
	TL0 = (unsigned char)(TIMER_COUNT_VALUE&0x0FF);						//	装载计数值
	ET0 = 1;									//	使能定时器0中断
	EA = 1;										//	开总中断				 
	TR0 = 1;									//	启动定时器0
}


/***********************************************************************************
	X-Bus 接口相关声明																SPICMD			
***********************************************************************************/	
unsigned char xdata XBICS _at_ 0xFEA6; 									//	XBI E51CS# Configuration  	 
unsigned char xdata SPIA0 _at_ 0xFEA8; 									//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]	
unsigned char xdata SPIA1 _at_ 0xFEA9; 									//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
unsigned char xdata SPIA2 _at_ 0xFEAA; 									//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
unsigned char xdata SPIDAT _at_ 0xFEAB; 								//	XBI SPI Flash Output/Input Data Port									   
unsigned char xdata SPICMD _at_ 0xFEAC; 								//	XBI SPI Flash Command Port


unsigned char xdata ECMISC _at_ 0xFF20;


/***********************************************************************************
	ADC 接口相关声明																	
***********************************************************************************/	
//-------------------ADC相关寄存器----------------------									   			
unsigned char xdata ADDAEN 		_at_	0xFF15;	   	   			//			   			
unsigned char xdata ADCTRL 		_at_	0xFF18;	   	   			//	  			
unsigned char xdata ADCDAT 		_at_	0xFF19;	   	   			//			
unsigned char xdata ECIF 		_at_	0xFF1A;	   	   			//	
