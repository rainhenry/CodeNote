
#include "KB926.H"
					 
/***********************************************************************************
	GPIO�Ĵ����������
***********************************************************************************/
GPIO_S_TypeDef xdata GPIOFS _at_ GPIOFS_BASE;	 				//	����GPIO�Ĺ���ѡ��Ĵ����� 
GPIO_A_TypeDef xdata GPIOOE _at_ GPIOOE_BASE;  					//	����GPIO�����ʹ�ܼĴ�����  
GPIO_A_TypeDef xdata GPIOOUT _at_ GPIOOUT_BASE;					//	����GPIO��������ݼĴ�����
GPIO_A_TypeDef xdata GPIOIN _at_ GPIOIN_BASE;	      		   	//	����GPIO���������ݼĴ����� 
GPIO_S_TypeDef xdata GPIOPU _at_ GPIOPU_BASE;	  				//	����GPIO������ʹ�ܼĴ�����
GPIO_S_TypeDef xdata GPIOOD _at_ GPIOOD_BASE;	  				//	����GPIO��©����·ʹ�ܼĴ����� 
GPIO_A_TypeDef xdata GPIOIE _at_ GPIOIE_BASE;	   				//	����GPIO������ʹ�ܼĴ�����	
unsigned char xdata GPIO_MISC _at_ GPIO_MISC_BASE;	   	   	    //	����GPIO������ƼĴ���  	 
																						 
/***********************************************************************************
	ʱ�ӼĴ����������
***********************************************************************************/
unsigned char xdata CLKCFG _at_ 0xFF0D;							//	����ECʱ�����üĴ���   
unsigned char xdata PLLFRH _at_ 0xFF16;							//	����EC PLLƵ�����ø��ֽڼĴ��� 
unsigned char xdata PLLFRL _at_ 0xFF17;							//	����EC PLLƵ�����ø��ֽڼĴ���

/***********************************************************************************
	���ڲ����������
***********************************************************************************/
void ComInit(unsigned long baud,unsigned long dpll)						//	��ʼ�����ڣ�����baudΪ�����ʣ�����dpllΪоƬDPLLʱ��Ƶ�ʣ�Hz��λ��
{	
	unsigned long tmp = (dpll/baud)-1;
	//	�������IO��		
	GPIOFS.GPIO1.BYTE[0] |= 1<<6;				//	�趨GPIO16 TXDΪ�����������	
	GPIOOE.GPIO1.BYTE[0] |= 1<<6;				//	ʹ��GPIO16 TXD���		
	GPIOIE.GPIO1.BYTE[0] |= 1<<7;				//	ʹ��GPIO17 RXD����	
	GPIOPU.GPIO1.BYTE[0] |= 1<<7;				//	ʹ��λ��������	  	
	
	//	���ô��ڹ�����ʽ
	SCON &= ~(1<<7);			   				//	�趨������ʽΪ8λ�첽����
	SCON |= 1<<6;	 							//	�趨������ʽΪ8λ�첽����
	SCON |= 1<<4;								//	ʹ�ܽ���

	//	���㲨����
	SCON2 = (unsigned char)((tmp/0x100)&0x0FF);
	SCON3 = (unsigned char)(tmp%0x100);	

	ES = 1;										//	���������ж�
	EA = 1;										//	�������ж�
}
	
void SEND_ONE_BYTE(unsigned char in)									//	����һ���ֽڣ���ռCPU�ķ�ʽ
{		  				  
	SBUF = in;	   														//	��������
	while(TI==0) ;														//	�ȴ��������(��TI�¼�����)
	TI=0;														   		//	����¼���־
}

void SendString(char *p) 												//	����һ���ַ���
{
	unsigned int i =0;
	while(*(p+i) != '\0')
	{
		SEND_ONE_BYTE(*(p+i));
		i++;
	}
}	 


/***********************************************************************************
	��ʱ�������������
	ע�⣬���붨ʱ����ʱ��Ƶ��ΪDPLL��12��Ƶ 
	װ��ֵΪ����ʣ�����ʱ���ǲ����Լӵ�
***********************************************************************************/
unsigned int TIMER_COUNT_VALUE;											//	����Ϊ��ʱ����ʱ����������װ�ء�
void Timer0_16bitInit(unsigned int ARR)									//	��Ϊ16λ��ʱ����ʼ��
{
	TIMER_COUNT_VALUE = ARR;					//	����װ��ֵ
	TMOD &= ~(1<<1);							//	�趨Ϊ16λ��ģʽ  
	TMOD |= 1<<0;								//	�趨Ϊ16λ��ģʽ   
	TMOD &= ~(1<<2);							//	�趨Ϊ��ʱģʽ	   
	TMOD &= ~(1<<3);							//	����ҪINIT0#�Ĳ��뼴��ʹ�ܹ���
	TH0 = (unsigned char)((TIMER_COUNT_VALUE/0x100)&0x0FF);				//	װ�ؼ���ֵ	 
	TL0 = (unsigned char)(TIMER_COUNT_VALUE&0x0FF);						//	װ�ؼ���ֵ
	ET0 = 1;									//	ʹ�ܶ�ʱ��0�ж�
	EA = 1;										//	�����ж�				 
	TR0 = 1;									//	������ʱ��0
}


/***********************************************************************************
	X-Bus �ӿ��������																SPICMD			
***********************************************************************************/	
unsigned char xdata XBICS _at_ 0xFEA6; 									//	XBI E51CS# Configuration  	 
unsigned char xdata SPIA0 _at_ 0xFEA8; 									//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]	
unsigned char xdata SPIA1 _at_ 0xFEA9; 									//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
unsigned char xdata SPIA2 _at_ 0xFEAA; 									//	XBI SPI Flash Address (22-bit) = [SPIA2(6bit) : SPIA1(8bit) : SPIA0(8bit)]
unsigned char xdata SPIDAT _at_ 0xFEAB; 								//	XBI SPI Flash Output/Input Data Port									   
unsigned char xdata SPICMD _at_ 0xFEAC; 								//	XBI SPI Flash Command Port


unsigned char xdata ECMISC _at_ 0xFF20;


/***********************************************************************************
	ADC �ӿ��������																	
***********************************************************************************/	
//-------------------ADC��ؼĴ���----------------------									   			
unsigned char xdata ADDAEN 		_at_	0xFF15;	   	   			//			   			
unsigned char xdata ADCTRL 		_at_	0xFF18;	   	   			//	  			
unsigned char xdata ADCDAT 		_at_	0xFF19;	   	   			//			
unsigned char xdata ECIF 		_at_	0xFF1A;	   	   			//	
