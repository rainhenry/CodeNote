
#include "stdafx.h"
#include "MemLeak.h"

CMemLeak mCMemLeak;

bool CMemLeak::init_flag = false;

CMemLeak::CMemLeak()
{
	if (init_flag == false)
	{
		init_flag = true;
		CheckMemoryLeak;
	}
}

CMemLeak::~CMemLeak()
{

}

