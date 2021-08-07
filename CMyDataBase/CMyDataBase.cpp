
#include "stdafx.h"
#include "CMyDataBase.h"
#include <iostream>
#include <string>

//	构造函数
CMyDatabase::CMyDatabase()
{
	//	设置当前为未连接
	ConnectionState = false;

	//	设置记录集为未打开
	RecordsetPtrState = false;

	//	设置非法的当前记录位置
	mRecordPos = -1;
}

//	构造函数
CMyDatabase::CMyDatabase(CString cmd_str)								//	行为同OpenEx
{
	//	设置当前为未连接
	ConnectionState = false;

	//	设置记录集为未打开
	RecordsetPtrState = false;

	//	设置非法的当前记录位置
	mRecordPos = -1;

	//	连接
	if (!OpenEx(cmd_str))
	{
		AfxMessageBox("构造数据库类时，打开数据库失败！！");
	}	
}

//	构造函数
CMyDatabase::CMyDatabase(
	CString host,				//	主机地址
	CString database_name,		//	数据库名称
	CString user,				//	用户名
	CString pwd					//	密码
	)		//	行为同Open
{
	//	设置当前为未连接
	ConnectionState = false;

	//	设置记录集为未打开
	RecordsetPtrState = false;

	//	设置非法的当前记录位置
	mRecordPos = -1;

	//	连接
	if (!Open(
		host,
		database_name,
		user,
		pwd
		))
	{
		AfxMessageBox("构造数据库类时，打开数据库失败！！");
	}		
}

//	析构函数
CMyDatabase::~CMyDatabase()
{
	Close();
}

//	打开数据库连接，自定义打开命令， true=成功   false=失败
bool CMyDatabase::OpenEx(CString cmd_str)
{
	//	如果当前为已经连接
	if (ConnectionState == true)
	{
		//	断开已有连接
		Close();
		Sleep(100);
	}

	//	初始化COM
	CoInitialize(NULL);

	//	尝试连接
	try
	{
		//	创建连接
		p_ConnectionPtr.CreateInstance("ADODB.Connection");

		//	打开数据库
		std::string tmp_str = cmd_str;
		p_ConnectionPtr->Open(
			tmp_str.data(),
			"",
			"",
			adModeUnknown
			);
	}
	catch (_com_error e)
	{
		//	设置为未连接
		ConnectionState = false;
		
		//	输出本次错误消息
		std::string tmp_str = e.Description();
		LastErrorInfoString = tmp_str.data();

		//	析构COM
		CoUninitialize();

		//	返回失败
		return false;
	}

	//	设置为已经连接
	ConnectionState = true;

	//	返回成功
	return true;
}

//	打开数据库连接，指定IP、数据库名、用户名、密码， true=成功   false=失败
bool CMyDatabase::Open(
	CString host,				//	主机地址
	CString database_name,		//	数据库名称
	CString user,				//	用户名
	CString pwd					//	密码
	)
{
	//	创建连接命令字符串
	CString cmd_str = 
		"Provider=SQLOLEDB; Server=" + host + 
		";Database=" + database_name + 
		"; uid=" + user +
		"; pwd=" + pwd +";";

	//	连接，并返回连接成功状态
	return OpenEx(cmd_str);
}

//	关闭数据库
void CMyDatabase::Close(void)
{
	//	如果当前记录集为打开
	if ((RecordsetPtrState == true)&&(p_RecordsetPtr->GetState() != 0))
	{
		//	关闭记录集
		p_RecordsetPtr->Close();

		//	设置记录集为未打开
		RecordsetPtrState = false;

		//	设置非法的当前记录位置
		mRecordPos = -1;
	}

	//	如果当前为已经连接
	if (ConnectionState == true)
	{
		//	断开已有连接
		p_ConnectionPtr->Close();

		//	析构COM
		CoUninitialize();

		//	设置当前为未连接
		ConnectionState = false;
	}
}


//	执行SQL语句,   true=成功   false=失败
bool CMyDatabase::ExeSQL(
	CString cmd
	)
{
	//	判读连接是否成功,当未连接的时候不允许执行SQL语句
	if (ConnectionState == false)
	{
		return false;
	}

	//	尝试执行SQL
	try
	{
		//	如果记录集已经打开，关闭它
		if (RecordsetPtrState == true)
		{
			p_RecordsetPtr->Close();
		}

		//	创建记录集
		p_RecordsetPtr.CreateInstance(__uuidof(Recordset));

		//	执行SQL
		std::string tmp_str = cmd;
		p_RecordsetPtr->Open(
			tmp_str.data(),
			p_ConnectionPtr.GetInterfacePtr(),
			adOpenStatic,//adOpenDynamic,
			adLockOptimistic,
			adCmdText);

		//	判断记录集是否创建成功，如果成功
		if (p_RecordsetPtr->GetState() != 0)
		{
			//	记录集已经创建
			RecordsetPtrState = true;
		}
		//	如果不成功
		else
		{
			//	记录集创建失败
			RecordsetPtrState = false;
		}


		//	设置当前为首位置
		mRecordPos = 0L;

		//	返回成功
		return true;
	}
	catch (_com_error e)
	{
		//	设置记录集为关闭
		RecordsetPtrState = false;

		//	输出本次错误消息
		std::string tmp_str = e.Description();
		LastErrorInfoString = tmp_str.data();

		//	返回失败
		return false;
	}
}



//	获取记录集列总数，-1表示获取失败，其他值表示获取结果
long CMyDatabase::GetFieldsCount(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "获取记录集列总数失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return -1;
	}

	return p_RecordsetPtr->GetFields()->GetCount();
}

//	获取记录集列的名字，正常操作时，函数将返回列名，并且若指定re的话，将返回true，表示成功。否则函数将返回""，当指定re时，将输出false
//	并且未指定re时，错误消息将通过AfxMessageBox形式给出。其中index为输入的列序号，0基序。
CString CMyDatabase::GetFieldsName(long index, bool* re)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		CString tmp_str;
		tmp_str.Format("获取记录集列名称失败！可能是连接未初始化，或记录集未创建！");
		LastErrorInfoString = tmp_str;

		//	返回失败，当指定re时
		if (re != NULL)
		{
			//	设置失败
			*re = false;

			//	返回空字符串
			return "";
		} 
		//	当未指定re时
		else
		{
			//	弹出错误提示
			AfxMessageBox(LastErrorInfoString);

			//	返回空字符串
			return "";
		}
	}

	//	检查index值
	if (GetFieldsCount() <= index)
	{
		//	设置错误信息
		CString tmp_str;
		tmp_str.Format("获取记录集列名称失败！非法的列index值%ld！",index);
		LastErrorInfoString = tmp_str;

		//	返回失败，当指定re时
		if (re != NULL)
		{
			//	设置失败
			*re = false;

			//	返回空字符串
			return "";
		} 
		//	当未指定re时
		else
		{
			//	弹出错误提示
			AfxMessageBox(LastErrorInfoString);

			//	返回空字符串
			return "";
		}
	}

	//	尝试获取名字
	std::string re_str;
	try
	{
		//	获取名字
		re_str = p_RecordsetPtr->GetFields()->GetItem(index)->GetName();

		//	给出状态
		if (re != NULL)
		{
			//	设置成功
			*re = true;
		}

		//	返回名字
		return re_str.data();
	}
	catch (...)
	{
		//	设置错误信息
		CString tmp_str;
		tmp_str.Format("获取记录集列名称失败！未知错误！");
		LastErrorInfoString = tmp_str;

		//	给出状态
		if (re != NULL)
		{
			//	设置失败
			*re = false;
		}
		else
		{
			//	弹出错误提示
			AfxMessageBox(LastErrorInfoString);
		}

		//	返回空字符串
		return "";
	}
}

//	获得当前记录的总数，-1表示获取失败，其他非负数值表示获取成功
long CMyDatabase::GetRecordCount(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "获取记录集记录总数失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return -1;
	}

	//	返回记录总数
	return p_RecordsetPtr->GetRecordCount();
}


//	获取当前记录的位置，-1表示获取失败，其他非负数值表示获取成功
long CMyDatabase::GetRecordPos(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "获取记录集记录总数失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return -1;
	}

	//	返回当前记录位置
	return mRecordPos;
}


//	将当前记录移动到指定的位置， true=成功     false=失败
bool CMyDatabase::MoveTo(long pos)
{
	//	首先移动到首位置
	MoveFirst();

	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return false;
	}

	//	检查记录集存储的记录数
	if (GetRecordCount() <= 0)
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！当前记录集为空！";

		//	返回失败
		return false;
	}

	//	检查pos是否合法，超过记录总数
	if (GetRecordCount() <= pos)
	{
		//	设置错误信息
		CString str;
		str.Format("移动当前记录失败！错误的index值%ld！",pos);
		LastErrorInfoString = str;

		//	返回失败
		return false;
	}

	//	移动记录
	p_RecordsetPtr->Move(pos);

	//	设置当前位置
	mRecordPos = pos;

	//	返回成功
	return true;
}


//	将当前记录移动到首位置， true=成功     false=失败
bool CMyDatabase::MoveFirst(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return false;
	}

	//	检查记录集存储的记录数
	if (GetRecordCount() <= 0)
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！当前记录集为空！";

		//	返回失败
		return false;
	}

	//	移动到首位置
	p_RecordsetPtr->MoveFirst();

	//	设置当前位置
	mRecordPos = 0L;

	//	返回成功
	return true;
}

//	将当前记录移动到末尾位置， true=成功     false=失败
bool CMyDatabase::MoveLast(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return false;
	}

	//	检查记录集存储的记录数
	if (GetRecordCount() <= 0)
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！当前记录集为空！";

		//	返回失败
		return false;
	}

	//	移动到首位置
	p_RecordsetPtr->MoveLast();

	//	设置当前位置
	mRecordPos = GetRecordCount()-1L;

	//	返回成功
	return true;
}


//	移动到下一个位置， true=成功     false=失败
bool CMyDatabase::MoveNext(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return false;
	}

	//	检查记录集存储的记录数
	if (GetRecordCount() <= 0)
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！当前记录集为空！";

		//	返回失败
		return false;
	}

	//	移动到首位置
	p_RecordsetPtr->MoveNext();

	//	判断当前所移动的操作是否合法，当从末尾溢出时，返回失败，并移动回来
	if (p_RecordsetPtr->adoEOF)
	{
		//	回到刚才位置
		p_RecordsetPtr->MovePrevious();

		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！新的移动将导致记录从末尾溢出！";

		//	返回失败
		return false;
	}

	//	设置当前位置
	++mRecordPos;

	//	返回成功
	return true;
}


//	移动到上一个位置， true=成功     false=失败
bool CMyDatabase::MovePrevious(void)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return false;
	}

	//	检查记录集存储的记录数
	if (GetRecordCount() <= 0)
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！当前记录集为空！";

		//	返回失败
		return false;
	}

	//	移动到首位置
	p_RecordsetPtr->MovePrevious();

	//	判断当前所移动的操作是否合法，当从顶端溢出时，返回失败，并移动回来
	if (p_RecordsetPtr->adoBOF)
	{
		//	回到刚才位置
		p_RecordsetPtr->MoveNext();

		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！新的移动将导致记录从顶端溢出！";

		//	返回失败
		return false;
	}

	//	设置当前位置
	--mRecordPos;

	//	返回成功
	return true;
}


//	得到当前位置的数据
_variant_t CMyDatabase::GetCurrentData(
	long fields_index,					//	列序号
	bool* re							//	返回成功标志
	)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		CString tmp_str;
		tmp_str.Format("获取记录集数据失败！可能是连接未初始化，或记录集未创建！");
		LastErrorInfoString = tmp_str;

		//	返回失败，当指定re时
		if (re != NULL)
		{
			//	设置失败
			*re = false;

			//	返回空
			return "";
		} 
		//	当未指定re时
		else
		{
			//	弹出错误提示
			AfxMessageBox(LastErrorInfoString);

			//	返回空
			return "";
		}
	}

	//	检查fields_index是否合法
	if (GetFieldsCount() <= fields_index)
	{
		//	设置错误信息
		CString tmp_str;
		tmp_str.Format("获取记录集数据失败！非法的列index值%ld！",fields_index);
		LastErrorInfoString = tmp_str;

		//	返回失败，当指定re时
		if (re != NULL)
		{
			//	设置失败
			*re = false;

			//	返回空
			return "";
		} 
		//	当未指定re时
		else
		{
			//	弹出错误提示
			AfxMessageBox(LastErrorInfoString);

			//	返回空
			return "";
		}
	}

	//	尝试获取数据
	_variant_t re_data;
	try
	{
		//	获取数据
		re_data = p_RecordsetPtr->GetFields()->GetItem(fields_index)->GetValue();

		//	给出状态
		if (re != NULL)
		{
			//	设置成功
			*re = true;
		}

		//	返回数据
		return re_data;
	}
	catch (...)
	{
		//	设置错误信息
		CString tmp_str;
		tmp_str.Format("获取记录数据失败！未知错误！");
		LastErrorInfoString = tmp_str;

		//	给出状态
		if (re != NULL)
		{
			//	设置失败
			*re = false;
		}
		else
		{
			//	弹出错误提示
			AfxMessageBox(LastErrorInfoString);
		}

		//	返回空
		return "";
	}
}


//	数据转换为字符串
CString CMyDatabase::DataToCString(_variant_t var)
{
	return (LPCSTR)_bstr_t(var);
}

//	得到当前位置数据的字符串
CString CMyDatabase::GetCurrentDataString(
	long fields_index,					//	列序号
	bool* re							//	返回成功标志
	)
{
	return DataToCString(GetCurrentData(fields_index,re));
}


//	得到指定位置的数据
_variant_t CMyDatabase::GetData(
	long pos,							//	记录集相对于起始的位置
	long fields_index,					//	列序号
	bool* re							//	返回成功标志
	)
{
	//	移动到指定位置
	MoveTo(pos);

	//	返回数据
	return GetCurrentData(fields_index,re);
}

//	得到指定位置数据的字符串
CString CMyDatabase::GetDataString(
	long pos,							//	记录集相对于起始的位置
	long fields_index,					//	列序号
	bool* re							//	返回成功标志
	)
{
	//	移动到指定位置
	MoveTo(pos);

	//	返回数据
	return GetCurrentDataString(fields_index,re);
}


//	将当前记录进行相对位置移动，此时代入数值为正为向末尾位置方向移动，为负为向首位置方向移动， true=成功     false=失败
bool CMyDatabase::Move(long pos)
{
	//	检查记录集是否已经创建，并且连接已经建立，有任何一个有差错将退出
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！可能是连接未初始化，或记录集未创建！";

		//	返回失败
		return false;
	}

	//	检查记录集存储的记录数
	if (GetRecordCount() <= 0)
	{
		//	设置错误信息
		LastErrorInfoString = "移动当前记录失败！当前记录集为空！";

		//	返回失败
		return false;
	}

	//	检查pos是否合法，小于0时，不可移动超出顶端，大于0，不能超过记录集容量
	if (!((0<=(GetRecordPos() + pos))&&((GetRecordPos() + pos)<GetRecordCount())))
	{
		//	设置错误信息
		CString str;
		str.Format("移动当前记录失败！相对移动位置移除！错误的index值%ld！",pos);
		LastErrorInfoString = str;

		//	返回失败
		return false;
	}

	//	移动记录
	p_RecordsetPtr->Move(pos);

	//	设置当前位置
	mRecordPos = mRecordPos+pos;

	//	返回成功
	return true;
}