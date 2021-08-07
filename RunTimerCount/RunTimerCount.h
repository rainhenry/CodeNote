/**********************************************************************
	
	�������ƣ�����ʱ��ͳ�Ƴ���
	�������ڣ�20180710
	��Ʊ�д�����鸣(rainhenry)
	����汾��REV 0.2

	���Ŀ�ģ�
		ͨ���жϵ���ʱ���ۻ�������Ȼ�������Զ��ۻ��ϵ����״̬������ʱ��
	�û����Ի�ȡ��ǰ������ʱ����Ϣ���������졢ʱ���֡��롢���롢΢��ĸ�ʽ��ȡ��
	ǰ����ʱ�䡣ͬʱ������ʱ�����бȽϣ��õ�����ʱ�������ʱ��Ȼ����Զ�
	��ʱ���������Զ�ʱ���ʵ��==��>=��<=��<��>�����жϺ�����
		�ó���ר��ΪС��΢����������ƣ�ȫ������ʹ��C���Ա�д������8-32bitо
	Ƭ���ۻ�ʱ��ʱ�������Զ���ʱ�䳣�������ۼӲ�����

	--------------------------------------------------------------------------------------------------------------
	�汾�޶�
		REV 0.1		20180710		���鸣			��������
		REV 0.2		20180711		���鸣			�Ż��жϷ�����������õĺ�����ִ��Ч�ʣ��������ļ���ת�Ƶ���������
													�Ա����ڸ�Сʱ�䡢�ٶȸ�����Ӳ��ƽ̨��ʹ��

	����ʹ��˵��
	--------------------------------------------------------------------------------------------------------------
	ʹ�����һ��
		���жϺ����е���RunTimerCount_OnHandle������ͳ������ʱ�䣬Ȼ������
	��������ѭ���м���Ѿ�����ʱ��Ķ��٣��Ա㰴��ʱ�����������д�����ϸ
	ʹ�÷������£�
	1������һ����һ��ִ��ʱ���ʱ�����
	SRunTimerCount_BaseTime last_time = {0};

	2���ڽ�����ѭ��֮ǰ���ñ�����ֵ
	last_time = RunTimerCount_GetNowTime();

	3�����жϴ�������м���ͳ��ʱ��Ĵ���	
	RunTimerCount_OnHandle(RunTimerCount_CreateTime(0, 0, 0, 0, 1, 0));

	4�����ʱ���
	if(
		RunTimerCount_IsGreat(  
			RunTimerCount_CalcSpan(RunTimerCount_GetNowTime(), last_time), 
			RunTimerCount_CreateTimeWithMakeTime(0, 0, 0, 1, 0, 0)
		) //  RunTimerCount_IsGreat
	) //  if
	{
		//	����ִ��ʱ�䣬�Ա��´μ���
		last_time = RunTimerCount_GetNowTime();

		//	ִ������û�����
		sci_printf("Now Time = %d Day %02d:%02d:%02d.%03d.%03d\r\n",
			last_time.day, last_time.hour, last_time.min, last_time.sec, last_time.msec, last_time.usec
			);
	}
	//	Ϊ��ʹ������򵥣�����ʹ��RunTimerCount_IsTimeOut�꣬��4��д������Ϊ	
	if(RunTimerCount_IsTimeOut(last_time, RunTimerCount_CreateTimeWithMakeTime(0, 0, 0, 1, 0, 0)))
	{
		//	����ִ��ʱ�䣬�Ա��´μ���
		last_time = RunTimerCount_GetNowTime();

		//	ִ������û�����
		sci_printf("Now Time = %d Day %02d:%02d:%02d.%03d.%03d\r\n",
			last_time.day, last_time.hour, last_time.min, last_time.sec, last_time.msec, last_time.usec
			);
	}

**********************************************************************/
//---------------------------------------------------------------------
//	�ض��屣��
#ifndef __RUNTIMERCOUNT_H__
#define __RUNTIMERCOUNT_H__

//---------------------------------------------------------------------
//	C++���ݴ���
#ifdef __cplusplus
extern "C"
{
#endif

//---------------------------------------------------------------------
//	�궨�����������Ͷ���
#define TIME_TYPE					signed int					//	���嵥���洢ʱ����������ͣ�����Ϊ�з������ͣ�Ҫ���������������Ϊ16λ�����ۿ���ͳ��89��

//	����������ȵļ�д���ĺ궨��
#define RunTimerCount_IsTimeOut(last_time, const_time)	RunTimerCount_IsGreat(	RunTimerCount_CalcSpan(RunTimerCount_GetNowTime(), last_time), 	const_time)

//	����ʱ�������������
typedef struct tagSRunTimerCount_BaseTime
{
	TIME_TYPE	day;				//	��
	TIME_TYPE	hour;				//	Сʱ
	TIME_TYPE	min;				//	����
	TIME_TYPE	sec;				//	��
	TIME_TYPE	msec;				//	����
	TIME_TYPE	usec;				//	΢��
}SRunTimerCount_BaseTime;

//---------------------------------------------------------------------
//	��������
//	����ʱ���ʽ
void RunTimerCount_MakeTime(SRunTimerCount_BaseTime* ptime);

//	����һ��ʱ�䳣��������ʱ������
SRunTimerCount_BaseTime RunTimerCount_CreateTime(
	TIME_TYPE	day,				//	��
	TIME_TYPE	hour,				//	Сʱ
	TIME_TYPE	min,				//	����
	TIME_TYPE	sec,				//	��
	TIME_TYPE	msec,				//	����
	TIME_TYPE	usec				//	΢��
	);

//	����һ��ʱ�䳣������ʱ������
SRunTimerCount_BaseTime RunTimerCount_CreateTimeWithMakeTime(
	TIME_TYPE	day,				//	��
	TIME_TYPE	hour,				//	Сʱ
	TIME_TYPE	min,				//	����
	TIME_TYPE	sec,				//	��
	TIME_TYPE	msec,				//	����
	TIME_TYPE	usec				//	΢��
	);

//	�ۼ�ʱ�䵽ϵͳ����ʱ��
void RunTimerCount_OnHandle(SRunTimerCount_BaseTime time);

//	��ȡϵͳ��ǰ������ʱ��
SRunTimerCount_BaseTime RunTimerCount_GetNowTime(void);

//	�Ƚ�����ʱ���Ƿ���ȣ���ȷ���1������ȷ���0
int RunTimerCount_IsEqual(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//	�Ƚ�ʱ��a�Ƿ�С��ʱ��b��С�ڷ���1����С�ڷ���0
int RunTimerCount_IsLess(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//	�Ƚ�ʱ��a�Ƿ����ʱ��b�����ڷ���1�������ڷ���0
int RunTimerCount_IsGreat(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//	����A-B��ʱ���
SRunTimerCount_BaseTime RunTimerCount_CalcSpan(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//---------------------------------------------------------------------
//	C++���ݴ���
#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------
#endif	//	__RUNTIMERCOUNT_H__