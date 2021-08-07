
//	����汾REV0.1

#include "AverageFilter_C.h"


//	�˲�����
AVGFILTER_DATATYPE AvgFilterProc(
	AVGFILTER_DATATYPE in,					//	��������
	SAvgFilterProc* pconfig					//	���ýṹ
	)
{
	//	���Ϊ��һ�β���׶�
	if(pconfig->already_proc_count == 0)
	{
		//	���뻺��
		pconfig->buff[0] = in;
		
		//	������һ�εĲ���λ��
		++(pconfig->already_proc_count);
		
		//	�������
		pconfig->output = in;
		
		//	��������
		return pconfig->output;
	}
	//	���Ϊ���׶�
	else if(pconfig->filter_length > pconfig->already_proc_count)
	{
		//	�ƶ�����
		int i = 0;
		for(i=AVGFILTER_COUNT-1;i>0;--i)
		{
			pconfig->buff[i] = pconfig->buff[i-1];
		}
		
		//	���뻺��
		pconfig->buff[0] = in;
		
		//	������һ�εĲ���λ��
		++(pconfig->already_proc_count);
		
		//	�������
		pconfig->output = in;
		for(i=1;i<pconfig->already_proc_count;++i)
		{
			pconfig->output = pconfig->output + pconfig->buff[i];
		}
		pconfig->output = pconfig->output / pconfig->already_proc_count;
		
		//	��������
		return pconfig->output;
	}
	//	����Ϊ���½׶�
	else
	{
		//	�����������������ѭ������
		int j = 0;
		AVGFILTER_DATATYPE sort_buff[AVGFILTER_COUNT];
		
		//	�ƶ�����
		int i = 0;
		for(i=AVGFILTER_COUNT-1;i>0;--i)
		{
			pconfig->buff[i] = pconfig->buff[i-1];
		}
		
		//	���뻺��
		pconfig->buff[0] = in;
		
		//	�����ڴ�
		for(i=0;i<AVGFILTER_COUNT;++i)
		{
			sort_buff[i] = pconfig->buff[i];
		}
		
		//	ð������[0]λ��Ϊ���ֵ
		for(i=0;i<AVGFILTER_COUNT-1;++i)
		{
			for(j=0;j<AVGFILTER_COUNT-1;++j)
			{
				//	���˳�򲻶ԣ��򽻻�����
				if(sort_buff[j] < sort_buff[j+1])
				{
					AVGFILTER_DATATYPE tmp_dat;
					tmp_dat = sort_buff[j];
					sort_buff[j] = sort_buff[j+1];
					sort_buff[j+1] = tmp_dat;
				}
			}
		}
		
		//	�������
		pconfig->output = 0;
		for(i=pconfig->without_max_count;i<pconfig->already_proc_count - pconfig->without_min_count;++i)
		{
			pconfig->output = pconfig->output + sort_buff[i];
		}
		pconfig->output = pconfig->output / (pconfig->already_proc_count - pconfig->without_max_count - pconfig->without_min_count);
		
		//	��������
		return pconfig->output;
	}
}

