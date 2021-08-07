/******************************************************************************************

	程序名称：C语言版本的平均滤波处理
	创建日期：2017.04.20
	最后修改：2017.04.20
	程序版本：REV 0.1
	适用平台：MCS-51

******************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////
//	重定义保护
#ifndef __AverageFilter_C_H__
#define __AverageFilter_C_H__

///////////////////////////////////////////////////////////////////////////////////////////
//	定义相关
#define AVGFILTER_DATATYPE				int				//	定义滤波的数据类型
#define AVGFILTER_COUNT					40				//	定义最大存储深度

//	定义滤波处理结构体
typedef struct tagSAvgFilterProc
{
	//	配置参数
	int filter_length;									//	滤波处理的长度
	int without_max_count;								//	忽略的最大值个数
	int without_min_count;								//	忽略的最小值个数
	
	//	缓冲区
	AVGFILTER_DATATYPE buff[AVGFILTER_COUNT];			//	数据缓冲区，处理区间为[0,filter_length-1]，从[0]位置进入数据，到[filter_length-1]位置结束
	
	//	相关结果
	int already_proc_count;								//	已经处理的数据个数
	AVGFILTER_DATATYPE output;							//	经过计算输出的平均值
}SAvgFilterProc;

///////////////////////////////////////////////////////////////////////////////////////////
//	导出函数

	//	滤波处理
	AVGFILTER_DATATYPE AvgFilterProc(
		AVGFILTER_DATATYPE in,					//	输入数据
		SAvgFilterProc* pconfig					//	配置结构
		);

///////////////////////////////////////////////////////////////////////////////////////////
#endif	//	__AverageFilter_C_H__

