
//	程序版本REV0.1

#include "AverageFilter_C.h"


//	滤波处理
AVGFILTER_DATATYPE AvgFilterProc(
	AVGFILTER_DATATYPE in,					//	输入数据
	SAvgFilterProc* pconfig					//	配置结构
	)
{
	//	如果为第一次插入阶段
	if(pconfig->already_proc_count == 0)
	{
		//	插入缓存
		pconfig->buff[0] = in;
		
		//	定义下一次的操作位置
		++(pconfig->already_proc_count);
		
		//	设置输出
		pconfig->output = in;
		
		//	返回数据
		return pconfig->output;
	}
	//	如果为填充阶段
	else if(pconfig->filter_length > pconfig->already_proc_count)
	{
		//	移动数据
		int i = 0;
		for(i=AVGFILTER_COUNT-1;i>0;--i)
		{
			pconfig->buff[i] = pconfig->buff[i-1];
		}
		
		//	插入缓存
		pconfig->buff[0] = in;
		
		//	定义下一次的操作位置
		++(pconfig->already_proc_count);
		
		//	设置输出
		pconfig->output = in;
		for(i=1;i<pconfig->already_proc_count;++i)
		{
			pconfig->output = pconfig->output + pconfig->buff[i];
		}
		pconfig->output = pconfig->output / pconfig->already_proc_count;
		
		//	返回数据
		return pconfig->output;
	}
	//	否则为更新阶段
	else
	{
		//	定义排序数组和排序循环变量
		int j = 0;
		AVGFILTER_DATATYPE sort_buff[AVGFILTER_COUNT];
		
		//	移动数据
		int i = 0;
		for(i=AVGFILTER_COUNT-1;i>0;--i)
		{
			pconfig->buff[i] = pconfig->buff[i-1];
		}
		
		//	插入缓存
		pconfig->buff[0] = in;
		
		//	拷贝内存
		for(i=0;i<AVGFILTER_COUNT;++i)
		{
			sort_buff[i] = pconfig->buff[i];
		}
		
		//	冒泡排序，[0]位置为最大值
		for(i=0;i<AVGFILTER_COUNT-1;++i)
		{
			for(j=0;j<AVGFILTER_COUNT-1;++j)
			{
				//	如果顺序不对，则交换数据
				if(sort_buff[j] < sort_buff[j+1])
				{
					AVGFILTER_DATATYPE tmp_dat;
					tmp_dat = sort_buff[j];
					sort_buff[j] = sort_buff[j+1];
					sort_buff[j+1] = tmp_dat;
				}
			}
		}
		
		//	设置输出
		pconfig->output = 0;
		for(i=pconfig->without_max_count;i<pconfig->already_proc_count - pconfig->without_min_count;++i)
		{
			pconfig->output = pconfig->output + sort_buff[i];
		}
		pconfig->output = pconfig->output / (pconfig->already_proc_count - pconfig->without_max_count - pconfig->without_min_count);
		
		//	返回数据
		return pconfig->output;
	}
}

