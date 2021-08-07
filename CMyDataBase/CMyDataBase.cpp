
#include "stdafx.h"
#include "CMyDataBase.h"
#include <iostream>
#include <string>

//	���캯��
CMyDatabase::CMyDatabase()
{
	//	���õ�ǰΪδ����
	ConnectionState = false;

	//	���ü�¼��Ϊδ��
	RecordsetPtrState = false;

	//	���÷Ƿ��ĵ�ǰ��¼λ��
	mRecordPos = -1;
}

//	���캯��
CMyDatabase::CMyDatabase(CString cmd_str)								//	��ΪͬOpenEx
{
	//	���õ�ǰΪδ����
	ConnectionState = false;

	//	���ü�¼��Ϊδ��
	RecordsetPtrState = false;

	//	���÷Ƿ��ĵ�ǰ��¼λ��
	mRecordPos = -1;

	//	����
	if (!OpenEx(cmd_str))
	{
		AfxMessageBox("�������ݿ���ʱ�������ݿ�ʧ�ܣ���");
	}	
}

//	���캯��
CMyDatabase::CMyDatabase(
	CString host,				//	������ַ
	CString database_name,		//	���ݿ�����
	CString user,				//	�û���
	CString pwd					//	����
	)		//	��ΪͬOpen
{
	//	���õ�ǰΪδ����
	ConnectionState = false;

	//	���ü�¼��Ϊδ��
	RecordsetPtrState = false;

	//	���÷Ƿ��ĵ�ǰ��¼λ��
	mRecordPos = -1;

	//	����
	if (!Open(
		host,
		database_name,
		user,
		pwd
		))
	{
		AfxMessageBox("�������ݿ���ʱ�������ݿ�ʧ�ܣ���");
	}		
}

//	��������
CMyDatabase::~CMyDatabase()
{
	Close();
}

//	�����ݿ����ӣ��Զ������� true=�ɹ�   false=ʧ��
bool CMyDatabase::OpenEx(CString cmd_str)
{
	//	�����ǰΪ�Ѿ�����
	if (ConnectionState == true)
	{
		//	�Ͽ���������
		Close();
		Sleep(100);
	}

	//	��ʼ��COM
	CoInitialize(NULL);

	//	��������
	try
	{
		//	��������
		p_ConnectionPtr.CreateInstance("ADODB.Connection");

		//	�����ݿ�
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
		//	����Ϊδ����
		ConnectionState = false;
		
		//	������δ�����Ϣ
		std::string tmp_str = e.Description();
		LastErrorInfoString = tmp_str.data();

		//	����COM
		CoUninitialize();

		//	����ʧ��
		return false;
	}

	//	����Ϊ�Ѿ�����
	ConnectionState = true;

	//	���سɹ�
	return true;
}

//	�����ݿ����ӣ�ָ��IP�����ݿ������û��������룬 true=�ɹ�   false=ʧ��
bool CMyDatabase::Open(
	CString host,				//	������ַ
	CString database_name,		//	���ݿ�����
	CString user,				//	�û���
	CString pwd					//	����
	)
{
	//	�������������ַ���
	CString cmd_str = 
		"Provider=SQLOLEDB; Server=" + host + 
		";Database=" + database_name + 
		"; uid=" + user +
		"; pwd=" + pwd +";";

	//	���ӣ����������ӳɹ�״̬
	return OpenEx(cmd_str);
}

//	�ر����ݿ�
void CMyDatabase::Close(void)
{
	//	�����ǰ��¼��Ϊ��
	if ((RecordsetPtrState == true)&&(p_RecordsetPtr->GetState() != 0))
	{
		//	�رռ�¼��
		p_RecordsetPtr->Close();

		//	���ü�¼��Ϊδ��
		RecordsetPtrState = false;

		//	���÷Ƿ��ĵ�ǰ��¼λ��
		mRecordPos = -1;
	}

	//	�����ǰΪ�Ѿ�����
	if (ConnectionState == true)
	{
		//	�Ͽ���������
		p_ConnectionPtr->Close();

		//	����COM
		CoUninitialize();

		//	���õ�ǰΪδ����
		ConnectionState = false;
	}
}


//	ִ��SQL���,   true=�ɹ�   false=ʧ��
bool CMyDatabase::ExeSQL(
	CString cmd
	)
{
	//	�ж������Ƿ�ɹ�,��δ���ӵ�ʱ������ִ��SQL���
	if (ConnectionState == false)
	{
		return false;
	}

	//	����ִ��SQL
	try
	{
		//	�����¼���Ѿ��򿪣��ر���
		if (RecordsetPtrState == true)
		{
			p_RecordsetPtr->Close();
		}

		//	������¼��
		p_RecordsetPtr.CreateInstance(__uuidof(Recordset));

		//	ִ��SQL
		std::string tmp_str = cmd;
		p_RecordsetPtr->Open(
			tmp_str.data(),
			p_ConnectionPtr.GetInterfacePtr(),
			adOpenStatic,//adOpenDynamic,
			adLockOptimistic,
			adCmdText);

		//	�жϼ�¼���Ƿ񴴽��ɹ�������ɹ�
		if (p_RecordsetPtr->GetState() != 0)
		{
			//	��¼���Ѿ�����
			RecordsetPtrState = true;
		}
		//	������ɹ�
		else
		{
			//	��¼������ʧ��
			RecordsetPtrState = false;
		}


		//	���õ�ǰΪ��λ��
		mRecordPos = 0L;

		//	���سɹ�
		return true;
	}
	catch (_com_error e)
	{
		//	���ü�¼��Ϊ�ر�
		RecordsetPtrState = false;

		//	������δ�����Ϣ
		std::string tmp_str = e.Description();
		LastErrorInfoString = tmp_str.data();

		//	����ʧ��
		return false;
	}
}



//	��ȡ��¼����������-1��ʾ��ȡʧ�ܣ�����ֵ��ʾ��ȡ���
long CMyDatabase::GetFieldsCount(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "��ȡ��¼��������ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return -1;
	}

	return p_RecordsetPtr->GetFields()->GetCount();
}

//	��ȡ��¼���е����֣���������ʱ������������������������ָ��re�Ļ���������true����ʾ�ɹ���������������""����ָ��reʱ�������false
//	����δָ��reʱ��������Ϣ��ͨ��AfxMessageBox��ʽ����������indexΪ���������ţ�0����
CString CMyDatabase::GetFieldsName(long index, bool* re)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		CString tmp_str;
		tmp_str.Format("��ȡ��¼��������ʧ�ܣ�����������δ��ʼ�������¼��δ������");
		LastErrorInfoString = tmp_str;

		//	����ʧ�ܣ���ָ��reʱ
		if (re != NULL)
		{
			//	����ʧ��
			*re = false;

			//	���ؿ��ַ���
			return "";
		} 
		//	��δָ��reʱ
		else
		{
			//	����������ʾ
			AfxMessageBox(LastErrorInfoString);

			//	���ؿ��ַ���
			return "";
		}
	}

	//	���indexֵ
	if (GetFieldsCount() <= index)
	{
		//	���ô�����Ϣ
		CString tmp_str;
		tmp_str.Format("��ȡ��¼��������ʧ�ܣ��Ƿ�����indexֵ%ld��",index);
		LastErrorInfoString = tmp_str;

		//	����ʧ�ܣ���ָ��reʱ
		if (re != NULL)
		{
			//	����ʧ��
			*re = false;

			//	���ؿ��ַ���
			return "";
		} 
		//	��δָ��reʱ
		else
		{
			//	����������ʾ
			AfxMessageBox(LastErrorInfoString);

			//	���ؿ��ַ���
			return "";
		}
	}

	//	���Ի�ȡ����
	std::string re_str;
	try
	{
		//	��ȡ����
		re_str = p_RecordsetPtr->GetFields()->GetItem(index)->GetName();

		//	����״̬
		if (re != NULL)
		{
			//	���óɹ�
			*re = true;
		}

		//	��������
		return re_str.data();
	}
	catch (...)
	{
		//	���ô�����Ϣ
		CString tmp_str;
		tmp_str.Format("��ȡ��¼��������ʧ�ܣ�δ֪����");
		LastErrorInfoString = tmp_str;

		//	����״̬
		if (re != NULL)
		{
			//	����ʧ��
			*re = false;
		}
		else
		{
			//	����������ʾ
			AfxMessageBox(LastErrorInfoString);
		}

		//	���ؿ��ַ���
		return "";
	}
}

//	��õ�ǰ��¼��������-1��ʾ��ȡʧ�ܣ������Ǹ���ֵ��ʾ��ȡ�ɹ�
long CMyDatabase::GetRecordCount(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "��ȡ��¼����¼����ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return -1;
	}

	//	���ؼ�¼����
	return p_RecordsetPtr->GetRecordCount();
}


//	��ȡ��ǰ��¼��λ�ã�-1��ʾ��ȡʧ�ܣ������Ǹ���ֵ��ʾ��ȡ�ɹ�
long CMyDatabase::GetRecordPos(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "��ȡ��¼����¼����ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return -1;
	}

	//	���ص�ǰ��¼λ��
	return mRecordPos;
}


//	����ǰ��¼�ƶ���ָ����λ�ã� true=�ɹ�     false=ʧ��
bool CMyDatabase::MoveTo(long pos)
{
	//	�����ƶ�����λ��
	MoveFirst();

	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return false;
	}

	//	����¼���洢�ļ�¼��
	if (GetRecordCount() <= 0)
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ���ǰ��¼��Ϊ�գ�";

		//	����ʧ��
		return false;
	}

	//	���pos�Ƿ�Ϸ���������¼����
	if (GetRecordCount() <= pos)
	{
		//	���ô�����Ϣ
		CString str;
		str.Format("�ƶ���ǰ��¼ʧ�ܣ������indexֵ%ld��",pos);
		LastErrorInfoString = str;

		//	����ʧ��
		return false;
	}

	//	�ƶ���¼
	p_RecordsetPtr->Move(pos);

	//	���õ�ǰλ��
	mRecordPos = pos;

	//	���سɹ�
	return true;
}


//	����ǰ��¼�ƶ�����λ�ã� true=�ɹ�     false=ʧ��
bool CMyDatabase::MoveFirst(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return false;
	}

	//	����¼���洢�ļ�¼��
	if (GetRecordCount() <= 0)
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ���ǰ��¼��Ϊ�գ�";

		//	����ʧ��
		return false;
	}

	//	�ƶ�����λ��
	p_RecordsetPtr->MoveFirst();

	//	���õ�ǰλ��
	mRecordPos = 0L;

	//	���سɹ�
	return true;
}

//	����ǰ��¼�ƶ���ĩβλ�ã� true=�ɹ�     false=ʧ��
bool CMyDatabase::MoveLast(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return false;
	}

	//	����¼���洢�ļ�¼��
	if (GetRecordCount() <= 0)
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ���ǰ��¼��Ϊ�գ�";

		//	����ʧ��
		return false;
	}

	//	�ƶ�����λ��
	p_RecordsetPtr->MoveLast();

	//	���õ�ǰλ��
	mRecordPos = GetRecordCount()-1L;

	//	���سɹ�
	return true;
}


//	�ƶ�����һ��λ�ã� true=�ɹ�     false=ʧ��
bool CMyDatabase::MoveNext(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return false;
	}

	//	����¼���洢�ļ�¼��
	if (GetRecordCount() <= 0)
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ���ǰ��¼��Ϊ�գ�";

		//	����ʧ��
		return false;
	}

	//	�ƶ�����λ��
	p_RecordsetPtr->MoveNext();

	//	�жϵ�ǰ���ƶ��Ĳ����Ƿ�Ϸ�������ĩβ���ʱ������ʧ�ܣ����ƶ�����
	if (p_RecordsetPtr->adoEOF)
	{
		//	�ص��ղ�λ��
		p_RecordsetPtr->MovePrevious();

		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ��µ��ƶ������¼�¼��ĩβ�����";

		//	����ʧ��
		return false;
	}

	//	���õ�ǰλ��
	++mRecordPos;

	//	���سɹ�
	return true;
}


//	�ƶ�����һ��λ�ã� true=�ɹ�     false=ʧ��
bool CMyDatabase::MovePrevious(void)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return false;
	}

	//	����¼���洢�ļ�¼��
	if (GetRecordCount() <= 0)
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ���ǰ��¼��Ϊ�գ�";

		//	����ʧ��
		return false;
	}

	//	�ƶ�����λ��
	p_RecordsetPtr->MovePrevious();

	//	�жϵ�ǰ���ƶ��Ĳ����Ƿ�Ϸ������Ӷ������ʱ������ʧ�ܣ����ƶ�����
	if (p_RecordsetPtr->adoBOF)
	{
		//	�ص��ղ�λ��
		p_RecordsetPtr->MoveNext();

		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ��µ��ƶ������¼�¼�Ӷ��������";

		//	����ʧ��
		return false;
	}

	//	���õ�ǰλ��
	--mRecordPos;

	//	���سɹ�
	return true;
}


//	�õ���ǰλ�õ�����
_variant_t CMyDatabase::GetCurrentData(
	long fields_index,					//	�����
	bool* re							//	���سɹ���־
	)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		CString tmp_str;
		tmp_str.Format("��ȡ��¼������ʧ�ܣ�����������δ��ʼ�������¼��δ������");
		LastErrorInfoString = tmp_str;

		//	����ʧ�ܣ���ָ��reʱ
		if (re != NULL)
		{
			//	����ʧ��
			*re = false;

			//	���ؿ�
			return "";
		} 
		//	��δָ��reʱ
		else
		{
			//	����������ʾ
			AfxMessageBox(LastErrorInfoString);

			//	���ؿ�
			return "";
		}
	}

	//	���fields_index�Ƿ�Ϸ�
	if (GetFieldsCount() <= fields_index)
	{
		//	���ô�����Ϣ
		CString tmp_str;
		tmp_str.Format("��ȡ��¼������ʧ�ܣ��Ƿ�����indexֵ%ld��",fields_index);
		LastErrorInfoString = tmp_str;

		//	����ʧ�ܣ���ָ��reʱ
		if (re != NULL)
		{
			//	����ʧ��
			*re = false;

			//	���ؿ�
			return "";
		} 
		//	��δָ��reʱ
		else
		{
			//	����������ʾ
			AfxMessageBox(LastErrorInfoString);

			//	���ؿ�
			return "";
		}
	}

	//	���Ի�ȡ����
	_variant_t re_data;
	try
	{
		//	��ȡ����
		re_data = p_RecordsetPtr->GetFields()->GetItem(fields_index)->GetValue();

		//	����״̬
		if (re != NULL)
		{
			//	���óɹ�
			*re = true;
		}

		//	��������
		return re_data;
	}
	catch (...)
	{
		//	���ô�����Ϣ
		CString tmp_str;
		tmp_str.Format("��ȡ��¼����ʧ�ܣ�δ֪����");
		LastErrorInfoString = tmp_str;

		//	����״̬
		if (re != NULL)
		{
			//	����ʧ��
			*re = false;
		}
		else
		{
			//	����������ʾ
			AfxMessageBox(LastErrorInfoString);
		}

		//	���ؿ�
		return "";
	}
}


//	����ת��Ϊ�ַ���
CString CMyDatabase::DataToCString(_variant_t var)
{
	return (LPCSTR)_bstr_t(var);
}

//	�õ���ǰλ�����ݵ��ַ���
CString CMyDatabase::GetCurrentDataString(
	long fields_index,					//	�����
	bool* re							//	���سɹ���־
	)
{
	return DataToCString(GetCurrentData(fields_index,re));
}


//	�õ�ָ��λ�õ�����
_variant_t CMyDatabase::GetData(
	long pos,							//	��¼���������ʼ��λ��
	long fields_index,					//	�����
	bool* re							//	���سɹ���־
	)
{
	//	�ƶ���ָ��λ��
	MoveTo(pos);

	//	��������
	return GetCurrentData(fields_index,re);
}

//	�õ�ָ��λ�����ݵ��ַ���
CString CMyDatabase::GetDataString(
	long pos,							//	��¼���������ʼ��λ��
	long fields_index,					//	�����
	bool* re							//	���سɹ���־
	)
{
	//	�ƶ���ָ��λ��
	MoveTo(pos);

	//	��������
	return GetCurrentDataString(fields_index,re);
}


//	����ǰ��¼�������λ���ƶ�����ʱ������ֵΪ��Ϊ��ĩβλ�÷����ƶ���Ϊ��Ϊ����λ�÷����ƶ��� true=�ɹ�     false=ʧ��
bool CMyDatabase::Move(long pos)
{
	//	����¼���Ƿ��Ѿ����������������Ѿ����������κ�һ���в���˳�
	if (!(ConnectionState && RecordsetPtrState))
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ�����������δ��ʼ�������¼��δ������";

		//	����ʧ��
		return false;
	}

	//	����¼���洢�ļ�¼��
	if (GetRecordCount() <= 0)
	{
		//	���ô�����Ϣ
		LastErrorInfoString = "�ƶ���ǰ��¼ʧ�ܣ���ǰ��¼��Ϊ�գ�";

		//	����ʧ��
		return false;
	}

	//	���pos�Ƿ�Ϸ���С��0ʱ�������ƶ��������ˣ�����0�����ܳ�����¼������
	if (!((0<=(GetRecordPos() + pos))&&((GetRecordPos() + pos)<GetRecordCount())))
	{
		//	���ô�����Ϣ
		CString str;
		str.Format("�ƶ���ǰ��¼ʧ�ܣ�����ƶ�λ���Ƴ��������indexֵ%ld��",pos);
		LastErrorInfoString = str;

		//	����ʧ��
		return false;
	}

	//	�ƶ���¼
	p_RecordsetPtr->Move(pos);

	//	���õ�ǰλ��
	mRecordPos = mRecordPos+pos;

	//	���سɹ�
	return true;
}