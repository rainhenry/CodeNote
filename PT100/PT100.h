
#ifndef __PT100_H__
#define __PT100_H__

//	基于ITS-90工业用铂电阻分度表IEC751的修订版。
#define     A       3.9083e-3  
#define     B       -5.775e-7 
#define     C       -4.183e-12 

#define OVER_TEMP_RETURN	999.99
#define OVER_RES_RETURN		999.99

#ifdef __cplusplus
extern "C"
{
#endif
				
	//	通过电阻得到温度	
	//	入口：电阻值   单位 0.1Ω
	//	出口：温度值   单位 0.1℃
	double GetTempFromRes(double res);


	
	//	通过温度得到电阻
	//	入口：温度值		单位 ℃
	//	出口：电阻值		单位 Ω
	double GetResFromTemp(double fT);



#ifdef __cplusplus
}
#endif

#endif
