/*

ԭ�ĳ�����http://www.cnblogs.com/nktblog/articles/2770706.html
posted on 2012-11-14 22:28 �������� 

�������裺

1.����һ�����ڶԻ����Ӧ�ó���ADODatabase��

2.����һ���������ӵ�ADO��⡣ϵͳ�˵���Insert->New Class�class typeѡ��Generic Class; ��Name������������ADOConn������OK���������ڹ������Զ�����ADOConn.h��ADOConn.cpp�����ļ���

3.��ADOConn.h�е���һ��ADO��̬���ӿ�msado15.dll����Ӵ��룺#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")��ͬʱ��ADOConn.h�ж��� public��//���һ��ָ��Connection�����ָ��: _ConnectionPtr m_pConnection; //���һ��ָ��Recordset�����ָ��: _RecordsetPtr m_pRecordset��

4.m_pConnection->Open(_bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options) ������ConnectionStringΪVC++�ж�SQL SERVER��ADO���á��磬Provider=SQLOLEDB;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=suntest;Data Source=WWW-ED1F301E78B ;User Id=sa;Password=;

����Data SourceΪ���������ƣ�������Server���棬������ʾΪ.����(local)��Initial CatalogΪ��Ҫʹ�õ����ݿ����� ������Database���棻Integrated Security=SSPI��ʾ�ü���ģʽ����ʱuid ��pwd ���ò������ã�ֻ�е�Integrated Security=FALSE��ʾ�û��ģʽ��uid ��pwd ���ò�������

����Options��ʾCommandText�����ͣ�adCmdText-�ı����adCmdTable-���� adCmdProc-�洢���̣�adCmdUnknown-δ֪

5.���������Ա������(1)ִ�в�ѯ���� _RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);(2)ִ��SQL��� BOOL ExecuteSQL(_bstr_t bstrSQL)��

���������ļ����������룺


*/


/*------------------ADOConn.h File-------------------------------------------------*/

// ADOConn.h: interface for the ADOConn class.

#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

#if !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)

#define AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000


#include <iostream>
#include <string>

class ADOConn

{

// �������

public:

//���һ��ָ��Connection�����ָ��:

_ConnectionPtr m_pConnection;

//���һ��ָ��Recordset�����ָ��:

_RecordsetPtr m_pRecordset;


// ���巽��

public:

ADOConn();

virtual ~ADOConn();

// ��ʼ�����������ݿ�

void OnInitADOConn(std::string conn_str);		//	�����ַ������� "Provider=SQLOLEDB; Server=CZ;Database=HrSys; uid=sa; pwd=sa;";

// ִ�в�ѯ

_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

// ִ��SQL��䣬Insert Update _variant_t

BOOL ExecuteSQL(_bstr_t bstrSQL);

void ExitConnect();

protected:
//	�����ַ����洢
	std::string ConnStr;

};

#endif // !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)
