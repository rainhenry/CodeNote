/**********************************************************************
	
	程序名称：运行时间统计程序
	创建日期：20180710
	设计编写：王祥福(rainhenry)
	程序版本：REV 0.2

	设计目的：
		通过中断调用时间累积函数，然后程序会自动累积上电后到现状态的运行时间
	用户可以获取当前的运行时间信息，可以以天、时、分、秒、毫秒、微妙的格式获取当
	前运行时间。同时可以以时间差进行比较，得到两个时间变量的时间差。然后可以定
	义时间差常量。可以对时间差实现==、>=、<=、<、>基本判断函数。
		该程序专门为小型微处理器而设计，全部代码使用C语言编写，兼容8-32bit芯
	片。累积时间时，可以自定义时间常量进行累加操作。

	--------------------------------------------------------------------------------------------------------------
	版本修订
		REV 0.1		20180710		王祥福			创建程序
		REV 0.2		20180711		王祥福			优化中断服务程序所调用的函数的执行效率，将大量的计算转移到主函数中
													以便用于更小时间、速度更慢的硬件平台中使用

	基本使用说明
	--------------------------------------------------------------------------------------------------------------
	使用情况一：
		在中断函数中调用RunTimerCount_OnHandle函数来统计运行时间，然后在主
	函数的主循环中检测已经运行时间的多少，以便按照时间调用任务进行处理，详细
	使用方法如下：
	1、定义一个上一次执行时间的时间变量
	SRunTimerCount_BaseTime last_time = {0};

	2、在进入主循环之前给该变量赋值
	last_time = RunTimerCount_GetNowTime();

	3、在中断处理程序中加入统计时间的代码	
	RunTimerCount_OnHandle(RunTimerCount_CreateTime(0, 0, 0, 0, 1, 0));

	4、检测时间差
	if(
		RunTimerCount_IsGreat(  
			RunTimerCount_CalcSpan(RunTimerCount_GetNowTime(), last_time), 
			RunTimerCount_CreateTimeWithMakeTime(0, 0, 0, 1, 0, 0)
		) //  RunTimerCount_IsGreat
	) //  if
	{
		//	更新执行时间，以便下次计算
		last_time = RunTimerCount_GetNowTime();

		//	执行相关用户代码
		sci_printf("Now Time = %d Day %02d:%02d:%02d.%03d.%03d\r\n",
			last_time.day, last_time.hour, last_time.min, last_time.sec, last_time.msec, last_time.usec
			);
	}
	//	为了使书序更简单，可以使用RunTimerCount_IsTimeOut宏，第4步写法将变为	
	if(RunTimerCount_IsTimeOut(last_time, RunTimerCount_CreateTimeWithMakeTime(0, 0, 0, 1, 0, 0)))
	{
		//	更新执行时间，以便下次计算
		last_time = RunTimerCount_GetNowTime();

		//	执行相关用户代码
		sci_printf("Now Time = %d Day %02d:%02d:%02d.%03d.%03d\r\n",
			last_time.day, last_time.hour, last_time.min, last_time.sec, last_time.msec, last_time.usec
			);
	}

**********************************************************************/
//---------------------------------------------------------------------
//	重定义保护
#ifndef __RUNTIMERCOUNT_H__
#define __RUNTIMERCOUNT_H__

//---------------------------------------------------------------------
//	C++兼容处理
#ifdef __cplusplus
extern "C"
{
#endif

//---------------------------------------------------------------------
//	宏定义与数据类型定义
#define TIME_TYPE					signed int					//	定义单个存储时间的数据类型，必须为有符号类型，要求该数据类型至少为16位，理论可以统计89年

//	用于任务调度的简单写法的宏定义
#define RunTimerCount_IsTimeOut(last_time, const_time)	RunTimerCount_IsGreat(	RunTimerCount_CalcSpan(RunTimerCount_GetNowTime(), last_time), 	const_time)

//	定义时间基本数据类型
typedef struct tagSRunTimerCount_BaseTime
{
	TIME_TYPE	day;				//	天
	TIME_TYPE	hour;				//	小时
	TIME_TYPE	min;				//	分钟
	TIME_TYPE	sec;				//	秒
	TIME_TYPE	msec;				//	毫秒
	TIME_TYPE	usec;				//	微秒
}SRunTimerCount_BaseTime;

//---------------------------------------------------------------------
//	导出函数
//	整理时间格式
void RunTimerCount_MakeTime(SRunTimerCount_BaseTime* ptime);

//	创建一个时间常量，不带时间整理
SRunTimerCount_BaseTime RunTimerCount_CreateTime(
	TIME_TYPE	day,				//	天
	TIME_TYPE	hour,				//	小时
	TIME_TYPE	min,				//	分钟
	TIME_TYPE	sec,				//	秒
	TIME_TYPE	msec,				//	毫秒
	TIME_TYPE	usec				//	微秒
	);

//	创建一个时间常量，带时间整理
SRunTimerCount_BaseTime RunTimerCount_CreateTimeWithMakeTime(
	TIME_TYPE	day,				//	天
	TIME_TYPE	hour,				//	小时
	TIME_TYPE	min,				//	分钟
	TIME_TYPE	sec,				//	秒
	TIME_TYPE	msec,				//	毫秒
	TIME_TYPE	usec				//	微秒
	);

//	累加时间到系统运行时间
void RunTimerCount_OnHandle(SRunTimerCount_BaseTime time);

//	获取系统当前的运行时间
SRunTimerCount_BaseTime RunTimerCount_GetNowTime(void);

//	比较两个时间是否相等，相等返回1，不相等返回0
int RunTimerCount_IsEqual(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//	比较时间a是否小于时间b，小于返回1，不小于返回0
int RunTimerCount_IsLess(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//	比较时间a是否大于时间b，大于返回1，不大于返回0
int RunTimerCount_IsGreat(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//	计算A-B的时间差
SRunTimerCount_BaseTime RunTimerCount_CalcSpan(SRunTimerCount_BaseTime a_time, SRunTimerCount_BaseTime b_time);

//---------------------------------------------------------------------
//	C++兼容处理
#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------
#endif	//	__RUNTIMERCOUNT_H__