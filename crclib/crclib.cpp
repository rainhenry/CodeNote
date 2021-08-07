/************************************************************************/
/*	CRC库源文件                                                         */
/*	支持CRC8 CRC16 CRC32												*/
/*	工程日期：2016.08.19												*/
/*	版本：REV 1.0														*/
/*	编码：rainhenry														*/
/*	设计：rainhenry														*/
/*	调试/校对：rainhenry												*/
/*	测试平台：Windows 7 64bit, VS2010									*/
/*	鸣谢：北极星的眼泪													*/
/*	声明：本代码可以自由转载使用，但仅供学习研究，切勿用于商业用途！	*/
/*		  转载使用时请保留本注释。										*/
/************************************************************************/

//包含头文件//////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "crclib.h"

//函数定义////////////////////////////////////////////////////////////////
//	CRC计算函数
//	返回CRC计算是否成功的代码
int CRC_Calc_Func(
	void* dat,						//	输入数据的首地址
	uint32 length,					//	输入数据的字节个数
	uint32* outdata,				//	输出数据的存放地址
	CRC_Calc_Config cfg				//	计算配置
	)
{
	//	声明相关变量，兼容C规范
	uint8 intmp;							//	定义输入CRC计算的临时变量
	uint8 *pdat = (uint8 *)dat;				//	初始化输入缓冲区的首地址
	uint32 i;								//	定义循环变量
	uint32 outtmp;							//	定义CRC计算的输出临时变量
	uint32 CRC_POLY = 0x00000000;			//	定义多项式值
	int re;									//	定义参数检查返回值

	uint32 ShiftBitNum;						//	计算对齐时需要移动的位的数目
	uint32 HighBitFilter;					//	高位过滤掩码

	//	检查入口指针
	if ((dat==NULL)||(dat==INVALID_ADDRESS))
	{
		return IN_ERROR_ADDRESS;
	}

	//	检查出口指针
	if ((outdata==NULL)||(outdata==INVALID_ADDRESS))
	{
		return OUT_ERROR_ADDRESS;
	}

	//	检查配置
	re = CheckCRCConfig(cfg);
	if (re != CHECK_OK)
	{
		return re;					//	失败
	} 

	//	设置CRC的初值
	switch(cfg.data_width)
	{
	case CRC8BIT:
		outtmp = 0x000000FF & cfg.Init;
		break;
	case CRC16BIT:
		outtmp = 0x0000FFFF & cfg.Init;
		break;
	case CRC32BIT:
		outtmp = 0xFFFFFFFF & cfg.Init;	
		break;
	}

	//	设置多项式的值
	switch(cfg.data_width)
	{
	case CRC8BIT:
		CRC_POLY = 0x000000FF & cfg.calc_poly.u32_dat;
		break;
	case CRC16BIT:
		CRC_POLY = 0x0000FFFF & cfg.calc_poly.u32_dat;
		break;
	case CRC32BIT:
		CRC_POLY = 0xFFFFFFFF & cfg.calc_poly.u32_dat;		
		break;
	}

	//	计算对齐移位数目
	switch(cfg.data_width)
	{
	case CRC8BIT:
		ShiftBitNum = 0;
		break;
	case CRC16BIT:
		ShiftBitNum = 8;
		break;
	case CRC32BIT:
		ShiftBitNum = 24;	
		break;
	}

	//	计算高位掩码
	switch(cfg.data_width)
	{
	case CRC8BIT:
		HighBitFilter = 0x00000080;
		break;
	case CRC16BIT:
		HighBitFilter = 0x00008000;
		break;
	case CRC32BIT:
		HighBitFilter = 0x80000000;
		break;
	}

	//	开始计算CRC
	while (length--)
	{
		intmp = *pdat++;										//	导入一个字节
		if(cfg.RefIn)	intmp = CRC_U8Invert(intmp);			//	输入翻转
		outtmp ^= (((uint32) intmp) << ShiftBitNum);		//	对齐到最高位
		for (i = 0; i < 8; i++)
		{
			if (outtmp & HighBitFilter)
				outtmp = (outtmp << 1) ^ CRC_POLY;
			else
				outtmp <<= 1;
		}
	}
	switch(cfg.data_width)										//	输出翻转
	{
	case CRC8BIT:
		if(cfg.RefOut)	outtmp = CRC_U8Invert((uint8)(outtmp&0x000000FF));
		break;
	case CRC16BIT:
		if(cfg.RefOut)	outtmp = CRC_U16Invert((uint16)(outtmp&0x0000FFFF));
		break;
	case CRC32BIT:
		if(cfg.RefOut)	outtmp = CRC_U32Invert(outtmp);
		break;
	}
	outtmp ^= cfg.XorOut;										//	输出异或

	//	输出结果
	switch(cfg.data_width)										//	输出翻转
	{
	case CRC8BIT:
		*outdata = outtmp & 0x000000FF;
		break;
	case CRC16BIT:
		*outdata = outtmp & 0x0000FFFF;
		break;
	case CRC32BIT:
		*outdata = outtmp & 0xFFFFFFFF;
		break;
	}

	//	返回计算状态
	return CRC_CALC_SUCCESS;
}

//	配置参数检查
int CheckCRCConfig(CRC_Calc_Config cfg)
{
	//	检查数据宽度
	if (!( \
		(cfg.data_width == CRC8BIT) || \
		(cfg.data_width == CRC16BIT) || \
		(cfg.data_width == CRC32BIT) \
		))
	{
		return DATA_WIDTH_FAIL;			//	失败
	}

	//	检查多项式
	if (cfg.calc_poly.u32_dat == 0)
	{
		return POLY_FAIL;				//	失败
	}

	return CHECK_OK;
}

//	8位字节翻转
uint8 CRC_U8Invert(uint8 chSrc)
{
	int i;
	uint8 det = 0x00;
	for (i=0;i<8;i++)
	{
		if ((chSrc&(1<<i)) == 0)
		{
		} 
		else
		{
			det |= 1<<(7-i);
		}
	}
	return det;
}

//	16位单字翻转
uint16 CRC_U16Invert(uint16 chSrc)
{
	int i;
	uint16 det = 0x0000;
	for (i=0;i<16;i++)
	{
		if ((chSrc&(1<<i)) == 0)
		{
		} 
		else
		{
			det |= 1<<(15-i);
		}
	}
	return det;
}

//	32位双字翻转
uint32  CRC_U32Invert(uint32 chSrc)
{
	int i;
	uint32 det = 0x00000000;
	for (i=0;i<32;i++)
	{
		if ((chSrc&(1<<i)) == 0)
		{
		} 
		else
		{
			det |= 1<<(31-i);
		}
	}
	return det;
}