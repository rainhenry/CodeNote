
/************************************************************************

	�������ƣ��ļ�����Ի�������
	�������ڣ�2016.12.14
	��    ����0.3
	��Ʊ��룺rainhenry

	�汾�޶���
		0.2�汾		���Ӷ�UNICODE�Ͷ��ֽ��ַ����ļ���֧��
		0.3�汾		���ӱ����ļ��Ի���ť
					�����ڿ���̨�����а�����ͷ�ļ�
					���ĺ�������Ӧ�ľ�������
					���ӵõ�·���ַ����Ӻ���

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
//#define NO_WIN32_LEAN_AND_MEAN // �� Windows ͷ���ų�����ʹ�õ�����
#include <shlobj.h>
#include <iostream>
#include <afx.h>
#include <afxwin.h>

//////////////////////////////////////////////////////////////////////////
//	���������ռ�
namespace CBrowseDlg
{


//	����ļ��������壬δѡ��ʱ����""
CString FileBrowseDlg()
{
	CString str;
	CFileDialog fileDlg(TRUE);
#ifdef UNICODE
	//fileDlg.m_ofn.lpstrTitle=L"�ļ��򿪶Ի���";
	fileDlg.m_ofn.lpstrFilter=L"All Files(*.*)\0*.*\0\0"; 
#else
	//fileDlg.m_ofn.lpstrTitle="�ļ��򿪶Ի���";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0"; 
#endif
	if(IDOK==fileDlg.DoModal())
	{ 
		//CFile file(fileDlg.GetFileName(),CFile::modeRead);
		str=fileDlg.GetPathName();//�ļ���+��׺ 
	}
	return str;
}

//�����ļ�  ��δѡ��ʱ����""
CString SaveFileDlg(CString default_open_path = _T(""),CString default_open_file = _T(""))
{  
	BOOL isOpen = FALSE;        //�Ƿ��(����Ϊ����)  
	CString defaultDir = default_open_path;   //Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = default_open_file;         //Ĭ�ϴ򿪵��ļ���  
	//CString filter = L"�ļ� (*.doc; *.ppt; *.xls)|*.doc;*.ppt;*.xls||";   //�ļ����ǵ�����  
	CString filter = L"�����ļ� (*.*)|*.*||";   //�ļ����ǵ�����  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);  
	openFileDlg.GetOFN().lpstrInitialDir = default_open_path;  
	INT_PTR result = openFileDlg.DoModal();  
	CString filePath = "";  
	if(result == IDOK) {  
		filePath = openFileDlg.GetPathName();  
	}  
	return filePath;
}  

//	·������������壬δѡ��ʱ����""��ѡ��ʱ·����"\\"��β
CString PathBrowseDlg()
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
	//bi.lpszTitle = L"ѡ���ļ���Ŀ¼";
#else
	//bi.lpszTitle = "ѡ���ļ���Ŀ¼";
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

//	ͨ���������ļ�·���������ļ������ڣ�����ȡ��·�����ַ���
CString GetPathString(CString in_str)
{
	//	�������Ϊ�գ�����ҲΪ��
	if (in_str == _T(""))
	{
		return _T("");
	}

	//	�������"\\"����ͬʱɾ������"\\"���ַ�
	for(int i=in_str.GetLength();i>=0;--i)
	{
		if (in_str.GetAt(i) != '\\')
		{
			in_str.Delete(i);
		}
		else
		{
			break;
		}
	}

	return in_str;
}

//	�����ռ����
};		//	namespace CBrowseDlg
//////////////////////////////////////////////////////////////////////////
#endif	//	__CBROWSEDLG_H__

