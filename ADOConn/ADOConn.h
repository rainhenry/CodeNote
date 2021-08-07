/*

原文出处：http://www.cnblogs.com/nktblog/articles/2770706.html
posted on 2012-11-14 22:28 技术蛀虫 

基本步骤：

1.创建一个基于对话框的应用程序ADODatabase。

2.创建一个用来连接的ADO类库。系统菜单中Insert->New Class项，class type选择Generic Class; 在Name后输入类名“ADOConn”，点OK。这样，在工程上自动生成ADOConn.h和ADOConn.cpp两个文件。

3.在ADOConn.h中导入一个ADO动态链接库msado15.dll，添加代码：#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")，同时在ADOConn.h中定义 public：//添加一个指向Connection对象的指针: _ConnectionPtr m_pConnection; //添加一个指向Recordset对象的指针: _RecordsetPtr m_pRecordset。

4.m_pConnection->Open(_bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options) 操作。ConnectionString为VC++中对SQL SERVER中ADO设置。如，Provider=SQLOLEDB;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=suntest;Data Source=WWW-ED1F301E78B ;User Id=sa;Password=;

其中Data Source为服务器名称，可以用Server代替，本机表示为.或者(local)；Initial Catalog为你要使用的数据库名称 可以用Database代替；Integrated Security=SSPI表示用集成模式，此时uid 和pwd 设置不起作用；只有当Integrated Security=FALSE表示用混合模式，uid 和pwd 设置才起作用

参数Options表示CommandText的类型：adCmdText-文本命令；adCmdTable-表名 adCmdProc-存储过程；adCmdUnknown-未知

5.添加两个成员函数：(1)执行查询函数 _RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);(2)执行SQL语句 BOOL ExecuteSQL(_bstr_t bstrSQL)。

给出两个文件的完整代码：


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

// 定义变量

public:

//添加一个指向Connection对象的指针:

_ConnectionPtr m_pConnection;

//添加一个指向Recordset对象的指针:

_RecordsetPtr m_pRecordset;


// 定义方法

public:

ADOConn();

virtual ~ADOConn();

// 初始化—连接数据库

void OnInitADOConn(std::string conn_str);		//	连接字符串举例 "Provider=SQLOLEDB; Server=CZ;Database=HrSys; uid=sa; pwd=sa;";

// 执行查询

_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

// 执行SQL语句，Insert Update _variant_t

BOOL ExecuteSQL(_bstr_t bstrSQL);

void ExitConnect();

protected:
//	连接字符串存储
	std::string ConnStr;

};

#endif // !defined(AFX_ADOCONN_H__AC448F02_AF26_45E4_9B2D_D7ECB8FFCFB9__INCLUDED_)
