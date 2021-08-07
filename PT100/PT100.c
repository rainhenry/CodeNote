#include "PT100.h"
#include <math.h>


//	ͨ������õ��¶�	
//	��ڣ�����ֵ   ��λ ��
//	���ڣ��¶�ֵ   ��λ ��
double GetTempFromRes(double fR)
{				   
    double  fT  , fT0;     
    short   i;          
    fT0 = (fR / 100 - 1) / A;       
    if(fR >= 18.52 && fR < 100) //-200��- 0��     
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
        return fT;			//	����¶�     
    }      
    else if(fR >= 100 && fR <= 390.481)  //0��- 850��     
    {          
        for(i = 0 ; i < 50 ; i ++)         
        {              
            fT = fT0 + (fR - 100*(1 + A*fT0 + B*fT0*fT0)) / (100*(A + 2*B*fT0));             
            if(fabs(fT - fT0) < 0.001)                 
                break;              
            else                  
                fT0 = fT;           
        }          
        return fT;			//	����¶�     
    }      
    else          
        return OVER_TEMP_RETURN;		//	����¶ȳ���  
}

//	ͨ���¶ȵõ�����
//	��ڣ��¶�ֵ		��λ ��
//	���ڣ�����ֵ		��λ ��
double GetResFromTemp(double fT)
{		  
    double fR;         
    if(fT >= -200 && fT < 0)      
    {          
        fR = 100 * (1 + A*fT + B*fT*fT + C*(fT-100)*fT*fT*fT);          
        return fR;				//	�������    
    }      
    else if(fT >= 0 && fT <= 850)     
    {          
        fR = 100 * (1 + A*fT + B*fT*fT);           
        return fR;				//	�������      
    }     
    else          
        return OVER_RES_RETURN;		//	������賬�� 
}
