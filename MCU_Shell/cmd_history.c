
//  ����汾 REV 0.1

//-----------------------------------------------------------------------------------
//  ����ͷ�ļ�
#include "../MCU_Shell/MCU_Shell.h"
#include <string.h>


//-----------------------------------------------------------------------------------
//  �Ƿ�����ʷ�����
#if CMD_HISTORY_EN

//  �������������λ��
static int CmdHistory_BeginPos = 0;   //  ��ʼλ��
static int CmdHistory_EndPos = 0;     //  ����λ��

//  ������ʷ�����
static char CmdHistory_CmdStringBuffer[SHELL_CMD_MAX_CHAR * CMD_HISTORY_NUM_MAX];

//  ��ǰ��ʷ�����λ��  0=��������ģʽ  ����0��ʾ�ع˵���ʷ������
static int CmdHistory_HistoryPos = 0;

//  ��λ������ʷ���
void CmdHistory_Init(void)
{
    //  ��λλ��
    CmdHistory_BeginPos = 0;
    CmdHistory_EndPos = 0;

    //  ��ջ���
    memset(CmdHistory_CmdStringBuffer, 0, sizeof(CmdHistory_CmdStringBuffer));

    //  ��λ��ʷλ��
    CmdHistory_HistoryPos = 0;
}

//  ��ʷ����  1=���óɹ�  0=����ʧ��
int CmdHistory_HistoryUp(void)
{
    //  ��ȡ��ǰ�Ѿ����ڵ��������
    int already_cmd_num = CmdHistory_GetHistoryNum();

    //  ����������ʷ
    if(already_cmd_num <= 0) return 0;     //  ����ʧ��

    //  ��鵱ǰ�Ƿ��Ѿ��ﵽ��ʷ�Ķ���
    if(CmdHistory_HistoryPos >= already_cmd_num) return 0;   //  ����ʧ��

    //  ͳ��λ��
    CmdHistory_HistoryPos++;

    //  �����ɹ�
    return 1;
}

//  ��ʷ����  1=���óɹ�  0=����ʧ��
int CmdHistory_HistoryDown(void)
{
    //  ��鵱ǰ�Ƿ��Ѿ����˵�����
    if(CmdHistory_HistoryPos <= 0) return 0;   //  ����ʧ��

    //  ͳ��λ��
    CmdHistory_HistoryPos--;

    //  �����ɹ�
    return 1;
}

//  �����ʷλ�ã����ڻس�����
void CmdHistory_CleanHistoryPos(void)
{
    CmdHistory_HistoryPos = 0;
}

//  ��ȡ��ǰ��ʷ������  �ɹ�������ʷ�����ַ����׵�ַ�� ʧ�ܷ���0ָ��
char* CmdHistory_GetHistoryCurStr(void)
{
    //  ����ǰ��ʷλ��Ϊ����
    if(CmdHistory_HistoryPos <= 0) return 0;

    //  ��ȡ�ַ���
    return CmdHistory_GetCmdStringByN(CmdHistory_HistoryPos - 1);
}

//  ��ȡ��ǰ���ڵ���ʷ��������
int CmdHistory_GetHistoryNum(void)
{
    //  �����
    if(CmdHistory_BeginPos == CmdHistory_EndPos)
    {
        return 0;    //  û������
    }
    //  ������
    else if(CmdHistory_EndPos > CmdHistory_BeginPos)
    {
        return CmdHistory_EndPos - CmdHistory_BeginPos;   //  ���ؼ��������
    }
    //  ������
    else
    {
        return (CMD_HISTORY_NUM_MAX - CmdHistory_BeginPos) + CmdHistory_EndPos;
    }
}

//  ����һ������
void CmdHistory_InsertNewCmd(char* cmd_str)
{
    //  ������
    if(cmd_str == 0) return;

    //  ��ȡ��ǰ��������ĳ���
    int cmd_len = strlen(cmd_str);

    //  ��鳤��
    if(cmd_len >= SHELL_CMD_MAX_CHAR) return;
    if(cmd_len <= 0) return;

    //  ��ȡ��ǰ�Ѿ����ڵ��������
    int already_cmd_num = CmdHistory_GetHistoryNum();

    //  ����Ƿ�Ϊ�������  ����  �������б�
    if(already_cmd_num <= 0)
    {
        //  ���λ��
        memset(CmdHistory_CmdStringBuffer + 0, 0, SHELL_CMD_MAX_CHAR);

        //  ִ�в���
        strcpy(CmdHistory_CmdStringBuffer + 0, cmd_str);

        //  ͳ��
        if(CmdHistory_EndPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_EndPos = 0;
        else                                               CmdHistory_EndPos = CmdHistory_EndPos + 1;
    }
    //  ���� ����������
    else if(already_cmd_num < (CMD_HISTORY_NUM_MAX - 1))
    {
        //  ������һ��λ��
        int last_cmd_pos = 0;
        if(CmdHistory_EndPos <= 0) last_cmd_pos = CMD_HISTORY_NUM_MAX - 1;
        else                       last_cmd_pos = CmdHistory_EndPos - 1;

        //  �Աȱ����������һ�������Ƿ�Ϊͬһ������ͬ����
        if(strcmp(CmdHistory_CmdStringBuffer + (last_cmd_pos * SHELL_CMD_MAX_CHAR), cmd_str) == 0) return;

        //  ���λ��
        memset(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), 0, SHELL_CMD_MAX_CHAR);

        //  ִ�в���
        strcpy(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), cmd_str);

        //  ͳ��
        if(CmdHistory_EndPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_EndPos = 0;
        else                                               CmdHistory_EndPos = CmdHistory_EndPos + 1;
    }
    //  �������
    else
    {
        //  ������һ��λ��
        int last_cmd_pos = 0;
        if(CmdHistory_EndPos <= 0) last_cmd_pos = CMD_HISTORY_NUM_MAX - 1;
        else                       last_cmd_pos = CmdHistory_EndPos - 1;

        //  �Աȱ����������һ�������Ƿ�Ϊͬһ������ͬ����
        if(strcmp(CmdHistory_CmdStringBuffer + (last_cmd_pos * SHELL_CMD_MAX_CHAR), cmd_str) == 0) return;

        //  ���λ��
        memset(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), 0, SHELL_CMD_MAX_CHAR);

        //  ִ�в���
        strcpy(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), cmd_str);

        //  ͳ��
        if(CmdHistory_EndPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_EndPos = 0;
        else                                               CmdHistory_EndPos = CmdHistory_EndPos + 1;

        //  ɾ�������ϵ�����
        if(CmdHistory_BeginPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_BeginPos = 0;
        else                                                 CmdHistory_BeginPos = CmdHistory_BeginPos + 1;
    }

    //USER_UI_printf("a = %d, b = %d, e = %d\r\n", already_cmd_num, CmdHistory_BeginPos, CmdHistory_EndPos);
}

//  ��ȡ������n��������ַ�����ַָ��
//  �ɹ������ַ���ָ�룬ʧ�ܷ���0ָ��
//  �����n��ʾʱ��˳�������һ����0����
char* CmdHistory_GetCmdStringByN(int n)
{
    //  ��ȡ��ǰ�Ѿ����ڵ��������
    int already_cmd_num = CmdHistory_GetHistoryNum();

    //  ���Ϊ�������б�
    if(already_cmd_num <= 0) return 0;

    //  ������n
    if(n > (already_cmd_num - 1)) return 0;

    //  ���Լ���ָ����λ��
    int tmp_pos = CmdHistory_EndPos;
    int i=0;
    for(i=0;i<(n+1);i++)
    {
        if(tmp_pos <= 0) tmp_pos = CMD_HISTORY_NUM_MAX - 1;
        else             tmp_pos = tmp_pos - 1;
    }

    //  �õ�ָ���������ַ������׵�ַ
    return CmdHistory_CmdStringBuffer + (tmp_pos * SHELL_CMD_MAX_CHAR);
}

//-----------------------------------------------------------------------------------
#endif  //  CMD_HISTORY_EN
