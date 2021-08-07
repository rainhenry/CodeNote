
/************************************************************************

	�������ƣ��ļ�����Ի�������
	�������ڣ�2016.12.14
	��    ����0.2
	��Ʊ��룺rainhenry

	�汾�޶���
		0.2�汾		���Ӷ�UNICODE�Ͷ��ֽ��ַ����ļ���֧��

	����������
	1��ʵ��ͼ�ν�����ļ������������·���ַ���
	2��ʵ��ͼ�ν�����ļ�·�������������·���ַ���

	ʹ�ð��� #include "CBrowseDlg.hpp"

************************************************************************/

//////////////////////////////////////////////////////////////////////////	
//	�ض��屣��
#ifndef __CBROWSEDLG_H__
#define __CBROWSEDLG_H__

//////////////////////////////////////////////////////////////////////////
//	����ͷ�ļ�
#include <iostream>

//////////////////////////////////////////////////////////////////////////
//	���������ռ�
namespace CBrowseDlg
{


//	����ļ���������
CString FileBrowse()
{
	CString str;
	CFileDialog fileDlg(TRUE);
#ifdef UNICODE
	fileDlg.m_ofn.lpstrTitle=L"�ļ��򿪶Ի���";
	fileDlg.m_ofn.lpstrFilter=L"All Files(*.*)\0*.*\0\0"; 
#else
	fileDlg.m_ofn.lpstrTitle="�ļ��򿪶Ի���";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0"; 
#endif
	if(IDOK==fileDlg.DoModal())
	{ 
		CFile file(fileDlg.GetFileName(),CFile::modeRead);
		str=fileDlg.GetPathName();//�ļ���+��׺ 
	}
	return str;
}

//	·�������������
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
	bi.lpszTitle = L"ѡ���ļ���Ŀ¼";
#else
	bi.lpszTitle = "ѡ���ļ���Ŀ¼";
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

//	�����ռ����
};		//	namespace CBrowseDlg
//////////////////////////////////////////////////////////////////////////
#endif	//	__CBROWSEDLG_H__

