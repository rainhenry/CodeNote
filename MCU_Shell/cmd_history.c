
//  程序版本 REV 0.1

//-----------------------------------------------------------------------------------
//  包含头文件
#include "../MCU_Shell/MCU_Shell.h"
#include <string.h>


//-----------------------------------------------------------------------------------
//  是否开启历史命令功能
#if CMD_HISTORY_EN

//  定义输入输出的位置
static int CmdHistory_BeginPos = 0;   //  开始位置
static int CmdHistory_EndPos = 0;     //  结束位置

//  定义历史命令缓存
static char CmdHistory_CmdStringBuffer[SHELL_CMD_MAX_CHAR * CMD_HISTORY_NUM_MAX];

//  当前历史命令的位置  0=正常输入模式  大于0表示回顾的历史命令编号
static int CmdHistory_HistoryPos = 0;

//  复位命令历史相关
void CmdHistory_Init(void)
{
    //  复位位置
    CmdHistory_BeginPos = 0;
    CmdHistory_EndPos = 0;

    //  清空缓存
    memset(CmdHistory_CmdStringBuffer, 0, sizeof(CmdHistory_CmdStringBuffer));

    //  复位历史位置
    CmdHistory_HistoryPos = 0;
}

//  历史向上  1=调用成功  0=调用失败
int CmdHistory_HistoryUp(void)
{
    //  获取当前已经存在的命令个数
    int already_cmd_num = CmdHistory_GetHistoryNum();

    //  当不存在历史
    if(already_cmd_num <= 0) return 0;     //  操作失败

    //  检查当前是否已经达到历史的顶端
    if(CmdHistory_HistoryPos >= already_cmd_num) return 0;   //  操作失败

    //  统计位置
    CmdHistory_HistoryPos++;

    //  操作成功
    return 1;
}

//  历史向下  1=调用成功  0=调用失败
int CmdHistory_HistoryDown(void)
{
    //  检查当前是否已经回退到最新
    if(CmdHistory_HistoryPos <= 0) return 0;   //  操作失败

    //  统计位置
    CmdHistory_HistoryPos--;

    //  操作成功
    return 1;
}

//  清除历史位置，用于回车调用
void CmdHistory_CleanHistoryPos(void)
{
    CmdHistory_HistoryPos = 0;
}

//  获取当前历史的内容  成功返回历史命令字符串首地址， 失败返回0指针
char* CmdHistory_GetHistoryCurStr(void)
{
    //  当当前历史位置为最新
    if(CmdHistory_HistoryPos <= 0) return 0;

    //  获取字符串
    return CmdHistory_GetCmdStringByN(CmdHistory_HistoryPos - 1);
}

//  获取当前存在的历史命令数量
int CmdHistory_GetHistoryNum(void)
{
    //  当相等
    if(CmdHistory_BeginPos == CmdHistory_EndPos)
    {
        return 0;    //  没有内容
    }
    //  当正向
    else if(CmdHistory_EndPos > CmdHistory_BeginPos)
    {
        return CmdHistory_EndPos - CmdHistory_BeginPos;   //  返回计算的数量
    }
    //  当反向
    else
    {
        return (CMD_HISTORY_NUM_MAX - CmdHistory_BeginPos) + CmdHistory_EndPos;
    }
}

//  插入一个命令
void CmdHistory_InsertNewCmd(char* cmd_str)
{
    //  检查参数
    if(cmd_str == 0) return;

    //  获取当前输入命令的长度
    int cmd_len = strlen(cmd_str);

    //  检查长度
    if(cmd_len >= SHELL_CMD_MAX_CHAR) return;
    if(cmd_len <= 0) return;

    //  获取当前已经存在的命令个数
    int already_cmd_num = CmdHistory_GetHistoryNum();

    //  检查是否为无损插入  无损  空命令列表
    if(already_cmd_num <= 0)
    {
        //  清空位置
        memset(CmdHistory_CmdStringBuffer + 0, 0, SHELL_CMD_MAX_CHAR);

        //  执行插入
        strcpy(CmdHistory_CmdStringBuffer + 0, cmd_str);

        //  统计
        if(CmdHistory_EndPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_EndPos = 0;
        else                                               CmdHistory_EndPos = CmdHistory_EndPos + 1;
    }
    //  无损 但有老命令
    else if(already_cmd_num < (CMD_HISTORY_NUM_MAX - 1))
    {
        //  计算上一个位置
        int last_cmd_pos = 0;
        if(CmdHistory_EndPos <= 0) last_cmd_pos = CMD_HISTORY_NUM_MAX - 1;
        else                       last_cmd_pos = CmdHistory_EndPos - 1;

        //  对比本次命令和上一次命令是否为同一个，相同忽略
        if(strcmp(CmdHistory_CmdStringBuffer + (last_cmd_pos * SHELL_CMD_MAX_CHAR), cmd_str) == 0) return;

        //  清空位置
        memset(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), 0, SHELL_CMD_MAX_CHAR);

        //  执行插入
        strcpy(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), cmd_str);

        //  统计
        if(CmdHistory_EndPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_EndPos = 0;
        else                                               CmdHistory_EndPos = CmdHistory_EndPos + 1;
    }
    //  有损插入
    else
    {
        //  计算上一个位置
        int last_cmd_pos = 0;
        if(CmdHistory_EndPos <= 0) last_cmd_pos = CMD_HISTORY_NUM_MAX - 1;
        else                       last_cmd_pos = CmdHistory_EndPos - 1;

        //  对比本次命令和上一次命令是否为同一个，相同忽略
        if(strcmp(CmdHistory_CmdStringBuffer + (last_cmd_pos * SHELL_CMD_MAX_CHAR), cmd_str) == 0) return;

        //  清空位置
        memset(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), 0, SHELL_CMD_MAX_CHAR);

        //  执行插入
        strcpy(CmdHistory_CmdStringBuffer + (CmdHistory_EndPos * SHELL_CMD_MAX_CHAR), cmd_str);

        //  统计
        if(CmdHistory_EndPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_EndPos = 0;
        else                                               CmdHistory_EndPos = CmdHistory_EndPos + 1;

        //  删除掉最老的命令
        if(CmdHistory_BeginPos >= (CMD_HISTORY_NUM_MAX - 1)) CmdHistory_BeginPos = 0;
        else                                                 CmdHistory_BeginPos = CmdHistory_BeginPos + 1;
    }

    //USER_UI_printf("a = %d, b = %d, e = %d\r\n", already_cmd_num, CmdHistory_BeginPos, CmdHistory_EndPos);
}

//  获取倒数第n个命令的字符串地址指针
//  成功返回字符串指针，失败返回0指针
//  输入的n表示时间顺序最近的一个，0基序
char* CmdHistory_GetCmdStringByN(int n)
{
    //  获取当前已经存在的命令个数
    int already_cmd_num = CmdHistory_GetHistoryNum();

    //  如果为空命令列表
    if(already_cmd_num <= 0) return 0;

    //  检查参数n
    if(n > (already_cmd_num - 1)) return 0;

    //  尝试计算指定的位置
    int tmp_pos = CmdHistory_EndPos;
    int i=0;
    for(i=0;i<(n+1);i++)
    {
        if(tmp_pos <= 0) tmp_pos = CMD_HISTORY_NUM_MAX - 1;
        else             tmp_pos = tmp_pos - 1;
    }

    //  得到指定的命令字符串的首地址
    return CmdHistory_CmdStringBuffer + (tmp_pos * SHELL_CMD_MAX_CHAR);
}

//-----------------------------------------------------------------------------------
#endif  //  CMD_HISTORY_EN
