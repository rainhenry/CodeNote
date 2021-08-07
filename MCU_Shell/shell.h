/************************************************************************************
 *
 *   程序名称：基于单片机的shell底层解码程序
 *   设计编写：rainhenry
 *   程序版本：REV 0.1
 *   最后修改：20191028
 *
 *   设计说明
 *     本代码包含以下几个主要部分
 *     1)  一行命令的动态编辑字符部分
 *     2)  解析命令，调用用户命令调用，和内建自组织的帮助系统
 *     3)  可选的历史命令系统(支持history命令和上下键控制)
 *
 ***********************************************************************************/
//-----------------------------------------------------------------------------------
//  重定义保护
#ifndef __SHELL_H__
#define __SHELL_H__

//-----------------------------------------------------------------------------------
//  包含头文件

//-----------------------------------------------------------------------------------
//  C++兼容处理
#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------------------------------------------------------------
//  相关宏定义
#define SHELL_CMD_LINE_STATUS_READY            0      //  命令行状态，等待用户输入命令
#define SHELL_CMD_LINE_STATUS_BUSY             1      //  命令行状态，正在处理用户命令

//-----------------------------------------------------------------------------------
//  定义相关数据类型
//  动态字符存储基本数据结构
typedef struct
{
    //  本单元存储的数据(存在数据时0~255之间， 当为-1的时候表示当前节点不可用)
    int dat;

    //  下一个连续节点(当存在下一个的时候，取值为大于等于0，当不存在的时候，为-1)
    int next;
}SCharUnit;

//-----------------------------------------------------------------------------------
//  导出函数
//  初始化相关
void ShellCmd_Init(void);

//  当硬件初始化完成后，开始shell
void ShellCmd_ShellBegin(void);

//  结束一行命令的处理
void ShellCmd_EndOfCmdLine(void);

//  获取缓存已经使用数量
int ShellCmd_GetAlreadyUseCount(void);

//  在当前光标位置用户输入一个字符  成功返回1， 失败返回0
int ShellCmd_InsertChar(unsigned char in);

//  插入字符串,成功返回插入的字符串长度,失败返回小于等于0
int ShellCmd_InsertString(char* str);

//  将动态字符转换成字符串  返回转换成功的字符个数   pstr为输出字符串的首地址， max_buf为缓冲区的最多大小
int ShellCmd_CurCmdLineToString(char* pstr, int max_buf);

//  获取当前光标位置
int ShellCmd_GetCursorPos(void);

//  光标向左移动 移动成功返回1  失败返回0
int ShellCmd_MoveCursorLeft(void);

//  光标向右移动  移动成功返回1  失败返回0
int ShellCmd_MoveCursorRight(void);

//  刷新当前行的显示  head_str为命令行提示的开头字符串
void ShellCmd_RefreshLineText(char* head_str);

//  从命令行字符串中移除一个字符   pos为要移除字符的位置,0基序   返回1表示移除成功， 返回0表示失败
int ShellCmd_RemoveOneChar(int pos);

//  删除按键  返回1表示有效  返回0表示无效
int ShellCmd_KeyDelete(void);

//  退格按键  返回1表示有效  返回0表示无效
int ShellCmd_KeyBackspace(void);

//  回车按键  成功返回获取到的用户命令的长度， 失败或者没有命令返回0
int ShellCmd_KeyEnter(void);

//  获取命令行状态
int ShellCmd_GetCmdLineStatus(void);

//  获取分隔段落的总数,成功返回段落的总数，失败返回小于等于0
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度
int ShellCmd_GetSplitTotalNum(char* cmd_str, int len);

//  根据指定的段落个数，获取该段落所在的位置序号，成功返回大于等于0，失败返回小于0
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度,seg_index为指定的段落序号，0基序号
int ShellCmd_GetSegPos(char* cmd_str, int len, int seg_index);

//-----------------------------------------------------------------------------------
//  C++兼容处理
#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------
#endif  //  __SHELL_H__
