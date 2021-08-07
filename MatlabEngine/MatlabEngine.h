/*************************************************************************

	工程名称：MATLAB Engine 接口程序包
	文 件 名：MatlabEngine.h
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

编译链接注意事项：
		本代码仅仅支持64位平台的windows应用程序，所以请选择x64平台进行编译
	，编译时请设置包含目录包含MATLAB安装目录下include目录，以及相关的库目录
	以C盘安装为例：
	包含目录为：
		C:\Program Files\MATLAB\R2015b\extern\include
	库目录为：
		C:\Program Files\MATLAB\R2015b\extern\lib\win64\microsoft
	并且本程序运行时需要libeng.dll动态链接库，请设置系统环境变量的PATH加入如下目录：
		C:\Program Files\MATLAB\R2015b\bin\win64
	经过以上设置后才可以正确编译、链接和运行。

使用说明
	1、设置相关目录和环境变量
	2、声明引擎接口实例
	3、初始化引擎接口（该时间比较长，大约10到20秒，老机器会更久）
	4、执行命令
	5、（可选）释放引擎接口，本代码在类析构的时候会自动释放，所以此步骤可以省略
		注意：宏定义 MATLABENGINE_STRING_BUFFER_SIZE 定义了MATLAB返回的字符串可能最大的长度，如果想
	获得更长的输出结果，请适当调整该宏，或者将结果临时存入MATLAB的workspace中分段返回到本程序的缓冲区

-------------------------------------------------------------------------------------------------
使用举例

//	声明引擎接口实例
//	声明该实例的同时，缓存也将会向系统申请
CMatlabEngine my_matlabengine;

//	初始化引擎接口
my_matlabengine.Init();			//	初始化引擎接口

//	执行命令，注意：该返回的字符串的换行格式为"\n"，而不是"\r\n"
std::string str = my_matlabengine.Execmd("pwd");

//	如果需要可以进行换行格式转换,转换为"\r\n"格式
//	这里使用笔者自己定义的转换类，详见 LineConv.h
str = CLineConv::Conv(str ,CLineConv::TF_RN);

//	释放引擎接口
//	注意：该释放操作仅仅对引擎接口进行释放，没有对字符串缓存进行释放
my_matlabengine.Release();




-------------------------------------------------------------------------------------------------
特殊用法

//	这里特别说明下构造实例的时候有几种构造格式
CMatlabEngine();							//	分配宏定义所给出的默认缓冲区尺寸
CMatlabEngine(int);							//	分配用户所给出的缓冲区尺寸
CMatlabEngine(char* ,int);					//	用户指定缓冲区和大小,此时释放函数将不处理该缓冲区的释放
	

*************************************************************************/

//  预编译保护  //////////////////////////////////////////////////////////
#ifndef __MATLABENGINE_H__
#define __MATLABENGINE_H__

//  包含头文件  //////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "engine.h"

//  宏定义声明  //////////////////////////////////////////////////////////
#define INVALID_MATLABENGINE					((Engine*)(-1))							//	非法的引擎指针
#define INVALID_CHAR_ADDRESS					((char*)(-1))							//	非法的字符指针
#define MATLABENGINE_STRING_BUFFER_SIZE			4096									//	定义最大的字符串接收尺寸

//	类定义  //////////////////////////////////////////////////////////////
class CMatlabEngine
{
public:
	//	构造函数
	CMatlabEngine();							//	分配宏定义所给出的默认缓冲区尺寸
	//	构造函数（自定义缓冲区大小）
	CMatlabEngine(int);							//	分配用户所给出的缓冲区尺寸
	//	构造函数（自定义缓冲区）
	CMatlabEngine(char* ,int);					//	用户指定缓冲区和大小,此时释放函数将不处理该缓冲区的释放
	//	析构函数
	~CMatlabEngine();

	//	返回枚举类型定义
	enum ReturnType 
	{
		OPEN_OK,								//	打开成功
		OPEN_FAIL,								//	打开失败
		CLOSE_OK,								//	关闭成功
		CLOSE_FAIL,								//	关闭失败
		NOT_INIT,								//	没有初始化
		EXEC_FAIL,								//	执行失败
		EXEC_OK,								//	执行成功
	};

	//	初始化
	ReturnType Init(void);						//	注意：该函数会延时比较长才会返回，因为要初始化很久。

	//	释放资源
	ReturnType Release(void);

	//	执行命令,没有返回状态
	std::string Execmd(std::string);
	//	执行命令，有返回状态
	std::string Execmd(std::string ,ReturnType&);

	//	类版本字符串
	const static std::string REV_STR;
	//	工程日期字符串
	const static std::string PROJECT_DATE;

private:
	//	引擎准备好标志
	bool engine_ready_ok;		

	//	接收缓冲区首地址
	char* str_buffer;							//	该缓冲区采用动态分配
	//	缓冲区大小
	int buffer_size;
	//	缓冲区是否用户定义
	bool uesr_define_buffer;					//	只有在用户指定缓冲区时才有效

	//	引擎接口指针
	Engine* pMatlabEngine;		

	//	锁定定向到输出缓冲区成功
	bool lock_outstrbuffer_ok;		
};


//////////////////////////////////////////////////////////////////////////
#endif //	__MATLABENGINE_H__