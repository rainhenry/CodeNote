/*************************************************************************

	工程名称：换行类型转换
	文 件 名：LineConv.h
	版	  本：REV 1.0
	工程日期：2016.08.27
	设    计：rainhenry
	编    码：rainhenry
	调试/校对:rainhenry
	测试环境：VS2010 + Win7
	联系方式：QQ 1444197961

功能介绍：
		实现"\r\n"和"\n"之间的转换

免责声明：
		本程序仅供学习研究讨论，可以随意转载复制，但不得用于商业用途！转载
	使用时，请保留本注释。

使用举例

//	要将原字符串统一转换为"\r\n"格式
std::string str = "source string\ntest\r\nhello";
str = CLineConv::Conv(str ,CLineConv::TF_RN);

//	或者统一转换为"\n"格式
str = CLineConv::Conv(str ,CLineConv::TF_N);
	

*************************************************************************/



//  预编译保护  //////////////////////////////////////////////////////////
#ifndef __LINECONV_H__
#define __LINECONV_H__

//  包含头文件  //////////////////////////////////////////////////////////
#include <iostream> 
#include <string>

//  宏定义声明  //////////////////////////////////////////////////////////


//	类定义  //////////////////////////////////////////////////////////////
class CLineConv
{
public:
	//	构造函数
	CLineConv();
	//	析构函数
	~CLineConv();

	//	目标格式定义
	enum TargetFormat 
	{
		TF_RN,						//	"\r\n"
		TF_N,						//	"\n"
	};

	//	转换函数
	static std::string Conv(std::string ,TargetFormat);

private:
	//	转换为"\r\n"
	static std::string ConvToRN(std::string);
	//	转换为"\n"
	static std::string ConvToN(std::string);
};



//////////////////////////////////////////////////////////////////////////
#endif	//	__LINECONV_H__

