
//	�������ƣ�����ʱ��ͳ�Ƴ���
//	����汾��REV 0.2

#include "RunTimerCount.h"

#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "CAN1.h"
#include "RTIs1.h"
#include "GPIO1.h"
#include "GPIO2.h"
#include "PE_Types.h"

//	��ǰϵͳ����ʱ�䣬����Ϊstatic�ͣ���ֹ�����ⲿ������ͻ����
static SRunTimerCount_BaseTime now_time;

//----------------------------------------------------------------------------------------------------
//	��Ҫ�û��޸ĵ�
//	�ý�����˳��ٽ�����Ե�ǰϵͳ����ʱ�����static SRunTimerCount_BaseTime now_time;��˵��
//	��Ϊ�ñ������ܻ����жϳ�����д�룬���������ж�ȡ
//	�����ٽ�
void RunTimerCount_EnterCritical(void)
{
	EnterCritical();
}

//	�˳��ٽ�
void RunTimerCount_ExitCritical(void)
{
	ExitCritical();
}

//----------------------------------------------------------------------------------------------------
//	���ܺ���
//	����ʱ���ʽ
void RunTimerCount_MakeTime(SRunTimerCount_BaseTime* ptime)
{
	//	������ʱ����
	int type = 0;				//	�������ͱ�����0=�������ͣ�1=��������

	//	ͳ��˳��Ӿ�ϸʱ�䵽��ʱ�䣬�ȼ����������
	if(ptime->usec >= 1000)	{ptime->msec += (ptime->usec)/1000;	ptime->usec = (ptime->usec) % 1000;}			//	΢��
	if(ptime->msec >= 1000)	{ptime->sec += (ptime->msec)/1000;	ptime->msec = (ptime->msec) % 1000;}			//	����
	if(ptime->sec >= 60)	{ptime->min += (ptime->sec)/60;		ptime->sec = (ptime->sec) % 60;}				//	��
	if(ptime->min >= 60)	{ptime->hour += (ptime->min)/60;	ptime->min = (ptime->min) % 60;}				//	��
	if(ptime->hour >= 24)	{ptime->day += (ptime->hour)/24;	ptime->hour = (ptime->hour) % 24;}				//	Сʱ

	//	���㸺�����
	if(ptime->usec < (-1000))	{ptime->msec += (ptime->usec)/1000;	ptime->usec = ((ptime->usec) % 1000);}			//	΢��
	if(ptime->msec < (-1000))	{ptime->sec += (ptime->msec)/1000;	ptime->msec = ((ptime->msec) % 1000);}			//	����
	if(ptime->sec < (-60))		{ptime->min += (ptime->sec)/60;		ptime->sec = ((ptime->sec) % 60);}				//	��
	if(ptime->min < (-60))		{ptime->hour += (ptime->min)/60;	ptime->min = ((ptime->min) % 60);}				//	��
	if(ptime->hour < (-24))		{ptime->day += (ptime->hour)/24;	ptime->hour = ((ptime->hour) % 24);}			//	Сʱ

	//	��������������Ǹ�
	//	������λ��ʼ���ҵ���˲�Ϊ0��ֵ
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

	//	�������0������ʱ�䣬�ӵ�λ��ʼ���������и���ֵ
	if(type == 0)
	{
		if(ptime->usec < 0)		{ptime->msec -= 1;	ptime->usec = 1000 - ptime->usec;}			//	΢��
		if(ptime->msec < 0)		{ptime->sec  -= 1;	ptime->msec = 1000 - ptime->msec;}			//	����
		if(ptime->sec < 0)		{ptime->min  -= 1;	ptime->sec = 60 - ptime->sec;}				//	��
		if(ptime->min < 0)		{ptime->hour -= 1;	ptime->min = 60 - ptime->min;}				//	��
		if(ptime->hour < 0)		{ptime->day  -= 1;	ptime->hour = 24 - ptime->hour;}			//	Сʱ
		return;																					//	�������
	}
	//	��ʱΪС��0������ʱ�䣬�ӵ�λ��ʼ��������������ֵ
	else
	{
		if(ptime->usec > 0)		{ptime->msec += 1;	ptime->usec = ptime->usec - 1000;}			//	΢��
		if(ptime->msec > 0)		{ptime->sec  += 1;	ptime->msec = ptime->msec - 1000;}			//	����
		if(ptime->sec > 0)		{ptime->min  += 1;	ptime->sec = ptime->sec - 60;}				//	��
		if(ptime->min > 0)		{ptime->hour += 1;	ptime->min = ptime->min - 60;}				//	��
		if(ptime->hour > 0)		{ptime->day  += 1;	ptime->hour = ptime->hour - 24;}			//	Сʱ
		return;																					//	�������
	}
}

//	����һ��ʱ�䳣��������ʱ������
SRunTimerCount_BaseTime RunTimerCount_CreateTime(
	TIME_TYPE	day,				//	��
	TIME_TYPE	hour,				//	Сʱ
	TIME_TYPE	min,				//	����
	TIME_TYPE	sec,				//	��
	TIME_TYPE	msec,				//	����
	TIME_TYPE	usec				//	΢��
	)
{
	//	������ʱʱ�����
	SRunTimerCount_BaseTime tmp_time = {0};

	//	����ʱ��
	tmp_time.day	=	day;
	tmp_time.hour	=	hour;
	tmp_time.min	=	min;
	tmp_time.sec	=	sec;
	tmp_time.msec	=	msec;
	tmp_time.usec	=	usec;

	//	����ʱ��
	return tmp_time;
}

//	����һ��ʱ�䳣������ʱ������
SRunTimerCount_BaseTime RunTimerCount_CreateTimeWithMakeTime(
	TIME_TYPE	day,				//	��
	TIME_TYPE	hour,				//	Сʱ
	TIME_TYPE	min,				//	����
	TIME_TYPE	sec,				//	��
	TIME_TYPE	msec,				//	����
	TIME_TYPE	usec				//	΢��
	)
{
	//	������ʱʱ�����
	SRunTimerCount_BaseTime tmp_time = {0};

	//	����ʱ��
	tmp_time.day	=	day;
	tmp_time.hour	=	hour;
	tmp_time.min	=	min;
	tmp_time.sec	=	sec;
	tmp_time.msec	=	msec;
	tmp_time.usec	=	usec;

	//	����ʱ��
	RunTimerCount_MakeTime(&tmp_time);

	//	����ʱ��
	return tmp_time;
}

//	�ۼ�ʱ�䵽ϵͳ����ʱ��
void RunTimerCount_OnHandle(SRunTimerCount_BaseTime time)
{
	//	������ʱ����
	SRunTimerCount_BaseTime tmp_time;

	//	��ȡ��ǰϵͳʱ��
	RunTimerCount_EnterCritical();
	tmp_time = now_time;
	RunTimerCount_ExitCritical();

	//	�����
	tmp_time.day	+=	time.day;
	tmp_time.hour	+=	time.hour;
	tmp_time.min	+=	time.min;
	tmp_time.sec	+=	time.sec;
	tmp_time.msec	+=	time.msec;
	tmp_time.usec	+=	time.usec;

	//	�˴�ȡ�������ʽ��Ϊ�˽�Լ�жϷ�������еĴ���ʱ��

	//	���µ�ϵͳ
	RunTimerCount_EnterCritical();
	now_time = tmp_time;
	RunTimerCount_ExitCritical();
}

//	��ȡϵͳ��ǰ������ʱ��
SRunTimerCount_BaseTime RunTimerCount_GetNowTime(void)
{
	//	������ʱ����
	SRunTimerCount_BaseTime tmp_time;

	//	��ȡ��ǰϵͳʱ��
	RunTimerCount_EnterCritical();
	tmp_time = now_time;
	RunTimerCount_ExitCritical();

	//	�����ʽ
	RunTimerCount_MakeTime(&tmp_time);

	//	��������д�뵽ϵͳʱ����
	RunTimerCount_EnterCritical();
	now_time = tmp_time;
	RunTimerCount_ExitCritical();

	//	����ʱ������
	return tmp_time;
}

//	�Ƚ�����ʱ���Ƿ���ȣ���ȷ���1������ȷ���0
int RunTimerCount_IsEqual(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	�ֱ������ʽ
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	ֱ�ӷֱ�Ƚ�ÿ��ֵ���������ȵĻ�
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
	//	���κ�һ�鲻��
	else
	{
		return 0;
	}
}

//	�Ƚ�ʱ��a�Ƿ�С��ʱ��b��С�ڷ���1����С�ڷ���0
int RunTimerCount_IsLess(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	������ʱ����
	SRunTimerCount_BaseTime tmp_time;

	//	�ֱ������ʽ
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	�����
	tmp_time.day	=	a_time.day	-	b_time.day;
	tmp_time.hour	=	a_time.hour	-	b_time.hour;
	tmp_time.min	=	a_time.min	-	b_time.min;
	tmp_time.sec	=	a_time.sec	-	b_time.sec;
	tmp_time.msec	=	a_time.msec	-	b_time.msec;
	tmp_time.usec	=	a_time.usec	-	b_time.usec;

	//	�����ʽ
	RunTimerCount_MakeTime(&tmp_time);
	
	//	�ж��Ƿ����κ�һ��С��0����ֵ
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

//	�Ƚ�ʱ��a�Ƿ����ʱ��b�����ڷ���1�������ڷ���0
int RunTimerCount_IsGreat(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	������ʱ����
	SRunTimerCount_BaseTime tmp_time;

	//	�ֱ������ʽ
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	�����
	tmp_time.day	=	b_time.day	-	a_time.day;
	tmp_time.hour	=	b_time.hour	-	a_time.hour;
	tmp_time.min	=	b_time.min	-	a_time.min;
	tmp_time.sec	=	b_time.sec	-	a_time.sec;
	tmp_time.msec	=	b_time.msec	-	a_time.msec;
	tmp_time.usec	=	b_time.usec	-	a_time.usec;

	//	�����ʽ
	RunTimerCount_MakeTime(&tmp_time);

	//	�ж��Ƿ����κ�һ��С��0����ֵ
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

//	����A-B��ʱ���
SRunTimerCount_BaseTime RunTimerCount_CalcSpan(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time)
{
	//	������ʱ����
	SRunTimerCount_BaseTime tmp_time;

	//	�ֱ������ʽ
	RunTimerCount_MakeTime(&a_time);
	RunTimerCount_MakeTime(&b_time);

	//	�����
	tmp_time.day	=	a_time.day	-	b_time.day;
	tmp_time.hour	=	a_time.hour	-	b_time.hour;
	tmp_time.min	=	a_time.min	-	b_time.min;
	tmp_time.sec	=	a_time.sec	-	b_time.sec;
	tmp_time.msec	=	a_time.msec	-	b_time.msec;
	tmp_time.usec	=	a_time.usec	-	b_time.usec;

	//	�����ʽ
	RunTimerCount_MakeTime(&tmp_time);

	//	��������
	return tmp_time;
}

