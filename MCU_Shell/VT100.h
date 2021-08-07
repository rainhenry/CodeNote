/************************************************************************************
 *
 *   程序名称：基于单片机的VT100协议
 *   设计编写：rainhenry
 *   程序版本：REV 0.1
 *   最后修改：20191021
 *
 ***********************************************************************************/
//-----------------------------------------------------------------------------------
//  重定义保护
#ifndef __VT100_H__
#define __VT100_H__

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
//  常用控制字符(控制字符范围0x01~0x1F,0x7F)
#define VT100_KEY_BACKSPACE           0x7F                //  退格
#define VT100_KEY_TAB                 0x09                //  TAB键
#define VT100_CTRL_ENTER_R            0x0D                //  回车 '\r'  回到当前行的行首  同时putty按回车时发送的也仅仅为该字符
#define VT100_CTRL_ENTER_N            0x0A                //  换行 '\n'  到达下一行，列位置不动
#define VT100_CTRL_C                  0x03                //  强制终止

//-----------------------------------------------------------------------------------
//  定义相关数据类型
typedef enum
{
    //  显示样式的设置
    VT100_DISP_RESETALL = 0,         //  复位所有设置
    VT100_DISP_BRIGHT = 1,           //  显示高亮
    VT100_DISP_DIM = 2,              //  显示变暗
    VT100_DISP_UNDERSCORE = 4,       //  下划线
    VT100_DISP_BLINK = 5,            //  闪烁
    VT100_DISP_REVERSE = 7,          //  反显
    VT100_DISP_HIDDEN = 8,           //  隐藏

    //  字符颜色
    VT100_TEXTCOLOR_BLACK = 30,      //  黑色
    VT100_TEXTCOLOR_RED = 31,        //  红色
    VT100_TEXTCOLOR_GREEN = 32,      //  绿色
    VT100_TEXTCOLOR_YELLOW = 33,     //  黄色
    VT100_TEXTCOLOR_BLUE = 34,       //  蓝色
    VT100_TEXTCOLOR_MAGENTA = 35,    //  紫色
    VT100_TEXTCOLOR_CYAN = 36,       //  青色/蓝绿色
    VT100_TEXTCOLOR_WHITE = 37,      //  白色

    //  背景颜色
    VT100_BACKCOLOR_BLACK = 40,      //  黑色
    VT100_BACKCOLOR_RED = 41,        //  红色
    VT100_BACKCOLOR_GREEN = 42,      //  绿色
    VT100_BACKCOLOR_YELLOW = 43,     //  黄色
    VT100_BACKCOLOR_BLUE = 44,       //  蓝色
    VT100_BACKCOLOR_MAGENTA = 45,    //  紫色
    VT100_BACKCOLOR_CYAN = 46,       //  青色/蓝绿色
    VT100_BACKCOLOR_WHITE = 47,      //  白色
}EVT100DispAttrib;

//-----------------------------------------------------------------------------------
//  导出函数  发送部分
//  发送一个字符
void VT100_Send_OneChar(char in);

//  发送一个字符串
void VT100_Send_String(char* str);

//  VT100的printf支持
void VT100_printf(char* str, ...);

//  向物理终端设备发送数据
void VT100_SEND_TO_PHY_DEVICE(unsigned char* pdat, int len);

//  清除屏幕
void VT100_Send_ClearScreen(void);

//  清除一行
void VT100_Send_ClearLine(void);

//  隐藏光标
void VT100_Send_HideCursor(void);

//  显示光标
void VT100_Send_ShowCursor(void);

//  光标向上   char_num移动的字符数目  1基序
void VT100_Send_CursorUp(int char_num);

//  光标向下   char_num移动的字符数目  1基序
void VT100_Send_CursorDown(int char_num);

//  光标向左   char_num移动的字符数目  1基序
void VT100_Send_CursorLeft(int char_num);

//  光标向右   char_num移动的字符数目  1基序
void VT100_Send_CursorRight(int char_num);

//  将光标移动到指定位置  1基序
void VT100_Send_CursorMoveTo(int x, int y);

//  获取当前光标的位置
void VT100_Send_GetCursorPos(void);

//  开启换行
void VT100_Send_EnableWrap(void);

//  关闭换行
void VT100_Send_DisableWrap(void);

//  设置显示属性
void VT100_Send_DisplayAttributes(EVT100DispAttrib attrib);

//  发送退格
void VT100_Send_Backspace(void);

//  发送保存光标位置
void VT100_Send_SaveCursorPos(void);

//  发送恢复光标位置
void VT100_Send_UnsaveCursorPos(void);

//-----------------------------------------------------------------------------------
//  导出函数  接收部分
//  获取缓冲区当前已经存在的字节个数
int VT100_GetRecvBufCount(void);

//  清空全部缓冲区
void VT100_ClearRecvBuf(void);

//  插入一个字符
void VT100_InsertRecvBuf(unsigned char in);

//  整体的接收处理
void VT100_RecvHandle(void);

//-----------------------------------------------------------------------------------
//  C++兼容处理
#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------
#endif  //  __VT100_H__
