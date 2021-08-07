/*************************************************************************

	工程名称：文本信息输出类
	文 件 名：TextInfoOut.h
	版	  本：REV 1.2
	工程日期：2016.08.28
	设    计：rainhenry
	编    码：rainhenry
	调试/校对:rainhenry
	测试环境：VS2010 + Win7 @ MFC
	联系方式：QQ 1444197961

功能介绍：
	通过绑定指定的CEdit控件，可以实现累积文本输出，并可以设定滚动条是否跟随滚动
	
版本修订：
	REV 1.1：		2016.10.21
		对AddStr成员函数增加可变参数功能
	REV 1.2:		2016.12.19
		增加EnableUpdate功能

免责声明：
		本程序仅供学习研究讨论，可以随意转载复制，但不得用于商业用途！转载
	使用时，请保留本注释。

使用举例

//----------------------------------------------------
//	定义实例,全局变量时
CTextInfoOut infoout(&dlg.m_edit);
//	定义实例,类成员时
//	头文件中
CTextInfoOut infoout;
//	源文件中的对话框类构造函数
CXXXDlg::CXXXDlg(CWnd* pParent)
: CDialogEx(CXXXDlg::IDD, pParent),
infoout(&(this->m_edit),true)				//	在此追加文本信息类的初始化
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//......
}
//	构造函数的参数可以是一个，此时需要指定CEdit控件的地址，
//	当为2个参数时，第二个参数为文本是否跟随滚动

//----------------------------------------------------
//	向控件中追加文本
infoout.AddStr("追加文本事例\r\n");

//----------------------------------------------------
//	清空
infoout.Clear();

//----------------------------------------------------
//	更改滚动跟随设置
infoout.SetKeep(false);			//	不跟随

//----------------------------------------------------
//	获取全部字符串缓冲区的内容
CString str = infoout.GetString();

*************************************************************************/

//	预编译保护  //////////////////////////////////////////////////////////
#ifndef __TEXTINFOOUT_H__
#define __TEXTINFOOUT_H__

//	包含头文件  //////////////////////////////////////////////////////////
//#include "stdafx.h"
#include <iostream>
#include <string>

//	宏定义  ///////////////////////////////////////////////////////////////
#define DEFAULT_KEEP_FLAG						true				//	默认清空下是否跟随滚动
#define INVALID_PCEDIT_ADDRESS					((CEdit*)(-1))		//	不正确的CEdit控件地址

//	类声明  //////////////////////////////////////////////////////////////
class CTextInfoOut
{
public:
	//	构造函数
	CTextInfoOut(CEdit*);
	//	构造函数,带有滚动设置参数
	CTextInfoOut(CEdit* ,bool);
	//	析构函数
	~CTextInfoOut();

	//	追加字符串
	void AddStr(CString);
	//	追加字符串，格式化方式
	void AddStr(
		char* pszFormat,
		...);

	//	清空字符串
	void Clear(void);

	//	获取全部字符串
	CString GetString(void);

	//	更改滚动转态
	void SetKeep(bool);

	//	使能禁止更新
	void EnableUpdate(bool flag = true);

private:
	//	指向控件的指针
	CEdit* pCEdit;
	//	字符串缓存
	CString str_buffer;
	//	滚动标志
	bool keepflag;
	//	更新到控件标志,true=更新，false=不更新
	bool updateflag;
};


//////////////////////////////////////////////////////////////////////////
#endif	//	__TEXTINFOOUT_H__
