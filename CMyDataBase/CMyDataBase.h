/************************************************************************

	工程名称：自定义数据库类
	工程版本：0.1
	工程日期：2016.12.17
	平    台：VS2010+SQL Server 2008 @ Win7 64bit SP1
	设计编码：rainhenry

	功能描述
	1、支持SQL Server数据库（其他数据库未测试）
	2、对基本功能的封装

	使用举例
//////////////////////////////////////////////////////////////////////////
	//	连接数据库
	CMyDatabase mCMyDatabase;
	bool re = mCMyDatabase.Open(
		"127.0.0.1",
		"DATABASE_TEST1",
		"sa",
		"nbl287"
		);
	if (re)
	{
		infoout.AddStr("数据库连接成功！\r\n");
	} 
	else
	{
		infoout.AddStr("数据库连接失败！\r\n    错误信息：" + mCMyDatabase.LastErrorInfoString + "\r\n");
	}
	
//////////////////////////////////////////////////////////////////////////
	//	执行查询,并遍历输出所有结果
	re = mCMyDatabase.ExeSQL("select * from DATABASE_TEST1.dbo.TABLES_TEST1;");
	if (re)
	{
		infoout.AddStr("执行查询成功！\r\n");
		long count = mCMyDatabase.GetFieldsCount();
		infoout.AddStr("    获得%ld个列\r\n",count);
	} 
	else
	{
		infoout.AddStr("执行查询失败！\r\n,    错误信息：%s\r\n",mCMyDatabase.LastErrorInfoString);
	}

	//	循环输出当前的列名
	for (long i=0;i<mCMyDatabase.GetFieldsCount();++i)
	{
		infoout.AddStr("    %ld列：%s\r\n",i+1,mCMyDatabase.GetFieldsName(i));
	}

	//	输出记录集内的数目
	infoout.AddStr("记录集内包含%ld条数据\r\n",mCMyDatabase.GetRecordCount());

	//	遍历输出记录集内的所有数据
	//	外循环为记录集循环
	for (long i=0;i<mCMyDatabase.GetRecordCount();++i)
	{
		//	内循环为列数目
		for (long j=0;j<mCMyDatabase.GetFieldsCount();++j)
		{
			infoout.AddStr("    %s",mCMyDatabase.GetDataString(i,j));
		}

		//	输出换行
		infoout.AddStr("\r\n");
	}

//////////////////////////////////////////////////////////////////////////
	//	释放资源
	mCMyDatabase.Close();
	infoout.AddStr("数据库断开成功！\r\n");

//////////////////////////////////////////////////////////////////////////
	程序输出：
数据库连接成功！
执行查询成功！
    获得3个列
    1列：ID
    2列：DATETIME
    3列：CAP_STR
记录集内包含2条数据
    1    2016/12/16 1:02:03    213TT
    2    2016/12/17    ABC
数据库断开成功！

///////////////////////////////////////////////////////////////////////////
//	执行插入如下
	//	执行插入
	bool re = mCMyDatabase.ExeSQL("insert into DATABASE_TEST1.dbo.TABLES_TEST1 (DATETIME,CAP_STR) VALUES (GETDATE(),'"+ str +"');");
	if (re)
	{
		infoout.AddStr("执行插入成功！\r\n");
	} 
	else
	{
		infoout.AddStr("执行插入失败！\r\n,    错误信息：%s\r\n",mCMyDatabase.LastErrorInfoString);
	}


************************************************************************/

//////////////////////////////////////////////////////////////////////////
//	重定义保护
#ifndef __CMYDATABASE_H__
#define __CMYDATABASE_H__

//////////////////////////////////////////////////////////////////////////
//	包含头文件与动态链接库
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
#include <iostream>
#include <string>

//////////////////////////////////////////////////////////////////////////
//	类定义
class CMyDatabase
{
private:
	//	数据库连接状态	true=已连接		false=未连接
	bool ConnectionState;

	//	数据库记录集打开状态， true=已经打开     false=未打开
	bool RecordsetPtrState;

	//	记录集的当前记录的位置
	long mRecordPos;

	//	记录集对象
	_RecordsetPtr p_RecordsetPtr;
	
	//	连接对象
	_ConnectionPtr p_ConnectionPtr;

public:
	CMyDatabase();
	CMyDatabase(CString);								//	行为同OpenEx
	CMyDatabase(CString,CString,CString,CString);		//	行为同Open
	~CMyDatabase();

	//	打开数据库连接，自定义打开命令， true=成功   false=失败
	bool OpenEx(CString);

	//	打开数据库连接，指定IP、数据库名、用户名、密码， true=成功   false=失败
	bool Open(
		CString host,				//	主机地址
		CString database_name,		//	数据库名称
		CString user,				//	用户名
		CString pwd					//	密码
		);

	//	关闭数据库
	void Close(void);

	//	执行SQL语句,   true=成功   false=失败
	bool ExeSQL(
		CString cmd
		);

	//	获取记录集列总数，-1表示获取失败，其他值表示获取结果
	long GetFieldsCount(void);

	//	获取记录集列的名字，正常操作时，函数将返回列名，并且若指定re的话，将返回true，表示成功。否则函数将返回""，当指定re时，将输出false
	//	并且未指定re时，错误消息将通过AfxMessageBox形式给出。其中index为输入的列序号，0基序。
	CString GetFieldsName(long index, bool* re = NULL);

	//	获得当前记录的总数，-1表示获取失败，其他非负数值表示获取成功
	long GetRecordCount(void);

	//	获取当前记录的位置，-1表示获取失败，其他非负数值表示获取成功
	long GetRecordPos(void);

	//	将当前记录移动到指定的位置， true=成功     false=失败
	bool MoveTo(long pos);

	//	将当前记录进行相对位置移动，此时代入数值为正为向末尾位置方向移动，为负为向首位置方向移动， true=成功     false=失败
	bool Move(long pos);

	//	将当前记录移动到首位置， true=成功     false=失败
	bool MoveFirst(void);

	//	将当前记录移动到末尾位置， true=成功     false=失败
	bool MoveLast(void);

	//	移动到下一个位置， true=成功     false=失败
	bool MoveNext(void);

	//	移动到上一个位置， true=成功     false=失败
	bool MovePrevious(void);

	//	得到当前位置的数据
	_variant_t GetCurrentData(
		long fields_index,					//	列序号
		bool* re = NULL						//	返回成功标志
		);
	
	//	得到当前位置数据的字符串
	CString GetCurrentDataString(
		long fields_index,					//	列序号
		bool* re = NULL						//	返回成功标志
		);

	//	数据转换为字符串
	static CString DataToCString(_variant_t var);

	//	得到指定位置的数据
	_variant_t GetData(
		long pos,							//	记录集相对于起始的位置
		long fields_index,					//	列序号
		bool* re = NULL						//	返回成功标志
		);

	//	得到指定位置数据的字符串
	CString GetDataString(
		long pos,							//	记录集相对于起始的位置
		long fields_index,					//	列序号
		bool* re = NULL						//	返回成功标志
		);

public:	
	//	最后一次的错误消息
	CString LastErrorInfoString;
};

//////////////////////////////////////////////////////////////////////////
#endif	//	__CMYDATABASE_H__

