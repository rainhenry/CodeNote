/************************************************************************************
 *
 *   程序名称：用户命令程序
 *   设计编写：rainhenry
 *   程序版本：REV 0.1
 *   最后修改：20191112
 *
 *   设计说明
 *     为了实现进入命令行后依然可以进行VT100协议交互，并且能够降低MCU的使用资源，采用功能分类的回调函数
 *   进行编写，多个按键或者事件占用一个回调函数时，采用不同参数进行区分。然后传递参数都是采用void指针的形
 *   式，这样增大了代码的可移植性和可扩展性。
 *     对于现阶段仅仅用于简单的MCU调试使用的话，并未实现和用户命令的交互动作代码，因为实现这种需要采用多
 *   任务并发执行的要求，即需要操作系统的支持。所以暂时保留该功能。
 *
 ***********************************************************************************/
//-----------------------------------------------------------------------------------
//  重定义保护
#ifndef __USER_CMD_H__
#define __USER_CMD_H__

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

//-----------------------------------------------------------------------------------
//  定义相关数据类型

//  定义控制回调类型分类
typedef enum
{
    FunCtrlType_BufFull = 0,           //  VT100的缓冲区已经满了
    FunCtrlType_CtrlChar,              //  收到控制类的字符
    FunCtrlType_ASCII_Char,            //  收到ASCII字符
    FunCtrlType_OhterChar,             //  收到其他字符
    FunCtrlType_Up,                    //  上键
    FunCtrlType_Down,                  //  下键
    FunCtrlType_Left,                  //  左键
    FunCtrlType_Right,                 //  右键
    FunCtrlType_Delete,                //  删除键
    FunCtrlType_PageUp,                //  上翻页
    FunCtrlType_PageDown,              //  下翻页
    FunCtrlType_Home,                  //  Home键
    FunCtrlType_End,                   //  End键
    FunCtrlType_Insert,                //  Insert键
    FunCtrlType_F1,                    //  F1键
    FunCtrlType_F2,                    //  F2键
    FunCtrlType_F3,                    //  F3键
    FunCtrlType_F4,                    //  F4键
    FunCtrlType_F5,                    //  F5键
    FunCtrlType_F6,                    //  F6键
    FunCtrlType_F7,                    //  F7键
    FunCtrlType_F8,                    //  F8键
    FunCtrlType_F9,                    //  F9键
    FunCtrlType_F10,                   //  F10键
    FunCtrlType_F11,                   //  F11键
    FunCtrlType_F12,                   //  F12键
    FunCtrlType_ReportCursorPos,       //  得到当前光标位置
    FunCtrlType_HintTab,               //  匹配到N个TAB的提示功能
}EFunCtrlType;

//  定义命令的函数指针
//  成功返回0，失败返回非0  预留
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度
typedef int (*TpUserCmdFun_do)(char* cmd_str, int len);

//  定义命令的控制回调函数指针
//  成功返回0，失败返回非0  预留
//  type为当前的控制类型,pUserData为预留的用户数据指针（仅仅在有参数时才使用）
typedef int (*TpUserCmdFun_ctrl)(EFunCtrlType type, void* pUserData);

//  用于定义一条用户命令的数据结构
typedef struct
{
    TpUserCmdFun_do pUserCmdFun_do;      //  用户执行函数指针
    TpUserCmdFun_ctrl pUserCmdFun_ctrl;  //  用户控制函数指针，当执行用户程序的时候，该函数会接收相关接盘消息，实现用户程序的交互（保留）
    const char* pCmdStr;                 //  命令字符串
    const char* pHelpStr;                //  帮助字符串
}SUserCmd;


//-----------------------------------------------------------------------------------
//  导出函数

//  获取当前命令的总数
int UserCmd_GetCmdTotalNum(void);

//  获取指定序号命令的结构体指针
//  成功返回指针，失败返回0
//  index为指定的序号
const SUserCmd* UserCmd_GetCmdInfo(int index);

//-----------------------------------------------------------------------------------
//  C++兼容处理
#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------
#endif  //  __USER_CMD_H__
