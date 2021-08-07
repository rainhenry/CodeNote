
//	程序名称：运行时间统计程序
//	程序版本：REV 0.2

#include "RunTimerCount.h"

#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "CAN1.h"
#include "RTIs1.h"
#include "GPIO1.h"
#include "GPIO2.h"
#include "PE_Types.h"

//	当前系统运行时间，定义为static型，防止其他外部变量冲突访问
static SRunTimerCount_BaseTime now_time;

//----------------------------------------------------------------------------------------------------
//	需要用户修改的
//	该进入和退出临界是针对当前系统运行时间变量static SRunTimerCount_BaseTime now_time;来说的
//	因为该变量可能会在中断程序中写入，在主程序中读取
//	进入临界
void RunTimerCount_EnterCritical(void)
{
	EnterCritical();
}

//	退出临界
void RunTimerCount_ExitCritical(void)
{
	ExitCritical();
}

//----------------------------------------------------------------------------------------------------
//	功能函数
//	整理时间格式
void RunTimerCount_MakeTime(SRunTimerCount_BaseTime* ptime)
{
	//	定义临时变量
	int type = 0;				//	符号类型变量，0=正数类型，1=负数类型

	//	统计顺序从精细时间到粗时间，先计算正数溢出
	if(ptime->usec >= 1000)	{ptime->msec += (ptime->usec)/1000;	ptime->usec = (ptime->usec) % 1000;}			//	微秒
	if(ptime->msec >= 1000)	{ptime->sec += (ptime->msec)/1000;	ptime->msec = (ptime->msec) % 1000;}			//	毫秒
	if(ptime->sec >= 60)	{ptime->min += (ptime->sec)/60;		ptime->sec = (ptime->sec) % 60;}				//	秒
	if(ptime->min >= 60)	{ptime->hour += (ptime->min)/60;	ptime->min = (ptime->min) % 60;}				//	分
	if(ptime->hour >= 24)	{ptime->day += (ptime->hour)/24;	ptime->hour = (ptime->hour) % 24;}				//	小时

	//	计算负数溢出
	if(ptime->usec < (-1000))	{ptime->msec += (ptime->usec)/1000;	ptime->usec = ((ptime->usec) % 1000);}			//	微秒
	if(ptime->msec < (-1000))	{ptime->sec += (ptime->msec)/1000;	ptime->msec = ((ptime->msec) % 1000);}			//	毫秒
	if(ptime->sec < (-60))		{ptime->min += (ptime->sec)/60;		ptime->sec = ((ptime->sec) % 60);}				//	秒
	if(ptime->min < (-60))		{ptime->hour += (ptime->min)/60;	ptime->min = ((ptime->min) % 60);}				//	分
	if(ptime->hour < (-24))		{ptime->day += (ptime->hour)/24;	ptime->hour = ((ptime->hour) % 24);}			//	小时

	//	检测整体是正还是负
	//	从最大的位开始，找到最顶端不为0的值
	do
	{
		if(ptime->day > 0)	{type = 0;	break;}		else if(ptime->day < 0)		{type = 1;	break;}
		if(ptime->hour > 0)	{type = 0;	break;}		else if(ptime->hour < 0)	{type = 1;	break;}
		if(ptime->min > 0)	{type = 0;	break;}		else if(ptime->min < 0)		{type = 1;	break;}
		if(ptime->sec > 0)	{type = 0;	break;}		else if(ptime->sec < 0)		{type = 1;	break;}
		if(ptime->msec > 0)	{type = 0;	break;}		else if(ptime->msec < 0)	{type = 1;	break;}
		if(ptime->usec > 0)	{type = 0;	break;}		else if(ptime->usec < 0)	{type = 1;	break;}
	}
	while(0);

	//	如果大于0的整体时间，从低位开始，不允许有负数值
	if(type == 0)
	{
		if(ptime->usec < 0)		{ptime->msec -= 1;	ptime->usec = 1000 - ptime->usec;}			//	微秒
		if(ptime->msec < 0)		{ptime->sec  -= 1;	ptime->msec = 1000 - ptime->msec;}			//	毫秒
		if(ptime->sec < 0)		{ptime->min  -= 1;	ptime->sec = 60 - ptime->sec;}				//	秒
		if(ptime->min < 0)		{ptime->hour -= 1;	ptime->min = 60 - ptime->min;}				//	分
		if(ptime->hour < 0)		{ptime->day  -= 1;	ptime->hour = 24 - ptime->hour;}			//	小时
		return;																					//	计算完成
	}
	//	此时为小于0的整体时间，从低位开始，不允许有正数值
	else
	{
		if(ptime->usec > 0)		{ptime->msec += 1;	ptime->usec = ptime->usec - 1000;}			//	微秒
		if(ptime->msec > 0)		{ptime->sec  += 1;	ptime->msec = ptime->msec - 1000;}			//	毫秒
		if(ptime->sec > 0)		{ptime->min  += 1;	ptime->sec = ptime->sec - 60;}				//	秒
		if(ptime->min > 0)		{ptime->hour += 1;	ptime->min = ptime->min - 60;}				//	分
		if(ptime->hour > 0)		{ptime->day  += 1;	ptime->hour = ptime->hour - 24;}			//	小时
		return;																					//	计算完成
	}
}

//	创建一个时间常量，不带时间整理
SRunTimerCount_BaseTime RunTimerCount_CreateTime(
	TIME_TYPE	day,				//	天
	TIME_TYPE	hour,				//	小时
	TIME_TYPE	min,				//	分钟
	TIME_TYPE	sec,				//	秒
	TIME_TYPE	msec,				//	毫秒
	TIME_TYPE	usec				//	微秒
	)
{
	//	定义临时时间变量
	SRunTimerCount_BaseTime tmp_time = {0};

	//	设置时间
	tmp_time.day	=	day;
	tmp_time.hour	=	hour;
	tmp_time.min	=	min;
	tmp_time.sec	=	sec;
	tmp_time.msec	=	msec;
	tmp_time.usec	=	usec;

	//	返回时间
	return tmp_time;
}

//	创建一个时间常量，带时间整理
SRunTimerCount_BaseTime RunTimerCount_CreateTimeWithMakeTime(
	TIME_TYPE	day,				//	天
	TIME_TYPE	hour,				//	小时
	TIME_TYPE	min,				//	分钟
	TIME_TYPE	sec,				//	秒
	TIME_TYPE	msec,				//	毫秒
	TIME_TYPE	usec				//	微秒
	)
{
	//	定义临时时间变量
	SRunTimerCount_BaseTime tmp_time = {0};

	//	设置时间
	tmp_time.day	=	day;
	tmp_time.hour	=	hour;
	tmp_time.min	=	min;
	tmp_time.sec	=	sec;
	tmp_time.msec	=	msec;
	tmp_time.usec	=	usec;

	//	整理时间
	RunTimerCount_MakeTime(&tmp_time);

	//	返回时间
	return tmp_time;
}

//	累加时间到系统运行时间
void RunTimerCount_OnHandle(SRunTimerCount_BaseTime time)
{
	//	定义临时变量
	SRunTimerCount_BaseTime tmp_time;

	//	获取当前系统时间
	RunTimerCount_EnterCritical();
	tmp_time = now_time;
	RunTimerCount_ExitCritical();

	//	简单相加
	tmp_time.day	+=	time.day;
	tmp_time.hour	+=	time.hour;
	tmp_time.min	+=	time.min;
	tmp_time.sec	+=	time.sec;
	tmp_time.msec	+=	time.msec;
	tmp_time.usec	+=	time.usec;

	//	此处取消整理格式，为了节约中断服务程序中的处理时间

	//	更新到系统
	RunTimerCount_EnterCritical();
	now_time = tmp_time;
	RunTimerCount_ExitCritical();
}

//	获取系统当前的运行时间
SRunTimerCount_BaseTime RunTimerCount_GetNowTime(void)
{
	//	定义临时变量
	SRunTimerCount_BaseTime tmp_time;

	//	获取当前系统时间
	RunTimerCount_EnterCritical();
	tmp_time = now_time;
	RunTimerCount_ExitCritical();

	//	整理格式
	RunTimerCount_MakeTime(&tmp_time);

	//	将整理结果写入到系统时间内
	RunTimerCount_EnterCritical();
	now_time = tmp_time;
	RunTimerCount_ExitCritical();

	//	返回时间数据
	return tmp_time;
}

//	比较两个时间是否相等，相等返回1，不相等返回0
int RunTimerCount_IsEqual(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	分别整理格式
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	直接分别比较每个值，如果都相等的话
	if(	(a_time.day		==	b_time.day)		&&
		(a_time.hour	==	b_time.hour)	&&
		(a_time.min		==	b_time.min)		&&
		(a_time.sec		==	b_time.sec)		&&
		(a_time.msec	==	b_time.msec)	&&
		(a_time.usec	==	b_time.usec)	
		)
	{
		return 1;
	}
	//	有任何一组不等
	else
	{
		return 0;
	}
}

//	比较时间a是否小于时间b，小于返回1，不小于返回0
int RunTimerCount_IsLess(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	定义临时变量
	SRunTimerCount_BaseTime tmp_time;

	//	分别整理格式
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	简单相减
	tmp_time.day	=	a_time.day	-	b_time.day;
	tmp_time.hour	=	a_time.hour	-	b_time.hour;
	tmp_time.min	=	a_time.min	-	b_time.min;
	tmp_time.sec	=	a_time.sec	-	b_time.sec;
	tmp_time.msec	=	a_time.msec	-	b_time.msec;
	tmp_time.usec	=	a_time.usec	-	b_time.usec;

	//	整理格式
	RunTimerCount_MakeTime(&tmp_time);
	
	//	判断是否有任何一个小于0的数值
	if(	(tmp_time.day	<	0)	||
		(tmp_time.hour	<	0)	||
		(tmp_time.min	<	0)	||
		(tmp_time.sec	<	0)	||
		(tmp_time.msec	<	0)	||
		(tmp_time.usec	<	0)	
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//	比较时间a是否大于时间b，大于返回1，不大于返回0
int RunTimerCount_IsGreat(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	定义临时变量
	SRunTimerCount_BaseTime tmp_time;

	//	分别整理格式
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	简单相减
	tmp_time.day	=	b_time.day	-	a_time.day;
	tmp_time.hour	=	b_time.hour	-	a_time.hour;
	tmp_time.min	=	b_time.min	-	a_time.min;
	tmp_time.sec	=	b_time.sec	-	a_time.sec;
	tmp_time.msec	=	b_time.msec	-	a_time.msec;
	tmp_time.usec	=	b_time.usec	-	a_time.usec;

	//	整理格式
	RunTimerCount_MakeTime(&tmp_time);

	//	判断是否有任何一个小于0的数值
	if(	(tmp_time.day	<	0)	||
		(tmp_time.hour	<	0)	||
		(tmp_time.min	<	0)	||
		(tmp_time.sec	<	0)	||
		(tmp_time.msec	<	0)	||
		(tmp_time.usec	<	0)	
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//	计算A-B的时间差
SRunTimerCount_BaseTime RunTimerCount_CalcSpan(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	定义临时变量
	SRunTimerCount_BaseTime tmp_time;

	//	分别整理格式
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	简单相减
	tmp_time.day	=	a_time.day	-	b_time.day;
	tmp_time.hour	=	a_time.hour	-	b_time.hour;
	tmp_time.min	=	a_time.min	-	b_time.min;
	tmp_time.sec	=	a_time.sec	-	b_time.sec;
	tmp_time.msec	=	a_time.msec	-	b_time.msec;
	tmp_time.usec	=	a_time.usec	-	b_time.usec;

	//	整理格式
	RunTimerCount_MakeTime(&tmp_time);

	//	返回数据
	return tmp_time;
}

