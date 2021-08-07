/*******************************************************************

	�������ƣ��ڴ�й©���ͷ�ļ�
	�������ڣ�2017.03.06
	ԭ�ĳ�����http://www.cppblog.com/kylinxh/archive/2008/05/29/51457.html
	��    ����REV 0.1
	�޸����ƣ�rainhenry
	������ԣ�rainhenry
	����ƽ̨��VS2013 @ CMD

	ʹ��˵����
	ֻ��Ҫ��ÿ��Դ�ļ��а������ļ����ɼ���ڴ�й©�����Ҷ�λй©���ڵ��ļ����к�
	ֻ�����ڿ���̨���򣬲�������MFC�У���

	���Լ����������
	Detected memory leaks!
	Dumping objects ->
	d:\project\20170306_memtest\20170306_memfail_cmd\20170306_memfail_cmd\user.h(11) : {115} normal block at 0x0025BEC0, 4 bytes long.
	Data: <    > CD CD CD CD
	d:\project\20170306_memtest\20170306_memfail_cmd\20170306_memfail_cmd\20170306_memfail_cmd.cpp(16) : {114} normal block at 0x0025BE78, 7 bytes long.
	Data: <       > CD CD CD CD CD CD CD
	Object dump complete.
	The program '[13752] 20170306_memfail_cmd.exe' has exited with code 0 (0x0).

	ʹ�þ�����
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
#define _CRTDBG_MAP_ALLOC //ʹ���ɵ��ڴ�dump�����ڴ�����ľ������Ϊֹ
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

