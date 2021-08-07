
/*************************************************************************

	�������ƣ�MATLAB Engine �ӿڳ����
	�� �� ����MatlabEngine.cpp
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

*��ϸʹ�÷��������MatlabEngine.h�ļ�
*************************************************************************/

//	����ͷ�ļ�  //////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MatlabEngine.h"
#include "engine.h"

//	������̬��  //////////////////////////////////////////////////////////
#pragma comment( lib, "libeng.lib" )
#pragma comment( lib, "libmx.lib" )
#pragma comment( lib, "libmat.lib" )

//  �ඨ��  //////////////////////////////////////////////////////////////
//	���캯��
CMatlabEngine::CMatlabEngine()								//	����궨����������Ĭ�ϻ������ߴ�
{
	/*
		��ģʽ���������£����ܻ�������СΪĬ�ϵĺ궨���С
		�������ͷź�������Ҫ�ͷŸû�����
	*/
	this->engine_ready_ok = false;										//	����ӿ�δ׼����
	this->str_buffer = new char[MATLABENGINE_STRING_BUFFER_SIZE];		//	�����ڴ�

	//	��黺�����Ƿ�����ɹ�
	if((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS))
	//	����������ʧ��
	{
		this->buffer_size = 0;	
		this->str_buffer = NULL;
	}
	else
	{
		this->buffer_size = MATLABENGINE_STRING_BUFFER_SIZE;				//	������ջ�������С
		*(this->str_buffer) = '\0';											//	���ַ���
	}

	this->uesr_define_buffer = false;										//	���û��Զ���
	this->pMatlabEngine = NULL;												//	��
	this->lock_outstrbuffer_ok = false;										//	û��������������
}

//	���캯�����Զ��建������С��
CMatlabEngine::CMatlabEngine(int size)						//	�����û��������Ļ������ߴ�
{
	/*
		��ģʽ���������£����ܻ�������СΪ�û�ָ���Ĵ�С
		�������ͷź�������Ҫ�ͷŸû�����
	*/
	this->engine_ready_ok = false;										//	����ӿ�δ׼����
	this->str_buffer = new char[size];									//	�����ڴ�

	//	��黺�����Ƿ�����ɹ�
	if((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS))
	//	����������ʧ��
	{
		this->buffer_size = 0;	
		this->str_buffer = NULL;
	}
	else
	{
		this->buffer_size = size;											//	������ջ�������С
		*(this->str_buffer) = '\0';											//	���ַ���
	}

	this->uesr_define_buffer = false;										//	���û��Զ���
	this->pMatlabEngine = NULL;												//	��
	this->lock_outstrbuffer_ok = false;										//	û��������������
}

//	���캯�����Զ��建������
CMatlabEngine::CMatlabEngine(char* pstr ,int length)		//	�û�ָ���������ʹ�С,��ʱ�ͷź�����������û��������ͷ�
{
	/*
		��ģʽ���������£����ܻ�������СΪ�û�ָ���Ĵ�С�����һ�����ָ�뽫ָ���û���ָ��
		�������ͷź�������Ҫ�ͷŸû�����
	*/
	this->engine_ready_ok = false;										//	����ӿ�δ׼����
	this->str_buffer = pstr;											//	ָ���û�ָ��

	//	��黺�����Ƿ�����ɹ�
	if((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS))
	//	�û�ָ���˴���Ļ�����
	{
		this->buffer_size = 0;	
		this->str_buffer = NULL;
	}
	else
	{
		this->buffer_size = length;										//	������ջ�������С
		*(this->str_buffer) = '\0';											//	���ַ���
	}

	this->uesr_define_buffer = true;										//	�û��Զ���
	this->pMatlabEngine = NULL;												//	��
	this->lock_outstrbuffer_ok = false;										//	û��������������
}

//	��������
CMatlabEngine::~CMatlabEngine()
{
	//	�رսӿڣ��ͷŵ�ǰ��������
	this->Release();

	//	��黺�����Ϸ� ���Ҳ����û�ָ����
	if((!((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS)))&&(this->uesr_define_buffer == false))
	//	���������ã������ڴ��ͷ�
	{
		this->buffer_size = 0;	
		delete[] (this->str_buffer);
		this->str_buffer = NULL;
	}
	else
	//	�������Ƿ�
	{
		this->str_buffer = NULL;
	}
}

//	��ʼ��
CMatlabEngine::ReturnType CMatlabEngine::Init(void)			//	ע�⣺�ú�������ʱ�Ƚϳ��Ż᷵�أ���ΪҪ��ʼ���ܾá�
{
	//	��������Ƿ��Ѵ���
	if (!(((this->pMatlabEngine == NULL)||(this->pMatlabEngine == INVALID_MATLABENGINE))))
	{
		this->Release();								//	�ͷ��Ѿ����ڵ���Դ
	}

	//	��������
	this->pMatlabEngine = engOpen(NULL);
	if ((this->pMatlabEngine == NULL)||(this->pMatlabEngine == INVALID_MATLABENGINE))
	{
		return CMatlabEngine::OPEN_FAIL;
	} 
	else
	{
		this->engine_ready_ok = true;					//	��ʱ�������ӽӿ��Ѿ�׼����

		//	�趨��������������
		if (engOutputBuffer(this->pMatlabEngine ,this->str_buffer ,this->buffer_size - 1) == 0)
		{
			this->lock_outstrbuffer_ok = true;			//	�ɹ�
		} 
		else
		{
			this->lock_outstrbuffer_ok = false;			//	ʧ��
		}

		return CMatlabEngine::OPEN_OK;
	}
}

//	�ͷ���Դ
CMatlabEngine::ReturnType CMatlabEngine::Release(void)
{
	this->engine_ready_ok = false;										//	����ӿ�δ׼����
	this->lock_outstrbuffer_ok = false;									//	û��������������

	//	�����������ָ��
	if ((this->pMatlabEngine == NULL)||(this->pMatlabEngine == INVALID_MATLABENGINE))
	{
		return CMatlabEngine::CLOSE_FAIL;
	} 
	else
	{
		if (engClose(this->pMatlabEngine) == 0)
		{
			return CMatlabEngine::CLOSE_OK;
		} 
		else
		{
			return CMatlabEngine::CLOSE_FAIL;
		}
	}
}

//	ִ������
std::string CMatlabEngine::Execmd(std::string cmd)
{
	//	���ӿ�׼����ʱ
	if (this->engine_ready_ok == true)
	{
		//	���֮ǰ�Ľ��
		*(this->str_buffer) = '\0';

		//	ִ������
		if (engEvalString(this->pMatlabEngine ,cmd.data()) == 0)
			//	ִ�гɹ�
		{
			*(this->str_buffer + this->buffer_size - 1) = '\0';		//	�ضϣ���ֹԽ��
			return this->str_buffer;
		} 
		else
		{
			return "";
		}
	} 
	else
	{
		return "";
	}
}

//	ִ������з���״̬
std::string CMatlabEngine::Execmd(std::string cmd ,CMatlabEngine::ReturnType& re)
{
	//	���ӿ�׼����ʱ
	if (this->engine_ready_ok == true)
	{
		//	���֮ǰ�Ľ��
		*(this->str_buffer) = '\0';

		//	ִ������
		if (engEvalString(this->pMatlabEngine ,cmd.data()) == 0)
			//	ִ�гɹ�
		{
			*(this->str_buffer + this->buffer_size - 1) = '\0';		//	�ضϣ���ֹԽ��
			re = CMatlabEngine::EXEC_OK;			//	ִ�гɹ�
			return this->str_buffer;
		} 
		else
		{
			re = CMatlabEngine::EXEC_FAIL;			//	ִ��ʧ��
			return "";
		}
	} 
	else
	{
		re = CMatlabEngine::NOT_INIT;			//	û�г�ʼ��
		return "";
	}
}

//	�汾�ַ���
const std::string CMatlabEngine::REV_STR = "REV1.0";
//	���������ַ���
const std::string CMatlabEngine::PROJECT_DATE = "2016/08/27";

