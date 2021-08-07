/************************************************************************/
/*	CRC库头文件                                                         */
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

/*************************************************************************
	使用步骤
		1、配置CRC计算设置
		2、调用计算函数CRC_Calc_Func，计算成功将返回CRC_CALC_SUCCESS

**************************************************************************
	使用举例

	//	定义用户数据
	unsigned char dat[] = {'1','2','3','4','5','6','7','8','9'};

	//	定义计算CRC结果的变量
	unsigned int crc_out;

	//	配置CRC计算器
	CRC_Calc_Config cfg;

	//////////////////////////////////////////////////////////////////////////
	//	举例32位 CRC-32 for WinRAR
	//	设置数据宽度为32位
	cfg.data_width = CRC32BIT;							
	//	配置多项式
	cfg.calc_poly.u32_dat = 0;
	//cfg.calc_poly.bit_dat.D32 = 1;		//	此处要忽略此语句
	cfg.calc_poly.bit_dat.D26 = 1;
	cfg.calc_poly.bit_dat.D23 = 1;
	cfg.calc_poly.bit_dat.D22 = 1;
	cfg.calc_poly.bit_dat.D16 = 1;
	cfg.calc_poly.bit_dat.D12 = 1;
	cfg.calc_poly.bit_dat.D11 = 1;
	cfg.calc_poly.bit_dat.D10 = 1;
	cfg.calc_poly.bit_dat.D8 = 1;
	cfg.calc_poly.bit_dat.D7 = 1;
	cfg.calc_poly.bit_dat.D5 = 1;
	cfg.calc_poly.bit_dat.D4 = 1;
	cfg.calc_poly.bit_dat.D2 = 1;
	cfg.calc_poly.bit_dat.D1 = 1;
	cfg.calc_poly.bit_dat.D0 = 1;
	//	设置初值
	cfg.Init = 0xFFFFFFFF;
	//	设置输入翻转
	cfg.RefIn = true;
	//	设置输出翻转
	cfg.RefOut = true;
	//	设置输出异或
	cfg.XorOut = 0xFFFFFFFF;

	//	计算
	if (CRC_Calc_Func(
	dat,
	sizeof(dat),
	&crc_out,
	cfg) == CRC_CALC_SUCCESS)
	{
	printf("CRC32计算成功！结果为：0x%08X\r\n",crc_out);
	}
	else
	{
	printf("CRC32计算失败！");
	}

	//////////////////////////////////////////////////////////////////////////
	//	举例16位 CRC-16/MODBUS
	//	设置数据宽度为16位
	cfg.data_width = CRC16BIT;							
	//	配置多项式
	cfg.calc_poly.u32_dat = 0;
	cfg.calc_poly.bit_dat.D16 = 1;		//	此语句可有可无
	cfg.calc_poly.bit_dat.D15 = 1;
	cfg.calc_poly.bit_dat.D2 = 1;
	cfg.calc_poly.bit_dat.D0 = 1;
	//	设置初值
	cfg.Init = 0xFFFF;
	//	设置输入翻转
	cfg.RefIn = true;
	//	设置输出翻转
	cfg.RefOut = true;
	//	设置输出异或
	cfg.XorOut = 0x0000;

	//	计算
	if (CRC_Calc_Func(
	dat,
	sizeof(dat),
	&crc_out,
	cfg) == CRC_CALC_SUCCESS)
	{
	printf("CRC16计算成功！结果为：0x%04X\r\n",crc_out);
	}
	else
	{
	printf("CRC16计算失败！");
	}

	//////////////////////////////////////////////////////////////////////////
	//	举例8位	CRC-8/ROHC
	//	设置数据宽度为8位
	cfg.data_width = CRC8BIT;							
	//	配置多项式
	cfg.calc_poly.u32_dat = 0;
	cfg.calc_poly.bit_dat.D8 = 1;		//	此语句可有可无
	cfg.calc_poly.bit_dat.D2 = 1;
	cfg.calc_poly.bit_dat.D1 = 1;
	cfg.calc_poly.bit_dat.D0 = 1;
	//	设置初值
	cfg.Init = 0xFF;
	//	设置输入翻转
	cfg.RefIn = true;
	//	设置输出翻转
	cfg.RefOut = true;
	//	设置输出异或
	cfg.XorOut = 0x00;

	//	计算
	if (CRC_Calc_Func(
	dat,
	sizeof(dat),
	&crc_out,
	cfg) == CRC_CALC_SUCCESS)
	{
	printf("CRC8计算成功！结果为：0x%02X\r\n",crc_out);
	}
	else
	{
	printf("CRC8计算失败！");
	}
*************************************************************************/

#ifndef __CRCLIB_H__
#define __CRCLIB_H__

//和处理器相关的设置//////////////////////////////////////////////////////
#define		POSSEQ									//	采用正序排序，取消该定义为逆序排序

#ifndef uint32
	typedef unsigned int uint32;					//	在32位处理器中
	//	typedef unsigned long int uint32;			//	在16位处理器或者8位处理器中
#endif

#ifndef uint16
	typedef unsigned short int uint16;				//	在32位处理器中
	//	typedef unsigned int uint16;				//	在16位处理器或者8位处理器中
#endif

#ifndef uint8
	typedef unsigned char uint8;
#endif 

#ifndef __cplusplus
	typedef unsigned char bool;						//	在C语言中定义布尔类型
#endif

//标准库兼容配置//////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

#ifndef INVALID_ADDRESS
#define INVALID_ADDRESS ((void*)(-1))
#endif

//返回值相关定义//////////////////////////////////////////////////////////
//	返回值定义
#define CRC_CALC_SUCCESS	0						//	计算成功
#define CHECK_OK			0						//	参数正常
#define POLY_FAIL			1						//	非法多项式
#define DATA_WIDTH_FAIL		2						//	非法数据宽度
#define IN_ERROR_ADDRESS	3						//	输入参数指针为错误地址
#define OUT_ERROR_ADDRESS	4						//	输出参数指针为错误地址

//类型定义////////////////////////////////////////////////////////////////
//	CRC数据宽度枚举定义
enum DataWidth {CRC8BIT = 8,CRC16BIT = 16,CRC32BIT = 32};

//	多项式位域结构体
typedef struct tagPolyBit
{
#ifdef POSSEQ
	uint8 D0:1;
	uint8 D1:1;
	uint8 D2:1;
	uint8 D3:1;
	uint8 D4:1;
	uint8 D5:1;
	uint8 D6:1;
	uint8 D7:1;
	uint8 D8:1;
	uint8 D9:1;
	uint8 D10:1;
	uint8 D11:1;
	uint8 D12:1;
	uint8 D13:1;
	uint8 D14:1;
	uint8 D15:1;
	uint8 D16:1;
	uint8 D17:1;
	uint8 D18:1;
	uint8 D19:1;
	uint8 D20:1;
	uint8 D21:1;
	uint8 D22:1;
	uint8 D23:1;
	uint8 D24:1;
	uint8 D25:1;
	uint8 D26:1;
	uint8 D27:1;
	uint8 D28:1;
	uint8 D29:1;
	uint8 D30:1;
	uint8 D31:1;
#else	//	not POSSEQ
	uint8 D31:1;
	uint8 D30:1;
	uint8 D29:1;
	uint8 D28:1;
	uint8 D27:1;
	uint8 D26:1;
	uint8 D25:1;
	uint8 D24:1;
	uint8 D23:1;
	uint8 D22:1;
	uint8 D21:1;
	uint8 D20:1;
	uint8 D19:1;
	uint8 D18:1;
	uint8 D17:1;
	uint8 D16:1;
	uint8 D15:1;
	uint8 D14:1;
	uint8 D13:1;
	uint8 D12:1;
	uint8 D11:1;
	uint8 D10:1;
	uint8 D9:1;
	uint8 D8:1;
	uint8 D7:1;
	uint8 D6:1;
	uint8 D5:1;
	uint8 D4:1;
	uint8 D3:1;
	uint8 D2:1;
	uint8 D1:1;
	uint8 D0:1;
#endif	//	POSSEQ
}PolyBit;

//	多项式共用体
typedef union tagPolyUnion
{
	PolyBit bit_dat;
	uint32 u32_dat;
}PolyUnion;

//	CRC计算参数结构体
typedef struct tagCRC_Calc_Config 
{
	DataWidth data_width;			//	CRC的数据宽度
	PolyUnion calc_poly;			//	CRC计算的多项式
	uint32 Init;					//	初始值
	bool RefIn;						//	输入是否翻转
	bool RefOut;					//	输出是否翻转
	uint32 XorOut;					//	异或输出值
}CRC_Calc_Config;

//	C兼容
#ifdef __cplusplus
extern "C"
{
#endif
//导出用户使用的函数//////////////////////////////////////////////////////
	//	CRC计算函数
	//	返回CRC计算是否成功的代码
	int CRC_Calc_Func(
		void* dat,						//	输入数据的首地址
		uint32 length,					//	输入数据的字节个数
		uint32* outdata,				//	输出数据的存放地址
		CRC_Calc_Config cfg				//	计算配置
		);

//保留的内部子函数////////////////////////////////////////////////////////
	//	配置参数检查
	int CheckCRCConfig(CRC_Calc_Config cfg);

	//	8位字节翻转
	uint8 CRC_U8Invert(uint8 chSrc);

	//	16位单字翻转
	uint16 CRC_U16Invert(uint16 chSrc);

	//	32位双字翻转
	uint32  CRC_U32Invert(uint32 chSrc);

//	C兼容
#ifdef __cplusplus
};		//	extern "C"
#endif

#endif //	__CRCLIB_H__
