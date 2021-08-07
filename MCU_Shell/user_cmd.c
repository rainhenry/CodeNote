
//  程序版本 REV 0.1

//-----------------------------------------------------------------------------------
//  包含头文件
#include "../MCU_Shell/MCU_Shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------------
//  用户命令相关定义

//==================================================
//  测试命令
//  执行函数
static int UserCmd_test_do(char* cmd_str, int len)
{
    VT100_Send_String("\r\nHello World!!\r\n");
    return 0;
}

//==================================================
//  计算加法
static int UserCmd_add_do(char* cmd_str, int len)
{
    //  获取当前输入命令的参数个数
    int total_seg_num = ShellCmd_GetSplitTotalNum(cmd_str, len);

    //  获取非法的总数
    if(total_seg_num != 3)
    {
        VT100_printf("\r\nInput arg num error!! Total num = %d\r\n", total_seg_num);
        return 1;
    }

    //  获取加数a
    int a = 0;
    int tmp_pos = ShellCmd_GetSegPos(cmd_str, len, 1);
    if(tmp_pos < 0) return 1;
    sscanf(cmd_str + tmp_pos, "%d", &a);

    //  获取加数b
    int b = 0;
    tmp_pos = ShellCmd_GetSegPos(cmd_str, len, 2);
    if(tmp_pos < 0) return 1;
    sscanf(cmd_str + tmp_pos, "%d", &b);

    //  计算并提示
    VT100_printf("\r\na = %d, b = %d, a+b = %d\r\n", a, b, a+b);

    //  完成
    return 0;
}

//==================================================
//  清屏
static int UserCmd_clean_do(char* cmd_str, int len)
{
    VT100_Send_CursorMoveTo(1, 1);   //  光标归位
    VT100_Send_ClearScreen();        //  清除内容
    return 0;
}

//==================================================
//  打印所有参数测试命令
static int UserCmd_arg_do(char* cmd_str, int len)
{
    //  换行开始
    VT100_Send_String("\r\n");

    //  获取当前输入命令的参数个数
    int total_seg_num = ShellCmd_GetSplitTotalNum(cmd_str, len);

    //  提示输入参数的个数
    VT100_printf("  You Input total arg num = %d\r\n", total_seg_num);

    //  循环打印所有的参数
    int i=0;
    for(i=0;i<total_seg_num;i++)
    {
    	//  获取当前参数的字符串位置
        int tmp_pos = ShellCmd_GetSegPos(cmd_str, len, i);

        //  打印该参数
        VT100_printf("  [%d] %s\r\n", i, cmd_str + tmp_pos);
    }

    //  完成
    return 0;
}

//-----------------------------------------------------------------------------------
//  打印指定的空格数目
static void UserCmd_help_printSpace(int num)
{
    int i=0;
    for(i=0;i<num;i++)
    {
        VT100_Send_String(" ");
    }
}

//  内置帮助命令
static int UserCmd_help_do(char* cmd_str, int len)
{
    //  获取当前命令的总数
    int total_cmd_num = UserCmd_GetCmdTotalNum();

    //  找到所有命令中，命令字符串最长的
    int max_len = 0;
    int i=0;
    for(i=0;i<total_cmd_num;i++)
    {
        //  获取当前命令的信息结构体指针
        SUserCmd* ptmp_cmd_info = UserCmd_GetCmdInfo(i);

        //  检查
        if(ptmp_cmd_info == 0) return 1;

        //  获取该命令字符串的长度
        int tmp_len = strlen(ptmp_cmd_info->pCmdStr);

        //  获取最长
        if(tmp_len > max_len) max_len = tmp_len;
    }

    //  设置间隔
    max_len = max_len + 4;

    //  换行开始
    VT100_Send_String("\r\n");

    //  打印所有命令和帮助信息
    for(i=0;i<total_cmd_num;i++)
    {
        //  获取当前命令的信息结构体指针
        SUserCmd* ptmp_cmd_info = UserCmd_GetCmdInfo(i);

        //  检查
        if(ptmp_cmd_info == 0) return 1;

        //  获取该命令字符串的长度
        int tmp_len = strlen(ptmp_cmd_info->pCmdStr);

        //  计算空格填充数量
        int space_num = max_len - tmp_len;

        //  打印前缀空格
        VT100_Send_String("    ");

        //  打印命令字符串
        VT100_Send_String(ptmp_cmd_info->pCmdStr);

        //  打印剩余填充空格
        UserCmd_help_printSpace(space_num);

        //  打印帮助字符串
        VT100_Send_String(ptmp_cmd_info->pHelpStr);

        //  结束当前命令
        VT100_Send_String("\r\n");
    }

    //  完成
    return 0;
}

//-----------------------------------------------------------------------------------
#if CMD_HISTORY_EN
//  打印历史命令
static int UserCmd_history_do(char* cmd_str, int len)
{
    //  换行开始
    VT100_Send_String("\r\n");

    //  获取历史命令总数
    int total_history_num = CmdHistory_GetHistoryNum();
    //USER_UI_printf("total_history_num = %d\r\n", total_history_num);

    //  当没有历史命令
    if(total_history_num <= 0)
    {
        VT100_Send_String("    No History Command!!\r\n");
        return 0;
    }

    //  循环打印所有历史命令
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

    //  完成
    return 0;
}
#endif

//-----------------------------------------------------------------------------------
//  命令列表
static const SUserCmd UserCmdBuf[] =
{
    //  执行函数指针           控制函数指针        命令字符串         帮助信息字符串
    {UserCmd_test_do,        0,                "test",          "This a test command"},
    //{UserCmd_add_do,         0,                "add",           "Calc Add a+b"},   //  由于单片机堆栈太小，无法支持sscanf的深层嵌套
    {UserCmd_arg_do,         0,                "arg",           "Test print arg command"},
    {UserCmd_clean_do,       0,                "clean",         "Clean the screen"},
    {UserCmd_help_do,        0,                "help",          "Show this help"},

    #if CMD_HISTORY_EN
    {UserCmd_history_do,     0,                CMD_HISTORY_STR, "Show command history list"},
    #endif
};

//-----------------------------------------------------------------------------------
//  已经封装的函数

//  获取当前命令的总数
int UserCmd_GetCmdTotalNum(void)
{
    return sizeof(UserCmdBuf) / sizeof(SUserCmd);
}

//  获取指定序号命令的结构体指针
//  成功返回指针，失败返回0
//  index为指定的序号
const SUserCmd* UserCmd_GetCmdInfo(int index)
{
    //  检查参数
    if(index < 0) return 0;

    //  获取当前命令总数
    int total_cmd_num = UserCmd_GetCmdTotalNum();

    //  检查输入序号是否合法
    if(index >= total_cmd_num) return 0;

    //  返回指针
    return &UserCmdBuf[index];
}
