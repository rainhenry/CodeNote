/*************************************************************************

	�������ƣ���������ת��
	�� �� ����LineConv.h
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

ʹ�þ���

//	Ҫ��ԭ�ַ���ͳһת��Ϊ"\r\n"��ʽ
std::string str = "source string\ntest\r\nhello";
str = CLineConv::Conv(str ,CLineConv::TF_RN);

//	����ͳһת��Ϊ"\n"��ʽ
str = CLineConv::Conv(str ,CLineConv::TF_N);
	

*************************************************************************/



//  Ԥ���뱣��  //////////////////////////////////////////////////////////
#ifndef __LINECONV_H__
#define __LINECONV_H__

//  ����ͷ�ļ�  //////////////////////////////////////////////////////////
#include <iostream> 
#include <string>

//  �궨������  //////////////////////////////////////////////////////////


//	�ඨ��  //////////////////////////////////////////////////////////////
class CLineConv
{
public:
	//	���캯��
	CLineConv();
	//	��������
	~CLineConv();

	//	Ŀ���ʽ����
	enum TargetFormat 
	{
		TF_RN,						//	"\r\n"
		TF_N,						//	"\n"
	};

	//	ת������
	static std::string Conv(std::string ,TargetFormat);

private:
	//	ת��Ϊ"\r\n"
	static std::string ConvToRN(std::string);
	//	ת��Ϊ"\n"
	static std::string ConvToN(std::string);
};



//////////////////////////////////////////////////////////////////////////
#endif	//	__LINECONV_H__

