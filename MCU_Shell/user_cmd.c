
//  ����汾 REV 0.1

//-----------------------------------------------------------------------------------
//  ����ͷ�ļ�
#include "../MCU_Shell/MCU_Shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------------
//  �û�������ض���

//==================================================
//  ��������
//  ִ�к���
static int UserCmd_test_do(char* cmd_str, int len)
{
    VT100_Send_String("\r\nHello World!!\r\n");
    return 0;
}

//==================================================
//  ����ӷ�
static int UserCmd_add_do(char* cmd_str, int len)
{
    //  ��ȡ��ǰ��������Ĳ�������
    int total_seg_num = ShellCmd_GetSplitTotalNum(cmd_str, len);

    //  ��ȡ�Ƿ�������
    if(total_seg_num != 3)
    {
        VT100_printf("\r\nInput arg num error!! Total num = %d\r\n", total_seg_num);
        return 1;
    }

    //  ��ȡ����a
    int a = 0;
    int tmp_pos = ShellCmd_GetSegPos(cmd_str, len, 1);
    if(tmp_pos < 0) return 1;
    sscanf(cmd_str + tmp_pos, "%d", &a);

    //  ��ȡ����b
    int b = 0;
    tmp_pos = ShellCmd_GetSegPos(cmd_str, len, 2);
    if(tmp_pos < 0) return 1;
    sscanf(cmd_str + tmp_pos, "%d", &b);

    //  ���㲢��ʾ
    VT100_printf("\r\na = %d, b = %d, a+b = %d\r\n", a, b, a+b);

    //  ���
    return 0;
}

//==================================================
//  ����
static int UserCmd_clean_do(char* cmd_str, int len)
{
    VT100_Send_CursorMoveTo(1, 1);   //  ����λ
    VT100_Send_ClearScreen();        //  �������
    return 0;
}

//==================================================
//  ��ӡ���в�����������
static int UserCmd_arg_do(char* cmd_str, int len)
{
    //  ���п�ʼ
    VT100_Send_String("\r\n");

    //  ��ȡ��ǰ��������Ĳ�������
    int total_seg_num = ShellCmd_GetSplitTotalNum(cmd_str, len);

    //  ��ʾ��������ĸ���
    VT100_printf("  You Input total arg num = %d\r\n", total_seg_num);

    //  ѭ����ӡ���еĲ���
    int i=0;
    for(i=0;i<total_seg_num;i++)
    {
    	//  ��ȡ��ǰ�������ַ���λ��
        int tmp_pos = ShellCmd_GetSegPos(cmd_str, len, i);

        //  ��ӡ�ò���
        VT100_printf("  [%d] %s\r\n", i, cmd_str + tmp_pos);
    }

    //  ���
    return 0;
}

//-----------------------------------------------------------------------------------
//  ��ӡָ���Ŀո���Ŀ
static void UserCmd_help_printSpace(int num)
{
    int i=0;
    for(i=0;i<num;i++)
    {
        VT100_Send_String(" ");
    }
}

//  ���ð�������
static int UserCmd_help_do(char* cmd_str, int len)
{
    //  ��ȡ��ǰ���������
    int total_cmd_num = UserCmd_GetCmdTotalNum();

    //  �ҵ����������У������ַ������
    int max_len = 0;
    int i=0;
    for(i=0;i<total_cmd_num;i++)
    {
        //  ��ȡ��ǰ�������Ϣ�ṹ��ָ��
        SUserCmd* ptmp_cmd_info = UserCmd_GetCmdInfo(i);

        //  ���
        if(ptmp_cmd_info == 0) return 1;

        //  ��ȡ�������ַ����ĳ���
        int tmp_len = strlen(ptmp_cmd_info->pCmdStr);

        //  ��ȡ�
        if(tmp_len > max_len) max_len = tmp_len;
    }

    //  ���ü��
    max_len = max_len + 4;

    //  ���п�ʼ
    VT100_Send_String("\r\n");

    //  ��ӡ��������Ͱ�����Ϣ
    for(i=0;i<total_cmd_num;i++)
    {
        //  ��ȡ��ǰ�������Ϣ�ṹ��ָ��
        SUserCmd* ptmp_cmd_info = UserCmd_GetCmdInfo(i);

        //  ���
        if(ptmp_cmd_info == 0) return 1;

        //  ��ȡ�������ַ����ĳ���
        int tmp_len = strlen(ptmp_cmd_info->pCmdStr);

        //  ����ո��������
        int space_num = max_len - tmp_len;

        //  ��ӡǰ׺�ո�
        VT100_Send_String("    ");

        //  ��ӡ�����ַ���
        VT100_Send_String(ptmp_cmd_info->pCmdStr);

        //  ��ӡʣ�����ո�
        UserCmd_help_printSpace(space_num);

        //  ��ӡ�����ַ���
        VT100_Send_String(ptmp_cmd_info->pHelpStr);

        //  ������ǰ����
        VT100_Send_String("\r\n");
    }

    //  ���
    return 0;
}

//-----------------------------------------------------------------------------------
#if CMD_HISTORY_EN
//  ��ӡ��ʷ����
static int UserCmd_history_do(char* cmd_str, int len)
{
    //  ���п�ʼ
    VT100_Send_String("\r\n");

    //  ��ȡ��ʷ��������
    int total_history_num = CmdHistory_GetHistoryNum();
    //USER_UI_printf("total_history_num = %d\r\n", total_history_num);

    //  ��û����ʷ����
    if(total_history_num <= 0)
    {
        VT100_Send_String("    No History Command!!\r\n");
        return 0;
    }

    //  ѭ����ӡ������ʷ����
    int i = 0;
    for(i=0;i<total_history_num;i++)
    {
        VT100_Send_String("    ");
        char* tmp_str = CmdHistory_GetCmdStringByN(total_history_num - 1 - i);
        if(tmp_str == 0)
        {
            VT100_Send_String("CmdHistory_GetCmdStringByN Error!!");
        }
        else
        {
            VT100_Send_String(tmp_str);
        }
        VT100_Send_String("\r\n");
    }

    //  ���
    return 0;
}
#endif

//-----------------------------------------------------------------------------------
//  �����б�
static const SUserCmd UserCmdBuf[] =
{
    //  ִ�к���ָ��           ���ƺ���ָ��        �����ַ���         ������Ϣ�ַ���
    {UserCmd_test_do,        0,                "test",          "This a test command"},
    //{UserCmd_add_do,         0,                "add",           "Calc Add a+b"},   //  ���ڵ�Ƭ����ջ̫С���޷�֧��sscanf�����Ƕ��
    {UserCmd_arg_do,         0,                "arg",           "Test print arg command"},
    {UserCmd_clean_do,       0,                "clean",         "Clean the screen"},
    {UserCmd_help_do,        0,                "help",          "Show this help"},

    #if CMD_HISTORY_EN
    {UserCmd_history_do,     0,                CMD_HISTORY_STR, "Show command history list"},
    #endif
};

//-----------------------------------------------------------------------------------
//  �Ѿ���װ�ĺ���

//  ��ȡ��ǰ���������
int UserCmd_GetCmdTotalNum(void)
{
    return sizeof(UserCmdBuf) / sizeof(SUserCmd);
}

//  ��ȡָ���������Ľṹ��ָ��
//  �ɹ�����ָ�룬ʧ�ܷ���0
//  indexΪָ�������
const SUserCmd* UserCmd_GetCmdInfo(int index)
{
    //  ������
    if(index < 0) return 0;

    //  ��ȡ��ǰ��������
    int total_cmd_num = UserCmd_GetCmdTotalNum();

    //  �����������Ƿ�Ϸ�
    if(index >= total_cmd_num) return 0;

    //  ����ָ��
    return &UserCmdBuf[index];
}
