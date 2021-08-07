/*************************************************************************

	�������ƣ���������ת��
	�� �� ����LineConv.cpp
	��	  ����REV 1.0
	�������ڣ�2016.08.27
	��    �ƣ�rainhenry
	��    �룺rainhenry
	����/У��:rainhenry
	���Ի�����VS2010 + Win7
	��ϵ��ʽ��QQ 1444197961

���ܽ��ܣ�
		ʵ��"\r\n"��"\n"֮���ת��

����������
		���������ѧϰ�о����ۣ���������ת�ظ��ƣ�������������ҵ��;��ת��
	ʹ��ʱ���뱣����ע�͡�
	

*��ϸʹ��˵����μ�.h�ļ�
*************************************************************************/


//	����ͷ�ļ�  //////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LineConv.h"

//	ʵ�嶨��  ////////////////////////////////////////////////////////////
//	���캯��
CLineConv::CLineConv()
{

}

//	��������
CLineConv::~CLineConv()
{

}

//	ת������
std::string CLineConv::Conv(std::string str ,CLineConv::TargetFormat tf)
{
	//	����Ŀ����ʽ���ֱ���
	switch (tf)
	{
	//	Ŀ��Ϊ"\r\n"
	case CLineConv::TF_RN:
		return CLineConv::ConvToRN(str);
		//	Ŀ��Ϊ"\n"
	case CLineConv::TF_N:
		return CLineConv::ConvToN(str);
	//	������ʽԭ�ⷵ��
	default:	
		return str;
	}
}

//	ת��Ϊ"\r\n"
std::string CLineConv::ConvToRN(std::string str)
{
	//	�ַ�������2���ַ�
	if (str.length() >= 2)
	{
		int i = 0;
		for (i=0;str.begin()+i!=str.end()-1;i++)
		{
			//	����ƥ��"\r\n"
			if ((*(str.begin()+i) == '\r')&&(*((str.begin()+i)+1) == '\n'))
			{
				//	��Ŀ��ƥ�䣬����ת��
			}

			//	���ڳ�ʼλ��
			if (i!=0)
			{
				//	ƥ��"\n"
				if ((*((str.begin()+i)-1) != '\r')&&(*(str.begin()+i) == '\n'))
				{
					str.insert((str.begin()+i),'\r');
					i++;
				}
			}
			//	�ڳ�ʼλ��
			else
			{
				if (*(str.begin()+i) == '\n')
				{
					str.insert((str.begin()+i),'\r');
					i++;
				}
			}
		}

		//	������һ���ַ�
		if ((*(str.begin()+i) == '\n')&&(*((str.begin()+i)-1) != '\r'))
		{
			str.insert((str.begin()+i),'\r');
		}

		//	���ؽ��
		return str;
	} 
	//	�ַ���Ϊ1���ַ�
	else if(str.length() == 1)
	{
		str.insert(str.begin(),'\r');

		//	���ؽ��
		return str;
	}
	//	�ַ���Ϊ��
	else
	{
		return "";		//	���ؿ��ַ���
	}
}

//	ת��Ϊ"\n"
std::string CLineConv::ConvToN(std::string str)
{
	//	�ַ�����Ϊ��
	if (str != "")
	{
		int i=0;
		for (i=0;(str.begin()+i)!=str.end();i++)
		{
			//	����"\r"
			if (*(str.begin()+i) == '\r')
			{
				str.erase((str.begin()+i));
			}

			//	������Χ
			if ((str.begin()+i) == str.end())
			{
				break;
			}
		}

		//	����
		return str;
	}
	//	�ַ���Ϊ��
	else
	{
		return "";		//	���ؿ��ַ���
	}
}

