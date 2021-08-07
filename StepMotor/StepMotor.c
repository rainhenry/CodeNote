
//	�������ƣ����������������
//	����汾��REV 0.1

#include "StepMotor.h"

#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "CAN1.h"
#include "RTIs1.h"
#include "GPIO1.h"
#include "GPIO2.h"
#include "PE_Types.h"

//	��ǰ��������в��裬�����в���Ϊ0-7��һ��8��
static int current_run_step = 0;

//	��ǰ�����æ״̬
static int motor_busy_status = 0;

//	��ǰ������õ������
static int set_step_num = 0;

//	��ǰ������õķ���0=������1=����1=����
static int set_dir = 0;

//	��λִ��˳����
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
//	��Ҫ�û��޸ĵĺ�������
//	�����ٽ�
void StepMotor_EnterCritical(void)
{
	EnterCritical();
}

//	�˳��ٽ�
void StepMotor_ExitCritical(void)
{
	ExitCritical();
}

//	���A�ߣ�һ��A��Ȧ+
void StepMotor_LINE_A(int in)
{
	PTU_PTU0 = in;
}

//	���B�ߣ�һ��A��Ȧ-
void StepMotor_LINE_B(int in)
{
	PTU_PTU1 = in;
}

//	���C�ߣ�һ��	B��Ȧ+
void StepMotor_LINE_C(int in)
{
	PTU_PTU3 = in;
}

//	���D�ߣ�һ��	B��Ȧ-
void StepMotor_LINE_D(int in)
{
	PTU_PTU2 = in;
}


//------------------------------------------------------------------
//	�ڲ�����õĺ���
//	��ȡ����
int StepMotor_GetStepNum(void)
{
	//	������ʱ����
	int tmp = 0;

	//	��ȡ����
	StepMotor_EnterCritical();					//	�����ٽ�
	tmp = set_step_num;
	StepMotor_ExitCritical();					//	�˳��ٽ�

	//	��������
	return tmp;
}

//	���ò���
void StepMotor_SetStepNum(int in)
{
	StepMotor_EnterCritical();					//	�����ٽ�
	set_step_num = in;
	StepMotor_ExitCritical();					//	�˳��ٽ�
}

//	��ȡ����
int StepMotor_GetDir(void)
{
	//	������ʱ����
	int tmp = 0;

	//	��ȡ����
	StepMotor_EnterCritical();					//	�����ٽ�
	tmp = set_dir;
	StepMotor_ExitCritical();					//	�˳��ٽ�

	//	��������
	return tmp;
}

//	���÷���
void StepMotor_SetDir(int in)
{
	StepMotor_EnterCritical();					//	�����ٽ�
	set_dir = in;
	StepMotor_ExitCritical();					//	�˳��ٽ�
}

//	��ȡæ��1=æ��0=��׼����
int StepMotor_GetBusy(void)
{
	//	������ʱ����
	int tmp = 0;

	//	��ȡ����
	StepMotor_EnterCritical();					//	�����ٽ�
	tmp = motor_busy_status;
	StepMotor_ExitCritical();					//	�˳��ٽ�

	//	��������
	return tmp;
}

//	����æ
void StepMotor_SetBusy(int in)
{
	StepMotor_EnterCritical();					//	�����ٽ�
	motor_busy_status = in;
	StepMotor_ExitCritical();					//	�˳��ٽ�
}

int dir = 0;
 int step_num = 0;

//	����ִ�к���
void StepMotor_OnHandle(void)
{
	//	�������
	static int cp = 0;									//	���������
	int TAG = 0;										//	�����ǩ
	static int delay_cnt = 0;							//	��ʱ������
	//static int dir = 0;									//	�����־��0=������1=����2=����
	//static int step_num = 0;							//	���õĲ���
	static int last_cp = 0;								//	��һ��ִ�е�״̬

	//	��ʼ״̬��
	//------------------------------------------------------------------------------	0	�õ�����
	if((TAG++) == cp)
	{
		//	��÷�������
		dir = StepMotor_GetDir();

		//	�������
		if(dir == 1)			//	����
		{
			//	��ȡҪִ�еĲ���
			step_num = StepMotor_GetStepNum();			

			//	��ⲽ��������������ʱ
			if(step_num > 1)	
			{
				cp = 1;					//	��ת��ִ������
				StepMotor_SetBusy(1);	//	���õ�ǰ���æ
			}
			//	�������Ĳ���
			else
			{
				StepMotor_SetDir(0);						//	�������
				StepMotor_SetStepNum(0);					//	�������
			}
		}
		else if(dir == 2)		//	����
		{
			//	��ȡҪִ�еĲ���
			step_num = StepMotor_GetStepNum();			

			//	��ⲽ��������������ʱ
			if(step_num > 1)	
			{
				cp = 2;					//	��ת��ִ�з���
				StepMotor_SetBusy(1);	//	���õ�ǰ���æ
			}
			//	�������Ĳ���
			else
			{
				StepMotor_SetDir(0);						//	�������
				StepMotor_SetStepNum(0);					//	�������
			}
		}
		else if(dir != 0)		//	��⵽���������
		{
			StepMotor_SetDir(0);						//	�������
		}
	}
	//------------------------------------------------------------------------------	1	����
	else if((TAG++) == cp)
	{
		//	����״̬�������ǰ״̬�ĵ�ƽ������
		StepMotor_LINE_A(StepMotor_DrvTab[current_run_step][0]);
		StepMotor_LINE_B(StepMotor_DrvTab[current_run_step][1]);
		StepMotor_LINE_C(StepMotor_DrvTab[current_run_step][2]);
		StepMotor_LINE_D(StepMotor_DrvTab[current_run_step][3]);

		//	������һ�εĶ���״̬��������ִ�������һ��״̬�󣬻ص�״̬��ʼ
		if(current_run_step >= 7)
		{
			current_run_step = 0;			//	�ص�״̬��ʼ
		}
		//	��������������״̬
		else
		{
			++current_run_step;				//	�ۼ�״̬
		}

		//	ͳ�Ƶ�ǰ�Ѿ�ִ�еĲ���
		--step_num;

		//	���ݵ�ǰ�Ѿ�ִ�й��Ĳ�������ͳ�ƺ���ת����������в�����ʱ�����¿�ʼ�ȴ��µ�����
		if(step_num <= 0)
		{
			cp = 0;
			StepMotor_SetBusy(0);		//	���õ���Ѿ�æ��
		}
		//	δ��ɲ���ʱ��������ʱִ�в���
		else
		{
			cp = 3;				//	��ת����ʱ����
			delay_cnt = 0;		//	�����ʱ������
			last_cp = 1;		//	���汾��״̬
		}
	}
	//------------------------------------------------------------------------------	2	����
	else if((TAG++) == cp)
	{
		//	����״̬�������ǰ״̬�ĵ�ƽ������
		StepMotor_LINE_A(StepMotor_DrvTab[current_run_step][0]);
		StepMotor_LINE_B(StepMotor_DrvTab[current_run_step][1]);
		StepMotor_LINE_C(StepMotor_DrvTab[current_run_step][2]);
		StepMotor_LINE_D(StepMotor_DrvTab[current_run_step][3]);

		//	������һ�εĶ���״̬��������ִ������ǰһ��״̬�󣬻ص�״̬���
		if(current_run_step <= 0)
		{
			current_run_step = 7;			//	�ص�״̬���
		}
		//	������������ǰ��״̬
		else
		{
			--current_run_step;				//	��С״̬
		}

		//	ͳ�Ƶ�ǰ�Ѿ�ִ�еĲ���
		--step_num;

		//	���ݵ�ǰ�Ѿ�ִ�й��Ĳ�������ͳ�ƺ���ת����������в�����ʱ�����¿�ʼ�ȴ��µ�����
		if(step_num <= 0)
		{
			cp = 0;
			StepMotor_SetBusy(0);		//	���õ���Ѿ�æ��
		}
		//	δ��ɲ���ʱ��������ʱִ�в���
		else
		{
			cp = 3;				//	��ת����ʱ����
			delay_cnt = 0;		//	�����ʱ������
			last_cp = 2;		//	���汾��״̬
		}
	}
	//------------------------------------------------------------------------------	3	��ʱ
	else if((TAG++) == cp)
	{
		//	�����ʱ
		if(delay_cnt >= STEPMOTOR_DELAY)
		{
			delay_cnt = 0;				//	�����ʱ������
			cp = last_cp;				//	������һ��״̬
		}
		//	δ��ʱ
		else
		{
			++delay_cnt;				//	ͳ�Ƴ�ʱʱ��
		}
	}
	//------------------------------------------------------------------------------	�������
	else
	{
		//	��λ״̬��
		cp = 0;							//	��ճ��������
		delay_cnt = 0;					//	���ʱ�������
		dir = 0;						//	��շ�������
		step_num = 0;					//	��ղ���
		last_cp = 0;					//	�����һ������λ��
	}
}

//	���Ϳ������0=�ɹ���1=ʧ��
int StepMotor_SendCmd(
	int indir,				//	����0=������1=����1=����
	int stepnum				//	����
	)
{
	//	��鷽��
	if(!((indir == 1) || (indir == 2))) return 1;

	//	��鲽��
	if(stepnum <= 0) return 1;

	//	�жϵ�ǰ��������Ƿ�æ
	if(StepMotor_GetBusy() != 0) return 1;

	//	��������
	StepMotor_SetBusy(1);	//	���õ�ǰ���æ
	StepMotor_SetStepNum(stepnum);			//	���ò���
	StepMotor_SetDir(indir);				//	���÷���

	//	���óɹ�
	return 0;
}

