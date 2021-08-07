/*************************************************************************

	工程名称：文本信息输出类
	文 件 名：TextInfoOut.cpp
	版	  本：REV 1.0
	工程日期：2016.08.28
	设    计：rainhenry
	编    码：rainhenry
	调试/校对:rainhenry
	测试环境：VS2010 + Win7 @ MFC
	联系方式：QQ 1444197961

功能介绍：
	通过绑定指定的CEdit控件，可以实现累积文本输出，并可以设定滚动条是否跟随滚动

免责声明：
		本程序仅供学习研究讨论，可以随意转载复制，但不得用于商业用途！转载
	使用时，请保留本注释。


*详细使用说明请参加.h文件
*************************************************************************/

//	包含头文件  //////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TextInfoOut.h"

//////////////////////////////////////////////////////////////////////////
//	构造函数
CTextInfoOut::CTextInfoOut(CEdit* pc)
{
	updateflag = true;									//	默认为更新的
	this->keepflag = DEFAULT_KEEP_FLAG;					//	设定默认的滚动转态
	this->pCEdit = pc;									//	设定控件指针
	this->str_buffer = "";								//	清空字符串
}

//	构造函数,带有滚动设置参数
CTextInfoOut::CTextInfoOut(CEdit* pc ,bool f)
{
	updateflag = true;									//	默认为更新的
	this->keepflag = f;									//	设定默认的滚动转态
	this->pCEdit = pc;									//	设定控件指针
	this->str_buffer = "";								//	清空字符串
}

//	析构函数
CTextInfoOut::~CTextInfoOut()
{

}

//	追加字符串
void CTextInfoOut::AddStr(CString in_str)
{
	//	追加
	this->str_buffer += in_str;

	//	检查是否需要更新,不需要更新的额时候直接跳过更新代码
	if (updateflag == false)
	{
		return ;
	}

	//	当控件指针可用的时候
	if (!((this->pCEdit == INVALID_PCEDIT_ADDRESS)||(this->pCEdit == NULL)))
	{
		//	输出字符串
		this->pCEdit->SetWindowText(this->str_buffer);

		//	控制滚动条
		if (this->keepflag == true)
		{
			this->pCEdit->LineScroll(this->pCEdit->GetLineCount());	//	滚动条指向最后一行
		}
	}
}
//	追加字符串，格式化方式
void CTextInfoOut::AddStr(
	char* pszFormat,
	...)
{
	CString tmp;
	ATLASSERT( AtlIsValidString( pszFormat ) );
	va_list argList;
	va_start( argList, pszFormat );
	tmp.FormatV(pszFormat, argList);
	va_end( argList );

	//	追加
	this->str_buffer += tmp;

	//	检查是否需要更新,不需要更新的额时候直接跳过更新代码
	if (updateflag == false)
	{
		return ;
	}

	//	当控件指针可用的时候
	if (!((this->pCEdit == INVALID_PCEDIT_ADDRESS)||(this->pCEdit == NULL)))
	{
		//	输出字符串
		this->pCEdit->SetWindowText(this->str_buffer);

		//	控制滚动条
		if (this->keepflag == true)
		{
			this->pCEdit->LineScroll(this->pCEdit->GetLineCount());	//	滚动条指向最后一行
		}
	}
}

//	清空字符串
void CTextInfoOut::Clear(void)
{
	this->str_buffer = "";
}

//	获取全部字符串
CString CTextInfoOut::GetString(void)
{
	return this->str_buffer;
}

//	更改滚动转态
void CTextInfoOut::SetKeep(bool f)
{
	this->keepflag = f;
}


//	使能禁止更新
void CTextInfoOut::EnableUpdate(bool flag)
{
	updateflag = flag;
}

