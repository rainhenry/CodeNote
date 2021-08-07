
/*************************************************************************

	工程名称：MATLAB Engine 接口程序包
	文 件 名：MatlabEngine.cpp
	版	  本：REV 1.0
	工程日期：2016.08.27
	设    计：rainhenry
	编    码：rainhenry
	调试/校对:rainhenry
	环	  境：VS2010 + Win7 64bit
	平	  台：Debug/Relesae x64
	设计参考：http://cn.mathworks.com/help/matlab/calling-matlab-engine-from-c-c-and-fortran-programs.html
	联系方式：QQ 1444197961

功能介绍：
		在C/C++环境下实现MATLAB命令的执行和获取结果，该版本仅仅提供字符串
	结果返回给用户做进一步的处理。本程序包实现了对MATLAB Engine接口的简单
	包装和初始化，并封装成类提供给用户使用

免责声明：
		本程序仅供学习研究讨论，可以随意转载复制，但不得用于商业用途！转载
	使用时，请保留本注释。

*详细使用方法请详见MatlabEngine.h文件
*************************************************************************/

//	包含头文件  //////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MatlabEngine.h"
#include "engine.h"

//	包含静态库  //////////////////////////////////////////////////////////
#pragma comment( lib, "libeng.lib" )
#pragma comment( lib, "libmx.lib" )
#pragma comment( lib, "libmat.lib" )

//  类定义  //////////////////////////////////////////////////////////////
//	构造函数
CMatlabEngine::CMatlabEngine()								//	分配宏定义所给出的默认缓冲区尺寸
{
	/*
		该模式构造的情况下，接受缓冲区大小为默认的宏定义大小
		并且在释放函数里面要释放该缓冲区
	*/
	this->engine_ready_ok = false;										//	引擎接口未准备好
	this->str_buffer = new char[MATLABENGINE_STRING_BUFFER_SIZE];		//	申请内存

	//	检查缓冲区是否申请成功
	if((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS))
	//	缓冲区申请失败
	{
		this->buffer_size = 0;	
		this->str_buffer = NULL;
	}
	else
	{
		this->buffer_size = MATLABENGINE_STRING_BUFFER_SIZE;				//	定义接收缓冲区大小
		*(this->str_buffer) = '\0';											//	空字符串
	}

	this->uesr_define_buffer = false;										//	非用户自定义
	this->pMatlabEngine = NULL;												//	空
	this->lock_outstrbuffer_ok = false;										//	没有锁定到缓冲区
}

//	构造函数（自定义缓冲区大小）
CMatlabEngine::CMatlabEngine(int size)						//	分配用户所给出的缓冲区尺寸
{
	/*
		该模式构造的情况下，接受缓冲区大小为用户指定的大小
		并且在释放函数里面要释放该缓冲区
	*/
	this->engine_ready_ok = false;										//	引擎接口未准备好
	this->str_buffer = new char[size];									//	申请内存

	//	检查缓冲区是否申请成功
	if((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS))
	//	缓冲区申请失败
	{
		this->buffer_size = 0;	
		this->str_buffer = NULL;
	}
	else
	{
		this->buffer_size = size;											//	定义接收缓冲区大小
		*(this->str_buffer) = '\0';											//	空字符串
	}

	this->uesr_define_buffer = false;										//	非用户自定义
	this->pMatlabEngine = NULL;												//	空
	this->lock_outstrbuffer_ok = false;										//	没有锁定到缓冲区
}

//	构造函数（自定义缓冲区）
CMatlabEngine::CMatlabEngine(char* pstr ,int length)		//	用户指定缓冲区和大小,此时释放函数将不处理该缓冲区的释放
{
	/*
		该模式构造的情况下，接受缓冲区大小为用户指定的大小，并且缓冲区指针将指向用户的指针
		并且在释放函数里面要释放该缓冲区
	*/
	this->engine_ready_ok = false;										//	引擎接口未准备好
	this->str_buffer = pstr;											//	指向用户指针

	//	检查缓冲区是否申请成功
	if((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS))
	//	用户指定了错误的缓冲区
	{
		this->buffer_size = 0;	
		this->str_buffer = NULL;
	}
	else
	{
		this->buffer_size = length;										//	定义接收缓冲区大小
		*(this->str_buffer) = '\0';											//	空字符串
	}

	this->uesr_define_buffer = true;										//	用户自定义
	this->pMatlabEngine = NULL;												//	空
	this->lock_outstrbuffer_ok = false;										//	没有锁定到缓冲区
}

//	析构函数
CMatlabEngine::~CMatlabEngine()
{
	//	关闭接口，释放当前引擎连接
	this->Release();

	//	检查缓冲区合法 并且不是用户指定的
	if((!((this->str_buffer == NULL)||(this->str_buffer == INVALID_CHAR_ADDRESS)))&&(this->uesr_define_buffer == false))
	//	缓冲区可用，进行内存释放
	{
		this->buffer_size = 0;	
		delete[] (this->str_buffer);
		this->str_buffer = NULL;
	}
	else
	//	缓冲区非法
	{
		this->str_buffer = NULL;
	}
}

//	初始化
CMatlabEngine::ReturnType CMatlabEngine::Init(void)			//	注意：该函数会延时比较长才会返回，因为要初始化很久。
{
	//	检测连接是否已存在
	if (!(((this->pMatlabEngine == NULL)||(this->pMatlabEngine == INVALID_MATLABENGINE))))
	{
		this->Release();								//	释放已经存在的资源
	}

	//	创建连接
	this->pMatlabEngine = engOpen(NULL);
	if ((this->pMatlabEngine == NULL)||(this->pMatlabEngine == INVALID_MATLABENGINE))
	{
		return CMatlabEngine::OPEN_FAIL;
	} 
	else
	{
		this->engine_ready_ok = true;					//	此时引擎连接接口已经准备好

		//	设定引擎的输出缓冲区
		if (engOutputBuffer(this->pMatlabEngine ,this->str_buffer ,this->buffer_size - 1) == 0)
		{
			this->lock_outstrbuffer_ok = true;			//	成功
		} 
		else
		{
			this->lock_outstrbuffer_ok = false;			//	失败
		}

		return CMatlabEngine::OPEN_OK;
	}
}

//	释放资源
CMatlabEngine::ReturnType CMatlabEngine::Release(void)
{
	this->engine_ready_ok = false;										//	引擎接口未准备好
	this->lock_outstrbuffer_ok = false;									//	没有锁定到缓冲区

	//	检查引擎连接指针
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

//	执行命令
std::string CMatlabEngine::Execmd(std::string cmd)
{
	//	当接口准备好时
	if (this->engine_ready_ok == true)
	{
		//	清空之前的结果
		*(this->str_buffer) = '\0';

		//	执行命令
		if (engEvalString(this->pMatlabEngine ,cmd.data()) == 0)
			//	执行成功
		{
			*(this->str_buffer + this->buffer_size - 1) = '\0';		//	截断，防止越界
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

//	执行命令，有返回状态
std::string CMatlabEngine::Execmd(std::string cmd ,CMatlabEngine::ReturnType& re)
{
	//	当接口准备好时
	if (this->engine_ready_ok == true)
	{
		//	清空之前的结果
		*(this->str_buffer) = '\0';

		//	执行命令
		if (engEvalString(this->pMatlabEngine ,cmd.data()) == 0)
			//	执行成功
		{
			*(this->str_buffer + this->buffer_size - 1) = '\0';		//	截断，防止越界
			re = CMatlabEngine::EXEC_OK;			//	执行成功
			return this->str_buffer;
		} 
		else
		{
			re = CMatlabEngine::EXEC_FAIL;			//	执行失败
			return "";
		}
	} 
	else
	{
		re = CMatlabEngine::NOT_INIT;			//	没有初始化
		return "";
	}
}

//	版本字符串
const std::string CMatlabEngine::REV_STR = "REV1.0";
//	工程日期字符串
const std::string CMatlabEngine::PROJECT_DATE = "2016/08/27";

