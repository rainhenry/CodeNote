/*******************************************************************

	工程名称：内存泄漏检测头文件
	工程日期：2017.03.06
	原文出处：http://www.cppblog.com/kylinxh/archive/2008/05/29/51457.html
	版    本：REV 0.1
	修改完善：rainhenry
	编码测试：rainhenry
	适用平台：VS2013 @ CMD

	使用说明：
	只需要在每个源文件中包含该文件即可检测内存泄漏，并且定位泄漏所在的文件及行号
	只能用于控制台程序，不可用在MFC中！！

	测试检测结果举例：
	Detected memory leaks!
	Dumping objects ->
	d:\project\20170306_memtest\20170306_memfail_cmd\20170306_memfail_cmd\user.h(11) : {115} normal block at 0x0025BEC0, 4 bytes long.
	Data: <    > CD CD CD CD
	d:\project\20170306_memtest\20170306_memfail_cmd\20170306_memfail_cmd\20170306_memfail_cmd.cpp(16) : {114} normal block at 0x0025BE78, 7 bytes long.
	Data: <       > CD CD CD CD CD CD CD
	Object dump complete.
	The program '[13752] 20170306_memfail_cmd.exe' has exited with code 0 (0x0).

	使用举例：
	#include "MemLeak.h"

*******************************************************************/

#ifndef __MEMLEAK_H__
#define __MEMLEAK_H__

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifdef _DEBUG
//for memory leak check
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#endif

#include <stdlib.h> 
#include <crtdbg.h>

#ifndef CheckMemoryLeak
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif

class CMemLeak
{
public:
	static bool init_flag;

	CMemLeak();

	~CMemLeak();
};

#endif 

#endif	//	__MEMLEAK_H__

