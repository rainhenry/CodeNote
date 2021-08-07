
//	程序名称：步进电机驱动程序
//	程序版本：REV 0.1

#include "StepMotor.h"

#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "CAN1.h"
#include "RTIs1.h"
#include "GPIO1.h"
#include "GPIO2.h"
#include "PE_Types.h"

//	当前电机的运行步骤，该运行步骤为0-7，一共8步
static int current_run_step = 0;

//	当前电机的忙状态
static int motor_busy_status = 0;

//	当前电机设置的命令步数
static int set_step_num = 0;

//	当前电机设置的方向，0=待机，1=正向，1=反向
static int set_dir = 0;

//	相位执行顺序定义
static const unsigned char StepMotor_DrvTab[8][4] = 
{	
	//	0	1	2	3
	//	A 	B 	C 	D
	//	A+	A-	B+	B-	
	{	1,	0,	0,	0	},		//	0	sin+
	{	1,	0,	1,	0	},		//	1	
	{	0,	0,	1,	0	},		//	2	cos+
	{	0,	1,	1,	0	},		//	3
	{	0,	1,	0,	0	},		//	4	sin-
	{	0,	1,	0,	1	},		//	5
	{	0,	0,	0,	1	},		//	6	cos-
	{	1,	0,	0,	1	}		//	7
};

//------------------------------------------------------------------
//	需要用户修改的函数定义
//	进入临界
void StepMotor_EnterCritical(void)
{
	EnterCritical();
}

//	退出临界
void StepMotor_ExitCritical(void)
{
	ExitCritical();
}

//	电机A线，一般A线圈+
void StepMotor_LINE_A(int in)
{
	PTU_PTU0 = in;
}

//	电机B线，一般A线圈-
void StepMotor_LINE_B(int in)
{
	PTU_PTU1 = in;
}

//	电机C线，一般	B线圈+
void StepMotor_LINE_C(int in)
{
	PTU_PTU3 = in;
}

//	电机D线，一般	B线圈-
void StepMotor_LINE_D(int in)
{
	PTU_PTU2 = in;
}


//------------------------------------------------------------------
//	内部定义好的函数
//	获取步数
int StepMotor_GetStepNum(void)
{
	//	定义临时变量
	int tmp = 0;

	//	获取数据
	StepMotor_EnterCritical();					//	进入临界
	tmp = set_step_num;
	StepMotor_ExitCritical();					//	退出临界

	//	返回数据
	return tmp;
}

//	设置步数
void StepMotor_SetStepNum(int in)
{
	StepMotor_EnterCritical();					//	进入临界
	set_step_num = in;
	StepMotor_ExitCritical();					//	退出临界
}

//	获取方向
int StepMotor_GetDir(void)
{
	//	定义临时变量
	int tmp = 0;

	//	获取数据
	StepMotor_EnterCritical();					//	进入临界
	tmp = set_dir;
	StepMotor_ExitCritical();					//	退出临界

	//	返回数据
	return tmp;
}

//	设置方向
void StepMotor_SetDir(int in)
{
	StepMotor_EnterCritical();					//	进入临界
	set_dir = in;
	StepMotor_ExitCritical();					//	退出临界
}

//	获取忙，1=忙，0=已准备好
int StepMotor_GetBusy(void)
{
	//	定义临时变量
	int tmp = 0;

	//	获取数据
	StepMotor_EnterCritical();					//	进入临界
	tmp = motor_busy_status;
	StepMotor_ExitCritical();					//	退出临界

	//	返回数据
	return tmp;
}

//	设置忙
void StepMotor_SetBusy(int in)
{
	StepMotor_EnterCritical();					//	进入临界
	motor_busy_status = in;
	StepMotor_ExitCritical();					//	退出临界
}

int dir = 0;
 int step_num = 0;

//	调用执行函数
void StepMotor_OnHandle(void)
{
	//	定义变量
	static int cp = 0;									//	程序计数器
	int TAG = 0;										//	程序标签
	static int delay_cnt = 0;							//	延时计数器
	//static int dir = 0;									//	方向标志，0=待机，1=正向，2=反向
	//static int step_num = 0;							//	设置的步数
	static int last_cp = 0;								//	上一次执行的状态

	//	开始状态机
	//------------------------------------------------------------------------------	0	得到命令
	if((TAG++) == cp)
	{
		//	获得方向命令
		dir = StepMotor_GetDir();

		//	检测命令
		if(dir == 1)			//	正向
		{
			//	获取要执行的步数
			step_num = StepMotor_GetStepNum();			

			//	检测步数，当步数正常时
			if(step_num > 1)	
			{
				cp = 1;					//	跳转到执行正向
				StepMotor_SetBusy(1);	//	设置当前电机忙
			}
			//	不正常的步数
			else
			{
				StepMotor_SetDir(0);						//	清除命令
				StepMotor_SetStepNum(0);					//	清除步数
			}
		}
		else if(dir == 2)		//	反向
		{
			//	获取要执行的步数
			step_num = StepMotor_GetStepNum();			

			//	检测步数，当步数正常时
			if(step_num > 1)	
			{
				cp = 2;					//	跳转到执行反向
				StepMotor_SetBusy(1);	//	设置当前电机忙
			}
			//	不正常的步数
			else
			{
				StepMotor_SetDir(0);						//	清除命令
				StepMotor_SetStepNum(0);					//	清除步数
			}
		}
		else if(dir != 0)		//	检测到错误的命令
		{
			StepMotor_SetDir(0);						//	清除命令
		}
	}
	//------------------------------------------------------------------------------	1	正向
	else if((TAG++) == cp)
	{
		//	根据状态表输出当前状态的电平驱动线
		StepMotor_LINE_A(StepMotor_DrvTab[current_run_step][0]);
		StepMotor_LINE_B(StepMotor_DrvTab[current_run_step][1]);
		StepMotor_LINE_C(StepMotor_DrvTab[current_run_step][2]);
		StepMotor_LINE_D(StepMotor_DrvTab[current_run_step][3]);

		//	设置下一次的动作状态，当本次执行完最后一个状态后，回到状态开始
		if(current_run_step >= 7)
		{
			current_run_step = 0;			//	回到状态开始
		}
		//	在其他不是最后的状态
		else
		{
			++current_run_step;				//	累加状态
		}

		//	统计当前已经执行的步数
		--step_num;

		//	根据当前已经执行过的步数进行统计和跳转，当完成所有步数的时候，重新开始等待新的命令
		if(step_num <= 0)
		{
			cp = 0;
			StepMotor_SetBusy(0);		//	设置电机已经忙完
		}
		//	未完成操作时，继续延时执行操作
		else
		{
			cp = 3;				//	跳转到延时过程
			delay_cnt = 0;		//	清空延时计数器
			last_cp = 1;		//	保存本次状态
		}
	}
	//------------------------------------------------------------------------------	2	反向
	else if((TAG++) == cp)
	{
		//	根据状态表输出当前状态的电平驱动线
		StepMotor_LINE_A(StepMotor_DrvTab[current_run_step][0]);
		StepMotor_LINE_B(StepMotor_DrvTab[current_run_step][1]);
		StepMotor_LINE_C(StepMotor_DrvTab[current_run_step][2]);
		StepMotor_LINE_D(StepMotor_DrvTab[current_run_step][3]);

		//	设置下一次的动作状态，当本次执行完最前一个状态后，回到状态最后
		if(current_run_step <= 0)
		{
			current_run_step = 7;			//	回到状态最后
		}
		//	在其他不是最前的状态
		else
		{
			--current_run_step;				//	减小状态
		}

		//	统计当前已经执行的步数
		--step_num;

		//	根据当前已经执行过的步数进行统计和跳转，当完成所有步数的时候，重新开始等待新的命令
		if(step_num <= 0)
		{
			cp = 0;
			StepMotor_SetBusy(0);		//	设置电机已经忙完
		}
		//	未完成操作时，继续延时执行操作
		else
		{
			cp = 3;				//	跳转到延时过程
			delay_cnt = 0;		//	清空延时计数器
			last_cp = 2;		//	保存本次状态
		}
	}
	//------------------------------------------------------------------------------	3	延时
	else if((TAG++) == cp)
	{
		//	如果超时
		if(delay_cnt >= STEPMOTOR_DELAY)
		{
			delay_cnt = 0;				//	清空延时计数器
			cp = last_cp;				//	返回上一个状态
		}
		//	未超时
		else
		{
			++delay_cnt;				//	统计超时时间
		}
	}
	//------------------------------------------------------------------------------	程序错误
	else
	{
		//	复位状态机
		cp = 0;							//	清空程序计数器
		delay_cnt = 0;					//	清空时间计数器
		dir = 0;						//	清空方向命令
		step_num = 0;					//	清空步数
		last_cp = 0;					//	清空上一次运行位置
	}
}

//	发送控制命令，0=成功，1=失败
int StepMotor_SendCmd(
	int indir,				//	方向，0=待机，1=正向，1=反向
	int stepnum				//	步数
	)
{
	//	检查方向
	if(!((indir == 1) || (indir == 2))) return 1;

	//	检查步数
	if(stepnum <= 0) return 1;

	//	判断当前电机处理是否忙
	if(StepMotor_GetBusy() != 0) return 1;

	//	发送命令
	StepMotor_SetBusy(1);	//	设置当前电机忙
	StepMotor_SetStepNum(stepnum);			//	设置步数
	StepMotor_SetDir(indir);				//	设置方向

	//	设置成功
	return 0;
}

