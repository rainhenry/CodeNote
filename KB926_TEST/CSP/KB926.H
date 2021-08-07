
#ifndef __KB926_H__
#define __KB926_H__

/***********************************************************************************
	传统8051特殊寄存器相关声明
***********************************************************************************/
//	P0中断使能寄存器(P0 Interrupt Enable Register)
sfr P0IE 		=		0x80;
	sbit P0IE0 = 0x80;		  
	sbit P0IE1 = 0x81;
	sbit P0IE2 = 0x82;
	sbit P0IE3 = 0x83;
	sbit P0IE4 = 0x84;
	sbit P0IE5 = 0x85;
	sbit P0IE6 = 0x86;
	sbit P0IE7 = 0x87;		 

//	堆栈指针(Stack Pointer)
sfr	SP			=		0x81;  

//	数据指针低字节(Data Pointer Low Byte)
sfr	DPL			=		0x82;			 

//	数据指针高字节(Data Pointer High Byte)
sfr	DPH			=		0x83;
		
//	处理器控制寄存器2(Processor Control Register 2)
sfr	PCON2		=		0x86;	

//	处理器控制寄存器(Processor Control Register)
sfr	PCON		=		0x87;		

//	定时计数器控制寄存器(Timer/Counter Control Register)
sfr	TCON		=		0x88;
	sbit IT0 = 0x88;			 
	sbit IE0 = 0x89;	
	sbit IT1 = 0x8A;	
	sbit IE1 = 0x8B;	
	sbit TR0 = 0x8C;	
	sbit TF0 = 0x8D;	
	sbit TR1 = 0x8E;	
	sbit TF1 = 0x8F;

//	定时器模式寄存器(Timer Mode Register)
sfr	TMOD		=		0x89;	

//	定时器0低字节(Timer 0 Low Byte)
sfr	TL0			=		0x8A;	   

//	定时器1低字节(Timer 1 Low Byte)
sfr	TL1			=		0x8B;		 

//	定时器0高字节(Timer 0 High Byte)
sfr	TH0			=		0x8C;	   

//	定时器1高字节(Timer 1 High Byte)
sfr	TH1			=		0x8D;	
								  
//	P1中断使能寄存器(P1 Interrupt Enable Register)
sfr P1IE 		=		0x90;
	sbit P1IE0 = 0x90;		  
	sbit P1IE1 = 0x91;
	sbit P1IE2 = 0x92;
	sbit P1IE3 = 0x93;
	sbit P1IE4 = 0x94;
	sbit P1IE5 = 0x95;
	sbit P1IE6 = 0x96;
	sbit P1IE7 = 0x97;	   	
			  
//	串口控制寄存器(Serial Port Control Register)
sfr SCON 		=		0x98;
	sbit RI  = 0x98;
	sbit TI  = 0x99;
	sbit RB8 = 0x9A;
	sbit TB8 = 0x9B;
	sbit REN = 0x9C;
	//	SCON.5 保留，位地址0x9D;
	sbit SM0 = 0x9E;
	sbit SM1 = 0x9F;	

//	串口数据缓冲寄存器(Serial Port Data Buffer Register)
sfr SBUF 		=		0x99;

//	串口控制寄存器2(Serial Port Control Register 2)
sfr SCON2 		=		0x9A;	   			//	波特率高字节

//	串口控制寄存器3(Serial Port Control Register 3)
sfr SCON3 		=		0x9B;		 		//	波特率低字节

//	端口2寄存器(Port 2 Register)
sfr P2			=		0xA0;
	sbit P20 = 0xA0;			
	sbit P21 = 0xA1;
	sbit P22 = 0xA2;
	sbit P23 = 0xA3;
	sbit P24 = 0xA4;
	sbit P25 = 0xA5;
	sbit P26 = 0xA6;
	sbit P27 = 0xA7;

//	中断使能寄存器
sfr IE			=		0xA8;
	sbit EX0 = 0xA8;
	sbit ET0 = 0xA9;
	sbit EX1 = 0xAA;
	sbit ET1 = 0xAB;
	sbit ES  = 0xAC;  
	//	IE.5 保留，位地址0xAD;
	//	IE.6 保留，位地址0xAE;
	sbit EA  = 0xAF;
			 
//	P3中断使能寄存器(P3 Interrupt Enable Register)
sfr P3IE 		=		0xB0;
	sbit P3IE0 = 0xB0;		  
	sbit P3IE1 = 0xB1;
	sbit P3IE2 = 0xB2;
	sbit P3IE3 = 0xB3;
	sbit P3IE4 = 0xB4;
	sbit P3IE5 = 0xB5;
	sbit P3IE6 = 0xB6;
	sbit P3IE7 = 0xB7;	 

//	中断优先级寄存器(Interrupt Priority Register)
sfr IP			=		0xB8;
	sbit PX0 = 0xB8;
	sbit PT0 = 0xB9;
	sbit PX1 = 0xBA;
	sbit PT1 = 0xBB;
	sbit PS  = 0xBC;   
	//	IP.5 保留，位地址0xBD;
	//	IP.6 保留，位地址0xBE;	
	//	IP.7 保留，位地址0xBF;

//	处理器状态字寄存器
sfr PSW			=		0xD0;
	sbit P   = 0xD0;
	sbit F1  = 0xD1;
	sbit OV  = 0xD2;
	sbit RS0 = 0xD3;
	sbit RS1 = 0xD4;
	sbit F0  = 0xD5;
	sbit AC  = 0xD6;
	sbit CY  = 0xD7; 		
	
//	P0中断标志寄存器(P0 Interrupt Flag Register)
sfr P0IF 		=		0xD8;
	sbit P0IF0 = 0xD8;		  
	sbit P0IF1 = 0xD9;
	sbit P0IF2 = 0xDA;
	sbit P0IF3 = 0xDB;
	sbit P0IF4 = 0xDC;
	sbit P0IF5 = 0xDD;
	sbit P0IF6 = 0xDE;
	sbit P0IF7 = 0xDF;	 

//	累加器(Accumulator)
sfr ACC			=		0xE0;
	sbit ACC0 = 0xE0;		  
	sbit ACC1 = 0xE1;
	sbit ACC2 = 0xE2;
	sbit ACC3 = 0xE3;
	sbit ACC4 = 0xE4;
	sbit ACC5 = 0xE5;
	sbit ACC6 = 0xE6;
	sbit ACC7 = 0xE7;
	
//	P1中断标志寄存器(P1 Interrupt Flag Register)
sfr P1IF 		=		0xE8;
	sbit P1IF0 = 0xE8;		  
	sbit P1IF1 = 0xE9;
	sbit P1IF2 = 0xEA;
	sbit P1IF3 = 0xEB;
	sbit P1IF4 = 0xEC;
	sbit P1IF5 = 0xED;
	sbit P1IF6 = 0xEE;
	sbit P1IF7 = 0xEF;	 

//	B寄存器(B Register)
sfr B			=		0xF0;
	sbit B0 = 0xF0;			   
	sbit B1 = 0xF1;
	sbit B2 = 0xF2;
	sbit B3 = 0xF3;
	sbit B4 = 0xF4;
	sbit B5 = 0xF5;
	sbit B6 = 0xF6;
	sbit B7 = 0xF7;
		
//	P3中断标志寄存器(P3 Interrupt Flag Register)
sfr P3IF 		=		0xF8;
	sbit P3IF0 = 0xF8;		  
	sbit P3IF1 = 0xF9;
	sbit P3IF2 = 0xFA;
	sbit P3IF3 = 0xFB;
	sbit P3IF4 = 0xFC;
	sbit P3IF5 = 0xFD;
	sbit P3IF6 = 0xFE;
	sbit P3IF7 = 0xFF;	 

	
/***********************************************************************************
	GPIO寄存器相关声明
***********************************************************************************/	
#define GPIO_BASE            	(0xFC00)			  					//	定义GPIO区域的首地址   
				
//	定义16位的字操作与字节操作的共用体
typedef union tagSTRUCT16BIT
{
	unsigned short WORD;
	unsigned char BYTE[2];
}STRUCT16BIT;			
							 
//	定义GPIO的全部访问结构体
typedef struct tagGPIO_A_TypeDef
{				 
	STRUCT16BIT		GPIO0;	 
	STRUCT16BIT		GPIO1; 	 
	STRUCT16BIT		GPIO2;	 
	STRUCT16BIT		GPIO3;	 
	STRUCT16BIT		GPIO4;	 
	STRUCT16BIT		GPIO5;
	unsigned char 	GPIOA7_0;	 
	unsigned char 	GPIOA15_8;
	unsigned char 	GPIOA18_16;
	unsigned char 	GPIOD;	
}GPIO_A_TypeDef;				
 
//	定义GPIO的部分访问结构体
typedef struct tagGPIO_S_TypeDef
{
	STRUCT16BIT		GPIO0;	 
	STRUCT16BIT		GPIO1; 	 
	STRUCT16BIT		GPIO2;	 
	STRUCT16BIT		GPIO3;	 
	STRUCT16BIT		GPIO4;	 
	STRUCT16BIT		GPIO5;
}GPIO_S_TypeDef;				  
				
//-------------------功能选择寄存器组----------------------
#define GPIOFS_BASE            	(GPIO_BASE+0x0000)			  			//	定义GPIO的功能选择区域的首地址 
extern GPIO_S_TypeDef xdata GPIOFS;										//	定义GPIO的功能选择寄存器组 
//	当DAC启用的时候，这里不能设置GPIO3.8~GPIO3.15为1！！
//	GPIO4.2~GPIO4.3其他功能的输出
//	设置0为通用IO，设置1为其他功能

//-------------------输出使能寄存器组----------------------				
#define GPIOOE_BASE            	(GPIO_BASE+0x0010)			  			//	定义GPIO的输出使能区域的首地址 	
extern GPIO_A_TypeDef xdata GPIOOE;										//	定义GPIO的输出使能寄存器组  
//	0为输出关闭，1为输出打开

//-------------------输出数据寄存器组----------------------				
#define GPIOOUT_BASE            (GPIO_BASE+0x0020)			  			//	定义GPIO的输出数据区域的首地址 
extern GPIO_A_TypeDef xdata GPIOOUT;	   								//	定义GPIO的输出数据寄存器组
//	GPIO38~GPIO3B即为GPIO3.8~GPIO3.11没有输出功能 	  

//-------------------输入数据寄存器组----------------------				
#define GPIOIN_BASE            	(GPIO_BASE+0x0030)			  			//	定义GPIO的输入数据区域的首地址 
extern GPIO_A_TypeDef xdata GPIOIN;	   									//	定义GPIO的输入数据寄存器组
//	GPIO3C~GPIO3F即为GPIO3.12~GPIO3.15没有输入功能
//	只读

//-------------------上拉使能寄存器组----------------------				
#define GPIOPU_BASE            	(GPIO_BASE+0x0040)			  			//	定义GPIO的上拉使能区域的首地址 
extern GPIO_S_TypeDef xdata GPIOPU;	   									//	定义GPIO的上拉使能寄存器组
//	GPIO3.8~GPIO3.15为保留的
//	0取消上拉，1使能上拉

//-------------------漏极开路使能寄存器组----------------------				
#define GPIOOD_BASE            	(GPIO_BASE+0x0050)			  			//	定义GPIO的漏极开路使能区域的首地址 
extern GPIO_S_TypeDef xdata GPIOOD;	   									//	定义GPIO的漏极开路使能寄存器组 
//	GPIO3.8~GPIO3.15为保留的
//	0漏极开路无效，1启用漏极开路

//-------------------输入使能寄存器组----------------------				
#define GPIOIE_BASE            	(GPIO_BASE+0x0060)			  			//	定义GPIO的输入使能区域的首地址 	
extern GPIO_A_TypeDef xdata GPIOIE;	   	   								//	定义GPIO的输入使能寄存器组	
//	GPIO3C~GPIO3F即为GPIO3.12~GPIO3.15没有输入功能
//	0关闭输入功能，1打开输入功能

//-------------------GPIO杂项控制寄存器----------------------									   			
#define GPIO_MISC_BASE          (GPIO_BASE+0x0070)			  			//	定义GPIO杂项控制寄存器地址
extern unsigned char xdata GPIO_MISC;	   	   							//	定义GPIO杂项控制寄存器

//-------------------ADC相关寄存器----------------------									   			
extern unsigned char xdata ADDAEN;	   	   								//			   			
extern unsigned char xdata ADCTRL;	   	   								//	   			
extern unsigned char xdata ADCDAT;	   	   								//	   			
extern unsigned char xdata ECIF;	   	   								//

		 
/***********************************************************************************
	时钟寄存器相关声明
***********************************************************************************/
extern unsigned char xdata CLKCFG;										//	定义EC时钟配置寄存器	 
extern unsigned char xdata PLLFRH;										//	定义EC PLL频率配置高字节寄存器 
extern unsigned char xdata PLLFRL;										//	定义EC PLL频率配置高字节寄存器


/***********************************************************************************
	串口操作相关声明
***********************************************************************************/
void ComInit(unsigned long baud,unsigned long dpll);					//	初始化串口，参数baud为波特率，参数dpll为芯片DPLL时钟频率（Hz单位）	
void SEND_ONE_BYTE(unsigned char in);									//	发送一个字节，独占CPU的方式	  
void SendString(char *p); 												//	发送一个字符串

																					   
/***********************************************************************************
	定时计数器相关声明		 
	注意，进入定时器的时钟频率为DPLL的12分频
	装载值为溢出率，即定时器是不断自加的
***********************************************************************************/
extern unsigned int TIMER_COUNT_VALUE;									//	当作为定时器的时候，用于重新装载。
void Timer0_16bitInit(unsigned int ARR);								//	作为16位定时器初始化
								
/***********************************************************************************
	X-Bus 接口相关声明
***********************************************************************************/
extern unsigned char xdata XBICS; 										//	XBI E51CS# Configuration  
extern unsigned char xdata SPIA0;										//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]	
extern unsigned char xdata SPIA1; 										//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
extern unsigned char xdata SPIA2; 										//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
extern unsigned char xdata SPIDAT; 										//	XBI SPI Flash Output/Input Data Port									   
extern unsigned char xdata SPICMD; 										//	XBI SPI Flash Command Port


extern unsigned char xdata ECMISC;

#endif //	__KB926_H__

