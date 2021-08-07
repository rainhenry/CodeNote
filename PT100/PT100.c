#include "PT100.h"
#include <math.h>


//	通过电阻得到温度	
//	入口：电阻值   单位 Ω
//	出口：温度值   单位 ℃
double GetTempFromRes(double fR)
{				   
    double  fT  , fT0;     
    short   i;          
    fT0 = (fR / 100 - 1) / A;       
    if(fR >= 18.52 && fR < 100) //-200℃- 0℃     
    {          
        for(i = 0 ;i < 50; i ++)         
        {              
            fT = fT0 + (fR - 100*(1 + A*fT0 + B*fT0*fT0 - 100*C*fT0*fT0*fT0 +  C*fT0*fT0*fT0*fT0)) /
                   (100 * (A + 2*B*fT0 - 300*C*fT0*fT0 + 4*C*fT0*fT0*fT0));                  
            if(fabs(fT - fT0) < 0.001)                 
                break;              
            else                  
                fT0 = fT;          
        }          
        return fT;			//	输出温度     
    }      
    else if(fR >= 100 && fR <= 390.481)  //0℃- 850℃     
    {          
        for(i = 0 ; i < 50 ; i ++)         
        {              
            fT = fT0 + (fR - 100*(1 + A*fT0 + B*fT0*fT0)) / (100*(A + 2*B*fT0));             
            if(fabs(fT - fT0) < 0.001)                 
                break;              
            else                  
                fT0 = fT;           
        }          
        return fT;			//	输出温度     
    }      
    else          
        return OVER_TEMP_RETURN;		//	输出温度超限  
}

//	通过温度得到电阻
//	入口：温度值		单位 ℃
//	出口：电阻值		单位 Ω
double GetResFromTemp(double fT)
{		  
    double fR;         
    if(fT >= -200 && fT < 0)      
    {          
        fR = 100 * (1 + A*fT + B*fT*fT + C*(fT-100)*fT*fT*fT);          
        return fR;				//	输出电阻    
    }      
    else if(fT >= 0 && fT <= 850)     
    {          
        fR = 100 * (1 + A*fT + B*fT*fT);           
        return fR;				//	输出电阻      
    }     
    else          
        return OVER_RES_RETURN;		//	输出电阻超限 
}
