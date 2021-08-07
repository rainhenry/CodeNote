/*************************************************************************

	�������ƣ��ı���Ϣ�����
	�� �� ����TextInfoOut.cpp
	��	  ����REV 1.0
	�������ڣ�2016.08.28
	��    �ƣ�rainhenry
	��    �룺rainhenry
	����/У��:rainhenry
	���Ի�����VS2010 + Win7 @ MFC
	��ϵ��ʽ��QQ 1444197961

���ܽ��ܣ�
	ͨ����ָ����CEdit�ؼ�������ʵ���ۻ��ı�������������趨�������Ƿ�������

����������
		���������ѧϰ�о����ۣ���������ת�ظ��ƣ�������������ҵ��;��ת��
	ʹ��ʱ���뱣����ע�͡�


*��ϸʹ��˵����μ�.h�ļ�
*************************************************************************/

//	����ͷ�ļ�  //////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TextInfoOut.h"

//////////////////////////////////////////////////////////////////////////
//	���캯��
CTextInfoOut::CTextInfoOut(CEdit* pc)
{
	updateflag = true;									//	Ĭ��Ϊ���µ�
	this->keepflag = DEFAULT_KEEP_FLAG;					//	�趨Ĭ�ϵĹ���ת̬
	this->pCEdit = pc;									//	�趨�ؼ�ָ��
	this->str_buffer = "";								//	����ַ���
}

//	���캯��,���й������ò���
CTextInfoOut::CTextInfoOut(CEdit* pc ,bool f)
{
	updateflag = true;									//	Ĭ��Ϊ���µ�
	this->keepflag = f;									//	�趨Ĭ�ϵĹ���ת̬
	this->pCEdit = pc;									//	�趨�ؼ�ָ��
	this->str_buffer = "";								//	����ַ���
}

//	��������
CTextInfoOut::~CTextInfoOut()
{

}

//	׷���ַ���
void CTextInfoOut::AddStr(CString in_str)
{
	//	׷��
	this->str_buffer += in_str;

	//	����Ƿ���Ҫ����,����Ҫ���µĶ�ʱ��ֱ���������´���
	if (updateflag == false)
	{
		return ;
	}

	//	���ؼ�ָ����õ�ʱ��
	if (!((this->pCEdit == INVALID_PCEDIT_ADDRESS)||(this->pCEdit == NULL)))
	{
		//	����ַ���
		this->pCEdit->SetWindowText(this->str_buffer);

		//	���ƹ�����
		if (this->keepflag == true)
		{
			this->pCEdit->LineScroll(this->pCEdit->GetLineCount());	//	������ָ�����һ��
		}
	}
}
//	׷���ַ�������ʽ����ʽ
void CTextInfoOut::AddStr(
	char* pszFormat,
	...)
{
	CString tmp;
	ATLASSERT( AtlIsValidString( pszFormat ) );
	va_list argList;
	va_start( argList, pszFormat );
	tmp.FormatV(pszFormat, argList);
	va_end( argList );

	//	׷��
	this->str_buffer += tmp;

	//	����Ƿ���Ҫ����,����Ҫ���µĶ�ʱ��ֱ���������´���
	if (updateflag == false)
	{
		return ;
	}

	//	���ؼ�ָ����õ�ʱ��
	if (!((this->pCEdit == INVALID_PCEDIT_ADDRESS)||(this->pCEdit == NULL)))
	{
		//	����ַ���
		this->pCEdit->SetWindowText(this->str_buffer);

		//	���ƹ�����
		if (this->keepflag == true)
		{
			this->pCEdit->LineScroll(this->pCEdit->GetLineCount());	//	������ָ�����һ��
		}
	}
}

//	����ַ���
void CTextInfoOut::Clear(void)
{
	this->str_buffer = "";
}

//	��ȡȫ���ַ���
CString CTextInfoOut::GetString(void)
{
	return this->str_buffer;
}

//	���Ĺ���ת̬
void CTextInfoOut::SetKeep(bool f)
{
	this->keepflag = f;
}


//	ʹ�ܽ�ֹ����
void CTextInfoOut::EnableUpdate(bool flag)
{
	updateflag = flag;
}

