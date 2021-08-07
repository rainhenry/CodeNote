
/************************************************************************

	工程名称：文件浏览对话框程序包
	工程日期：2016.12.14
	版    本：0.2
	设计编码：rainhenry

	版本修订：
		0.2版本		增加对UNICODE和多字节字符集的兼容支持

	功能描述：
	1、实现图形界面的文件浏览，并返回路径字符串
	2、实现图形界面的文件路径浏览，并返回路径字符串

	使用包含 #include "CBrowseDlg.hpp"

************************************************************************/

//////////////////////////////////////////////////////////////////////////	
//	重定义保护
#ifndef __CBROWSEDLG_H__
#define __CBROWSEDLG_H__

//////////////////////////////////////////////////////////////////////////
//	包含头文件
#include <iostream>

//////////////////////////////////////////////////////////////////////////
//	定义命名空间
namespace CBrowseDlg
{


//	浏览文件函数定义
CString FileBrowse()
{
	CString str;
	CFileDialog fileDlg(TRUE);
#ifdef UNICODE
	fileDlg.m_ofn.lpstrTitle=L"文件打开对话框";
	fileDlg.m_ofn.lpstrFilter=L"All Files(*.*)\0*.*\0\0"; 
#else
	fileDlg.m_ofn.lpstrTitle="文件打开对话框";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0"; 
#endif
	if(IDOK==fileDlg.DoModal())
	{ 
		CFile file(fileDlg.GetFileName(),CFile::modeRead);
		str=fileDlg.GetPathName();//文件名+后缀 
	}
	return str;
}

//	路径浏览函数定义
CString PathBrowse()
{
	BROWSEINFO bi;
#ifdef UNICODE
	WCHAR name[MAX_PATH];
#else
	char name[MAX_PATH];
#endif
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = name;
#ifdef UNICODE
	bi.lpszTitle = L"选择文件夹目录";
#else
	bi.lpszTitle = "选择文件夹目录";
#endif
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
#ifdef UNICODE
	if(idl == NULL)
		return L"";
#else
	if(idl == NULL)
		return "";
#endif
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
#ifdef UNICODE
	if(strDirectoryPath.IsEmpty())
		return L"";
	if(strDirectoryPath.Right(1)!=L"\\")
		strDirectoryPath+=L"\\";
#else
	if(strDirectoryPath.IsEmpty())
		return "";
	if(strDirectoryPath.Right(1)!="\\")
		strDirectoryPath+="\\";
#endif

	return strDirectoryPath;
}

//	命名空间结束
};		//	namespace CBrowseDlg
//////////////////////////////////////////////////////////////////////////
#endif	//	__CBROWSEDLG_H__

