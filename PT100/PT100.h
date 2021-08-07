
#ifndef __PT100_H__
#define __PT100_H__

//	����ITS-90��ҵ�ò�����ֶȱ�IEC751���޶��档
#define     A       3.9083e-3  
#define     B       -5.775e-7 
#define     C       -4.183e-12 

#define OVER_TEMP_RETURN	999.99
#define OVER_RES_RETURN		999.99

#ifdef __cplusplus
extern "C"
{
#endif
				
	//	ͨ������õ��¶�	
	//	��ڣ�����ֵ   ��λ 0.1��
	//	���ڣ��¶�ֵ   ��λ 0.1��
	double GetTempFromRes(double res);


	
	//	ͨ���¶ȵõ�����
	//	��ڣ��¶�ֵ		��λ ��
	//	���ڣ�����ֵ		��λ ��
	double GetResFromTemp(double fT);



#ifdef __cplusplus
}
#endif

#endif
