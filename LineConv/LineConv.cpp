/*************************************************************************

	工程名称：换行类型转换
	文 件 名：LineConv.cpp
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
	

*详细使用说明请参加.h文件
*************************************************************************/


//	包含头文件  //////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LineConv.h"

//	实体定义  ////////////////////////////////////////////////////////////
//	构造函数
CLineConv::CLineConv()
{

}

//	析构函数
CLineConv::~CLineConv()
{

}

//	转换函数
std::string CLineConv::Conv(std::string str ,CLineConv::TargetFormat tf)
{
	//	根据目标形式做分别处理
	switch (tf)
	{
	//	目标为"\r\n"
	case CLineConv::TF_RN:
		return CLineConv::ConvToRN(str);
		//	目标为"\n"
	case CLineConv::TF_N:
		return CLineConv::ConvToN(str);
	//	其他形式原封返回
	default:	
		return str;
	}
}

//	转换为"\r\n"
std::string CLineConv::ConvToRN(std::string str)
{
	//	字符串大于2个字符
	if (str.length() >= 2)
	{
		int i = 0;
		for (i=0;str.begin()+i!=str.end()-1;i++)
		{
			//	类型匹配"\r\n"
			if ((*(str.begin()+i) == '\r')&&(*((str.begin()+i)+1) == '\n'))
			{
				//	与目标匹配，无序转换
			}

			//	不在初始位置
			if (i!=0)
			{
				//	匹配"\n"
				if ((*((str.begin()+i)-1) != '\r')&&(*(str.begin()+i) == '\n'))
				{
					str.insert((str.begin()+i),'\r');
					i++;
				}
			}
			//	在初始位置
			else
			{
				if (*(str.begin()+i) == '\n')
				{
					str.insert((str.begin()+i),'\r');
					i++;
				}
			}
		}

		//	检查最后一个字符
		if ((*(str.begin()+i) == '\n')&&(*((str.begin()+i)-1) != '\r'))
		{
			str.insert((str.begin()+i),'\r');
		}

		//	返回结果
		return str;
	} 
	//	字符串为1个字符
	else if(str.length() == 1)
	{
		str.insert(str.begin(),'\r');

		//	返回结果
		return str;
	}
	//	字符串为空
	else
	{
		return "";		//	返回空字符串
	}
}

//	转换为"\n"
std::string CLineConv::ConvToN(std::string str)
{
	//	字符串不为空
	if (str != "")
	{
		int i=0;
		for (i=0;(str.begin()+i)!=str.end();i++)
		{
			//	发现"\r"
			if (*(str.begin()+i) == '\r')
			{
				str.erase((str.begin()+i));
			}

			//	超出范围
			if ((str.begin()+i) == str.end())
			{
				break;
			}
		}

		//	返回
		return str;
	}
	//	字符串为空
	else
	{
		return "";		//	返回空字符串
	}
}

