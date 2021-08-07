/*************************************************************************

	�������ƣ�MATLAB Engine �ӿڳ����
	�� �� ����MatlabEngine.h
	��	  ����REV 1.0
	�������ڣ�2016.08.27
	��    �ƣ�rainhenry
	��    �룺rainhenry
	����/У��:rainhenry
	��	  ����VS2010 + Win7 64bit
	ƽ	  ̨��Debug/Relesae x64
	��Ʋο���http://cn.mathworks.com/help/matlab/calling-matlab-engine-from-c-c-and-fortran-programs.html
	��ϵ��ʽ��QQ 1444197961

���ܽ��ܣ�
		��C/C++������ʵ��MATLAB�����ִ�кͻ�ȡ������ð汾�����ṩ�ַ���
	������ظ��û�����һ���Ĵ����������ʵ���˶�MATLAB Engine�ӿڵļ�
	��װ�ͳ�ʼ��������װ�����ṩ���û�ʹ��

����������
		���������ѧϰ�о����ۣ���������ת�ظ��ƣ�������������ҵ��;��ת��
	ʹ��ʱ���뱣����ע�͡�

��������ע�����
		���������֧��64λƽ̨��windowsӦ�ó���������ѡ��x64ƽ̨���б���
	������ʱ�����ð���Ŀ¼����MATLAB��װĿ¼��includeĿ¼���Լ���صĿ�Ŀ¼
	��C�̰�װΪ����
	����Ŀ¼Ϊ��
		C:\Program Files\MATLAB\R2015b\extern\include
	��Ŀ¼Ϊ��
		C:\Program Files\MATLAB\R2015b\extern\lib\win64\microsoft
	���ұ���������ʱ��Ҫlibeng.dll��̬���ӿ⣬������ϵͳ����������PATH��������Ŀ¼��
		C:\Program Files\MATLAB\R2015b\bin\win64
	�����������ú�ſ�����ȷ���롢���Ӻ����С�

ʹ��˵��
	1���������Ŀ¼�ͻ�������
	2����������ӿ�ʵ��
	3����ʼ������ӿڣ���ʱ��Ƚϳ�����Լ10��20�룬�ϻ�������ã�
	4��ִ������
	5������ѡ���ͷ�����ӿڣ�����������������ʱ����Զ��ͷţ����Դ˲������ʡ��
		ע�⣺�궨�� MATLABENGINE_STRING_BUFFER_SIZE ������MATLAB���ص��ַ����������ĳ��ȣ������
	��ø����������������ʵ������ú꣬���߽������ʱ����MATLAB��workspace�зֶη��ص�������Ļ�����

-------------------------------------------------------------------------------------------------
ʹ�þ���

//	��������ӿ�ʵ��
//	������ʵ����ͬʱ������Ҳ������ϵͳ����
CMatlabEngine my_matlabengine;

//	��ʼ������ӿ�
my_matlabengine.Init();			//	��ʼ������ӿ�

//	ִ�����ע�⣺�÷��ص��ַ����Ļ��и�ʽΪ"\n"��������"\r\n"
std::string str = my_matlabengine.Execmd("pwd");

//	�����Ҫ���Խ��л��и�ʽת��,ת��Ϊ"\r\n"��ʽ
//	����ʹ�ñ����Լ������ת���࣬��� LineConv.h
str = CLineConv::Conv(str ,CLineConv::TF_RN);

//	�ͷ�����ӿ�
//	ע�⣺���ͷŲ�������������ӿڽ����ͷţ�û�ж��ַ�����������ͷ�
my_matlabengine.Release();




-------------------------------------------------------------------------------------------------
�����÷�

//	�����ر�˵���¹���ʵ����ʱ���м��ֹ����ʽ
CMatlabEngine();							//	����궨����������Ĭ�ϻ������ߴ�
CMatlabEngine(int);							//	�����û��������Ļ������ߴ�
CMatlabEngine(char* ,int);					//	�û�ָ���������ʹ�С,��ʱ�ͷź�����������û��������ͷ�
	

*************************************************************************/

//  Ԥ���뱣��  //////////////////////////////////////////////////////////
#ifndef __MATLABENGINE_H__
#define __MATLABENGINE_H__

//  ����ͷ�ļ�  //////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "engine.h"

//  �궨������  //////////////////////////////////////////////////////////
#define INVALID_MATLABENGINE					((Engine*)(-1))							//	�Ƿ�������ָ��
#define INVALID_CHAR_ADDRESS					((char*)(-1))							//	�Ƿ����ַ�ָ��
#define MATLABENGINE_STRING_BUFFER_SIZE			4096									//	���������ַ������ճߴ�

//	�ඨ��  //////////////////////////////////////////////////////////////
class CMatlabEngine
{
public:
	//	���캯��
	CMatlabEngine();							//	����궨����������Ĭ�ϻ������ߴ�
	//	���캯�����Զ��建������С��
	CMatlabEngine(int);							//	�����û��������Ļ������ߴ�
	//	���캯�����Զ��建������
	CMatlabEngine(char* ,int);					//	�û�ָ���������ʹ�С,��ʱ�ͷź�����������û��������ͷ�
	//	��������
	~CMatlabEngine();

	//	����ö�����Ͷ���
	enum ReturnType 
	{
		OPEN_OK,								//	�򿪳ɹ�
		OPEN_FAIL,								//	��ʧ��
		CLOSE_OK,								//	�رճɹ�
		CLOSE_FAIL,								//	�ر�ʧ��
		NOT_INIT,								//	û�г�ʼ��
		EXEC_FAIL,								//	ִ��ʧ��
		EXEC_OK,								//	ִ�гɹ�
	};

	//	��ʼ��
	ReturnType Init(void);						//	ע�⣺�ú�������ʱ�Ƚϳ��Ż᷵�أ���ΪҪ��ʼ���ܾá�

	//	�ͷ���Դ
	ReturnType Release(void);

	//	ִ������,û�з���״̬
	std::string Execmd(std::string);
	//	ִ������з���״̬
	std::string Execmd(std::string ,ReturnType&);

	//	��汾�ַ���
	const static std::string REV_STR;
	//	���������ַ���
	const static std::string PROJECT_DATE;

private:
	//	����׼���ñ�־
	bool engine_ready_ok;		

	//	���ջ������׵�ַ
	char* str_buffer;							//	�û��������ö�̬����
	//	��������С
	int buffer_size;
	//	�������Ƿ��û�����
	bool uesr_define_buffer;					//	ֻ�����û�ָ��������ʱ����Ч

	//	����ӿ�ָ��
	Engine* pMatlabEngine;		

	//	������������������ɹ�
	bool lock_outstrbuffer_ok;		
};


//////////////////////////////////////////////////////////////////////////
#endif //	__MATLABENGINE_H__