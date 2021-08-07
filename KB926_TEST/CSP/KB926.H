
#ifndef __KB926_H__
#define __KB926_H__

/***********************************************************************************
	��ͳ8051����Ĵ����������
***********************************************************************************/
//	P0�ж�ʹ�ܼĴ���(P0 Interrupt Enable Register)
sfr P0IE 		=		0x80;
	sbit P0IE0 = 0x80;		  
	sbit P0IE1 = 0x81;
	sbit P0IE2 = 0x82;
	sbit P0IE3 = 0x83;
	sbit P0IE4 = 0x84;
	sbit P0IE5 = 0x85;
	sbit P0IE6 = 0x86;
	sbit P0IE7 = 0x87;		 

//	��ջָ��(Stack Pointer)
sfr	SP			=		0x81;  

//	����ָ����ֽ�(Data Pointer Low Byte)
sfr	DPL			=		0x82;			 

//	����ָ����ֽ�(Data Pointer High Byte)
sfr	DPH			=		0x83;
		
//	���������ƼĴ���2(Processor Control Register 2)
sfr	PCON2		=		0x86;	

//	���������ƼĴ���(Processor Control Register)
sfr	PCON		=		0x87;		

//	��ʱ���������ƼĴ���(Timer/Counter Control Register)
sfr	TCON		=		0x88;
	sbit IT0 = 0x88;			 
	sbit IE0 = 0x89;	
	sbit IT1 = 0x8A;	
	sbit IE1 = 0x8B;	
	sbit TR0 = 0x8C;	
	sbit TF0 = 0x8D;	
	sbit TR1 = 0x8E;	
	sbit TF1 = 0x8F;

//	��ʱ��ģʽ�Ĵ���(Timer Mode Register)
sfr	TMOD		=		0x89;	

//	��ʱ��0���ֽ�(Timer 0 Low Byte)
sfr	TL0			=		0x8A;	   

//	��ʱ��1���ֽ�(Timer 1 Low Byte)
sfr	TL1			=		0x8B;		 

//	��ʱ��0���ֽ�(Timer 0 High Byte)
sfr	TH0			=		0x8C;	   

//	��ʱ��1���ֽ�(Timer 1 High Byte)
sfr	TH1			=		0x8D;	
								  
//	P1�ж�ʹ�ܼĴ���(P1 Interrupt Enable Register)
sfr P1IE 		=		0x90;
	sbit P1IE0 = 0x90;		  
	sbit P1IE1 = 0x91;
	sbit P1IE2 = 0x92;
	sbit P1IE3 = 0x93;
	sbit P1IE4 = 0x94;
	sbit P1IE5 = 0x95;
	sbit P1IE6 = 0x96;
	sbit P1IE7 = 0x97;	   	
			  
//	���ڿ��ƼĴ���(Serial Port Control Register)
sfr SCON 		=		0x98;
	sbit RI  = 0x98;
	sbit TI  = 0x99;
	sbit RB8 = 0x9A;
	sbit TB8 = 0x9B;
	sbit REN = 0x9C;
	//	SCON.5 ������λ��ַ0x9D;
	sbit SM0 = 0x9E;
	sbit SM1 = 0x9F;	

//	�������ݻ���Ĵ���(Serial Port Data Buffer Register)
sfr SBUF 		=		0x99;

//	���ڿ��ƼĴ���2(Serial Port Control Register 2)
sfr SCON2 		=		0x9A;	   			//	�����ʸ��ֽ�

//	���ڿ��ƼĴ���3(Serial Port Control Register 3)
sfr SCON3 		=		0x9B;		 		//	�����ʵ��ֽ�

//	�˿�2�Ĵ���(Port 2 Register)
sfr P2			=		0xA0;
	sbit P20 = 0xA0;			
	sbit P21 = 0xA1;
	sbit P22 = 0xA2;
	sbit P23 = 0xA3;
	sbit P24 = 0xA4;
	sbit P25 = 0xA5;
	sbit P26 = 0xA6;
	sbit P27 = 0xA7;

//	�ж�ʹ�ܼĴ���
sfr IE			=		0xA8;
	sbit EX0 = 0xA8;
	sbit ET0 = 0xA9;
	sbit EX1 = 0xAA;
	sbit ET1 = 0xAB;
	sbit ES  = 0xAC;  
	//	IE.5 ������λ��ַ0xAD;
	//	IE.6 ������λ��ַ0xAE;
	sbit EA  = 0xAF;
			 
//	P3�ж�ʹ�ܼĴ���(P3 Interrupt Enable Register)
sfr P3IE 		=		0xB0;
	sbit P3IE0 = 0xB0;		  
	sbit P3IE1 = 0xB1;
	sbit P3IE2 = 0xB2;
	sbit P3IE3 = 0xB3;
	sbit P3IE4 = 0xB4;
	sbit P3IE5 = 0xB5;
	sbit P3IE6 = 0xB6;
	sbit P3IE7 = 0xB7;	 

//	�ж����ȼ��Ĵ���(Interrupt Priority Register)
sfr IP			=		0xB8;
	sbit PX0 = 0xB8;
	sbit PT0 = 0xB9;
	sbit PX1 = 0xBA;
	sbit PT1 = 0xBB;
	sbit PS  = 0xBC;   
	//	IP.5 ������λ��ַ0xBD;
	//	IP.6 ������λ��ַ0xBE;	
	//	IP.7 ������λ��ַ0xBF;

//	������״̬�ּĴ���
sfr PSW			=		0xD0;
	sbit P   = 0xD0;
	sbit F1  = 0xD1;
	sbit OV  = 0xD2;
	sbit RS0 = 0xD3;
	sbit RS1 = 0xD4;
	sbit F0  = 0xD5;
	sbit AC  = 0xD6;
	sbit CY  = 0xD7; 		
	
//	P0�жϱ�־�Ĵ���(P0 Interrupt Flag Register)
sfr P0IF 		=		0xD8;
	sbit P0IF0 = 0xD8;		  
	sbit P0IF1 = 0xD9;
	sbit P0IF2 = 0xDA;
	sbit P0IF3 = 0xDB;
	sbit P0IF4 = 0xDC;
	sbit P0IF5 = 0xDD;
	sbit P0IF6 = 0xDE;
	sbit P0IF7 = 0xDF;	 

//	�ۼ���(Accumulator)
sfr ACC			=		0xE0;
	sbit ACC0 = 0xE0;		  
	sbit ACC1 = 0xE1;
	sbit ACC2 = 0xE2;
	sbit ACC3 = 0xE3;
	sbit ACC4 = 0xE4;
	sbit ACC5 = 0xE5;
	sbit ACC6 = 0xE6;
	sbit ACC7 = 0xE7;
	
//	P1�жϱ�־�Ĵ���(P1 Interrupt Flag Register)
sfr P1IF 		=		0xE8;
	sbit P1IF0 = 0xE8;		  
	sbit P1IF1 = 0xE9;
	sbit P1IF2 = 0xEA;
	sbit P1IF3 = 0xEB;
	sbit P1IF4 = 0xEC;
	sbit P1IF5 = 0xED;
	sbit P1IF6 = 0xEE;
	sbit P1IF7 = 0xEF;	 

//	B�Ĵ���(B Register)
sfr B			=		0xF0;
	sbit B0 = 0xF0;			   
	sbit B1 = 0xF1;
	sbit B2 = 0xF2;
	sbit B3 = 0xF3;
	sbit B4 = 0xF4;
	sbit B5 = 0xF5;
	sbit B6 = 0xF6;
	sbit B7 = 0xF7;
		
//	P3�жϱ�־�Ĵ���(P3 Interrupt Flag Register)
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
	GPIO�Ĵ����������
***********************************************************************************/	
#define GPIO_BASE            	(0xFC00)			  					//	����GPIO������׵�ַ   
				
//	����16λ���ֲ������ֽڲ����Ĺ�����
typedef union tagSTRUCT16BIT
{
	unsigned short WORD;
	unsigned char BYTE[2];
}STRUCT16BIT;			
							 
//	����GPIO��ȫ�����ʽṹ��
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
 
//	����GPIO�Ĳ��ַ��ʽṹ��
typedef struct tagGPIO_S_TypeDef
{
	STRUCT16BIT		GPIO0;	 
	STRUCT16BIT		GPIO1; 	 
	STRUCT16BIT		GPIO2;	 
	STRUCT16BIT		GPIO3;	 
	STRUCT16BIT		GPIO4;	 
	STRUCT16BIT		GPIO5;
}GPIO_S_TypeDef;				  
				
//-------------------����ѡ��Ĵ�����----------------------
#define GPIOFS_BASE            	(GPIO_BASE+0x0000)			  			//	����GPIO�Ĺ���ѡ��������׵�ַ 
extern GPIO_S_TypeDef xdata GPIOFS;										//	����GPIO�Ĺ���ѡ��Ĵ����� 
//	��DAC���õ�ʱ�����ﲻ������GPIO3.8~GPIO3.15Ϊ1����
//	GPIO4.2~GPIO4.3�������ܵ����
//	����0Ϊͨ��IO������1Ϊ��������

//-------------------���ʹ�ܼĴ�����----------------------				
#define GPIOOE_BASE            	(GPIO_BASE+0x0010)			  			//	����GPIO�����ʹ��������׵�ַ 	
extern GPIO_A_TypeDef xdata GPIOOE;										//	����GPIO�����ʹ�ܼĴ�����  
//	0Ϊ����رգ�1Ϊ�����

//-------------------������ݼĴ�����----------------------				
#define GPIOOUT_BASE            (GPIO_BASE+0x0020)			  			//	����GPIO���������������׵�ַ 
extern GPIO_A_TypeDef xdata GPIOOUT;	   								//	����GPIO��������ݼĴ�����
//	GPIO38~GPIO3B��ΪGPIO3.8~GPIO3.11û��������� 	  

//-------------------�������ݼĴ�����----------------------				
#define GPIOIN_BASE            	(GPIO_BASE+0x0030)			  			//	����GPIO����������������׵�ַ 
extern GPIO_A_TypeDef xdata GPIOIN;	   									//	����GPIO���������ݼĴ�����
//	GPIO3C~GPIO3F��ΪGPIO3.12~GPIO3.15û�����빦��
//	ֻ��

//-------------------����ʹ�ܼĴ�����----------------------				
#define GPIOPU_BASE            	(GPIO_BASE+0x0040)			  			//	����GPIO������ʹ��������׵�ַ 
extern GPIO_S_TypeDef xdata GPIOPU;	   									//	����GPIO������ʹ�ܼĴ�����
//	GPIO3.8~GPIO3.15Ϊ������
//	0ȡ��������1ʹ������

//-------------------©����·ʹ�ܼĴ�����----------------------				
#define GPIOOD_BASE            	(GPIO_BASE+0x0050)			  			//	����GPIO��©����·ʹ��������׵�ַ 
extern GPIO_S_TypeDef xdata GPIOOD;	   									//	����GPIO��©����·ʹ�ܼĴ����� 
//	GPIO3.8~GPIO3.15Ϊ������
//	0©����·��Ч��1����©����·

//-------------------����ʹ�ܼĴ�����----------------------				
#define GPIOIE_BASE            	(GPIO_BASE+0x0060)			  			//	����GPIO������ʹ��������׵�ַ 	
extern GPIO_A_TypeDef xdata GPIOIE;	   	   								//	����GPIO������ʹ�ܼĴ�����	
//	GPIO3C~GPIO3F��ΪGPIO3.12~GPIO3.15û�����빦��
//	0�ر����빦�ܣ�1�����빦��

//-------------------GPIO������ƼĴ���----------------------									   			
#define GPIO_MISC_BASE          (GPIO_BASE+0x0070)			  			//	����GPIO������ƼĴ�����ַ
extern unsigned char xdata GPIO_MISC;	   	   							//	����GPIO������ƼĴ���

//-------------------ADC��ؼĴ���----------------------									   			
extern unsigned char xdata ADDAEN;	   	   								//			   			
extern unsigned char xdata ADCTRL;	   	   								//	   			
extern unsigned char xdata ADCDAT;	   	   								//	   			
extern unsigned char xdata ECIF;	   	   								//

		 
/***********************************************************************************
	ʱ�ӼĴ����������
***********************************************************************************/
extern unsigned char xdata CLKCFG;										//	����ECʱ�����üĴ���	 
extern unsigned char xdata PLLFRH;										//	����EC PLLƵ�����ø��ֽڼĴ��� 
extern unsigned char xdata PLLFRL;										//	����EC PLLƵ�����ø��ֽڼĴ���


/***********************************************************************************
	���ڲ����������
***********************************************************************************/
void ComInit(unsigned long baud,unsigned long dpll);					//	��ʼ�����ڣ�����baudΪ�����ʣ�����dpllΪоƬDPLLʱ��Ƶ�ʣ�Hz��λ��	
void SEND_ONE_BYTE(unsigned char in);									//	����һ���ֽڣ���ռCPU�ķ�ʽ	  
void SendString(char *p); 												//	����һ���ַ���

																					   
/***********************************************************************************
	��ʱ�������������		 
	ע�⣬���붨ʱ����ʱ��Ƶ��ΪDPLL��12��Ƶ
	װ��ֵΪ����ʣ�����ʱ���ǲ����Լӵ�
***********************************************************************************/
extern unsigned int TIMER_COUNT_VALUE;									//	����Ϊ��ʱ����ʱ����������װ�ء�
void Timer0_16bitInit(unsigned int ARR);								//	��Ϊ16λ��ʱ����ʼ��
								
/***********************************************************************************
	X-Bus �ӿ��������
***********************************************************************************/
extern unsigned char xdata XBICS; 										//	XBI E51CS# Configuration  
extern unsigned char xdata SPIA0;										//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]	
extern unsigned char xdata SPIA1; 										//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
extern unsigned char xdata SPIA2; 										//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
extern unsigned char xdata SPIDAT; 										//	XBI SPI Flash Output/Input Data Port									   
extern unsigned char xdata SPICMD; 										//	XBI SPI Flash Command Port


extern unsigned char xdata ECMISC;

#endif //	__KB926_H__
