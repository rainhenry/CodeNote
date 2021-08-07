/************************************************************************

	�������ƣ��Զ������ݿ���
	���̰汾��0.1
	�������ڣ�2016.12.17
	ƽ    ̨��VS2010+SQL Server 2008 @ Win7 64bit SP1
	��Ʊ��룺rainhenry

	��������
	1��֧��SQL Server���ݿ⣨�������ݿ�δ���ԣ�
	2���Ի������ܵķ�װ

	ʹ�þ���
//////////////////////////////////////////////////////////////////////////
	//	�������ݿ�
	CMyDatabase mCMyDatabase;
	bool re = mCMyDatabase.Open(
		"127.0.0.1",
		"DATABASE_TEST1",
		"sa",
		"nbl287"
		);
	if (re)
	{
		infoout.AddStr("���ݿ����ӳɹ���\r\n");
	} 
	else
	{
		infoout.AddStr("���ݿ�����ʧ�ܣ�\r\n    ������Ϣ��" + mCMyDatabase.LastErrorInfoString + "\r\n");
	}
	
//////////////////////////////////////////////////////////////////////////
	//	ִ�в�ѯ,������������н��
	re = mCMyDatabase.ExeSQL("select * from DATABASE_TEST1.dbo.TABLES_TEST1;");
	if (re)
	{
		infoout.AddStr("ִ�в�ѯ�ɹ���\r\n");
		long count = mCMyDatabase.GetFieldsCount();
		infoout.AddStr("    ���%ld����\r\n",count);
	} 
	else
	{
		infoout.AddStr("ִ�в�ѯʧ�ܣ�\r\n,    ������Ϣ��%s\r\n",mCMyDatabase.LastErrorInfoString);
	}

	//	ѭ�������ǰ������
	for (long i=0;i<mCMyDatabase.GetFieldsCount();++i)
	{
		infoout.AddStr("    %ld�У�%s\r\n",i+1,mCMyDatabase.GetFieldsName(i));
	}

	//	�����¼���ڵ���Ŀ
	infoout.AddStr("��¼���ڰ���%ld������\r\n",mCMyDatabase.GetRecordCount());

	//	���������¼���ڵ���������
	//	��ѭ��Ϊ��¼��ѭ��
	for (long i=0;i<mCMyDatabase.GetRecordCount();++i)
	{
		//	��ѭ��Ϊ����Ŀ
		for (long j=0;j<mCMyDatabase.GetFieldsCount();++j)
		{
			infoout.AddStr("    %s",mCMyDatabase.GetDataString(i,j));
		}

		//	�������
		infoout.AddStr("\r\n");
	}

//////////////////////////////////////////////////////////////////////////
	//	�ͷ���Դ
	mCMyDatabase.Close();
	infoout.AddStr("���ݿ�Ͽ��ɹ���\r\n");

//////////////////////////////////////////////////////////////////////////
	���������
���ݿ����ӳɹ���
ִ�в�ѯ�ɹ���
    ���3����
    1�У�ID
    2�У�DATETIME
    3�У�CAP_STR
��¼���ڰ���2������
    1    2016/12/16 1:02:03    213TT
    2    2016/12/17    ABC
���ݿ�Ͽ��ɹ���

///////////////////////////////////////////////////////////////////////////
//	ִ�в�������
	//	ִ�в���
	bool re = mCMyDatabase.ExeSQL("insert into DATABASE_TEST1.dbo.TABLES_TEST1 (DATETIME,CAP_STR) VALUES (GETDATE(),'"+ str +"');");
	if (re)
	{
		infoout.AddStr("ִ�в���ɹ���\r\n");
	} 
	else
	{
		infoout.AddStr("ִ�в���ʧ�ܣ�\r\n,    ������Ϣ��%s\r\n",mCMyDatabase.LastErrorInfoString);
	}


************************************************************************/

//////////////////////////////////////////////////////////////////////////
//	�ض��屣��
#ifndef __CMYDATABASE_H__
#define __CMYDATABASE_H__

//////////////////////////////////////////////////////////////////////////
//	����ͷ�ļ��붯̬���ӿ�
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
#include <iostream>
#include <string>

//////////////////////////////////////////////////////////////////////////
//	�ඨ��
class CMyDatabase
{
private:
	//	���ݿ�����״̬	true=������		false=δ����
	bool ConnectionState;

	//	���ݿ��¼����״̬�� true=�Ѿ���     false=δ��
	bool RecordsetPtrState;

	//	��¼���ĵ�ǰ��¼��λ��
	long mRecordPos;

	//	��¼������
	_RecordsetPtr p_RecordsetPtr;
	
	//	���Ӷ���
	_ConnectionPtr p_ConnectionPtr;

public:
	CMyDatabase();
	CMyDatabase(CString);								//	��ΪͬOpenEx
	CMyDatabase(CString,CString,CString,CString);		//	��ΪͬOpen
	~CMyDatabase();

	//	�����ݿ����ӣ��Զ������� true=�ɹ�   false=ʧ��
	bool OpenEx(CString);

	//	�����ݿ����ӣ�ָ��IP�����ݿ������û��������룬 true=�ɹ�   false=ʧ��
	bool Open(
		CString host,				//	������ַ
		CString database_name,		//	���ݿ�����
		CString user,				//	�û���
		CString pwd					//	����
		);

	//	�ر����ݿ�
	void Close(void);

	//	ִ��SQL���,   true=�ɹ�   false=ʧ��
	bool ExeSQL(
		CString cmd
		);

	//	��ȡ��¼����������-1��ʾ��ȡʧ�ܣ�����ֵ��ʾ��ȡ���
	long GetFieldsCount(void);

	//	��ȡ��¼���е����֣���������ʱ������������������������ָ��re�Ļ���������true����ʾ�ɹ���������������""����ָ��reʱ�������false
	//	����δָ��reʱ��������Ϣ��ͨ��AfxMessageBox��ʽ����������indexΪ���������ţ�0����
	CString GetFieldsName(long index, bool* re = NULL);

	//	��õ�ǰ��¼��������-1��ʾ��ȡʧ�ܣ������Ǹ���ֵ��ʾ��ȡ�ɹ�
	long GetRecordCount(void);

	//	��ȡ��ǰ��¼��λ�ã�-1��ʾ��ȡʧ�ܣ������Ǹ���ֵ��ʾ��ȡ�ɹ�
	long GetRecordPos(void);

	//	����ǰ��¼�ƶ���ָ����λ�ã� true=�ɹ�     false=ʧ��
	bool MoveTo(long pos);

	//	����ǰ��¼�������λ���ƶ�����ʱ������ֵΪ��Ϊ��ĩβλ�÷����ƶ���Ϊ��Ϊ����λ�÷����ƶ��� true=�ɹ�     false=ʧ��
	bool Move(long pos);

	//	����ǰ��¼�ƶ�����λ�ã� true=�ɹ�     false=ʧ��
	bool MoveFirst(void);

	//	����ǰ��¼�ƶ���ĩβλ�ã� true=�ɹ�     false=ʧ��
	bool MoveLast(void);

	//	�ƶ�����һ��λ�ã� true=�ɹ�     false=ʧ��
	bool MoveNext(void);

	//	�ƶ�����һ��λ�ã� true=�ɹ�     false=ʧ��
	bool MovePrevious(void);

	//	�õ���ǰλ�õ�����
	_variant_t GetCurrentData(
		long fields_index,					//	�����
		bool* re = NULL						//	���سɹ���־
		);
	
	//	�õ���ǰλ�����ݵ��ַ���
	CString GetCurrentDataString(
		long fields_index,					//	�����
		bool* re = NULL						//	���سɹ���־
		);

	//	����ת��Ϊ�ַ���
	static CString DataToCString(_variant_t var);

	//	�õ�ָ��λ�õ�����
	_variant_t GetData(
		long pos,							//	��¼���������ʼ��λ��
		long fields_index,					//	�����
		bool* re = NULL						//	���سɹ���־
		);

	//	�õ�ָ��λ�����ݵ��ַ���
	CString GetDataString(
		long pos,							//	��¼���������ʼ��λ��
		long fields_index,					//	�����
		bool* re = NULL						//	���سɹ���־
		);

public:	
	//	���һ�εĴ�����Ϣ
	CString LastErrorInfoString;
};

//////////////////////////////////////////////////////////////////////////
#endif	//	__CMYDATABASE_H__

