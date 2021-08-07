
//  程序版本 REV 0.1

//-----------------------------------------------------------------------------------
//  包含头文件
#include "../MCU_Shell/MCU_Shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "ics.h"
#include "uart.h"

#include "derivative.h" /* include peripheral declarations SSKEAZN64M2 */


//  定义发送数据
void Send_UART1(char* pdat, int len);

//-----------------------------------------------------------------------------------
//  用户定义回调函数
//  向物理终端设备发送数据
void VT100_SEND_TO_PHY_DEVICE(unsigned char* pdat, int len)
{
    //  调用发送函数
    //COM_Send(pdat, len);
	Send_UART1(pdat, len);
}

//  输入缓冲区已经满了
void VT100_RECV_BUF_FULL(void)
{
    //USER_UI_printf("----VT100_RECV_BUF_FULL----\n");
}

//  识别到用于控制字符
void VT100_USER_CTRL_CHAR(unsigned char in)
{
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  当为backspace按下的时候
        if(in == VT100_KEY_BACKSPACE)
        {
            //  执行backspace
            int re = ShellCmd_KeyBackspace();

            //  检查执行结果
            if(re == 1)
            {
                //  调用回显
                VT100_SEND_TO_PHY_DEVICE(&in, 1);

                //  保存光标
                VT100_Send_SaveCursorPos();

                //  刷新显示
                ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

                //  恢复光标
                VT100_Send_UnsaveCursorPos();
            }
        }
        //  当为回车键
        else if(in == VT100_CTRL_ENTER_R)
        {
            //  执行回车
            int re = ShellCmd_KeyEnter();

            //  当没有成功执行命令的时候
            if(re <= 0)
            {
                //  发送换行
                unsigned char tmp_char = VT100_CTRL_ENTER_N;
                VT100_SEND_TO_PHY_DEVICE(&tmp_char, 1);
            }

            //  刷新显示
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

            //  清除历史光标位置
#if CMD_HISTORY_EN
            CmdHistory_CleanHistoryPos();
#endif
        }
        //  其他控制
        else
        {
            //USER_UI_printf("Recv Other Ctrl char 0x%02X\n", in & 0x0FF);
        }
    }
}

//  识别到用户可显示的ASCII字符
void VT100_USER_ASCII_CHAR(unsigned char in)
{
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  调用shell解释器
        ShellCmd_InsertChar(in);

        //  调用回显
        VT100_SEND_TO_PHY_DEVICE(&in, 1);

        //  保存光标
        VT100_Send_SaveCursorPos();

        //  刷新显示
        ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

        //  恢复光标
        VT100_Send_UnsaveCursorPos();
    }
}

//  识别到其他用户字符
void VT100_USER_OTHER_CHAR(unsigned char in)
{
    //USER_UI_printf("Recv Ohter User char 0x%02X\n", in & 0x0FF);
}

//  全部字符回调
void VT100_USER_ALL_CHAR(unsigned char in)
{
    //USER_UI_printf("Recv char 0x%02X\n", in & 0x0FF);
}

//  匹配上键回调函数
void VT100_MATCH_UP(void)
{
    //USER_UI_printf("Recv Up Key!!\n");

#if CMD_HISTORY_EN
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  执行历史操作
        int re = CmdHistory_HistoryUp();

        //  当执行成功
        if(re == 1)
        {
            //  获取历史字符串
            char* tmp_str = CmdHistory_GetHistoryCurStr();

            //  清除shell当前行的原始内容
            ShellCmd_EndOfCmdLine();

            //  插入该字符串到shell中
            ShellCmd_InsertString(tmp_str);

            //  刷新显示
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
        }
        //  执行失败
        else
        {
            //  此时 达到最顶端的时候，不清除
        }
    }
#endif
}

//  匹配下键回调函数
void VT100_MATCH_DOWN(void)
{
    //USER_UI_printf("Recv Down Key!!\n");

#if CMD_HISTORY_EN
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  执行历史操作
        int re = CmdHistory_HistoryDown();

        //  当执行成功
        if(re == 1)
        {
            //  获取历史字符串
            char* tmp_str = CmdHistory_GetHistoryCurStr();

            //  清除shell当前行的原始内容
            ShellCmd_EndOfCmdLine();

            //  插入该字符串到shell中
            ShellCmd_InsertString(tmp_str);

            //  刷新显示
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
        }
    }
    //  执行失败
    else
    {
        //  清除shell当前行的原始内容
        ShellCmd_EndOfCmdLine();

        //  刷新显示
        ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
    }
#endif
}

//  匹配左键回调函数
void VT100_MATCH_LEFT(void)
{
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  调试提示
        //USER_UI_printf("Recv Left Key!!\n");

        //  调用shell解释器
        int re = ShellCmd_MoveCursorLeft();

        //  当调用成功
        if(re == 1)
        {
            //  回显左移动
            VT100_Send_CursorLeft(1);
        }
    }
}

//  匹配右键回调函数
void VT100_MATCH_RIGHT(void)
{
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  调试提示
        //USER_UI_printf("Recv Right Key!!\n");

        //  调用shell解释器
        int re = ShellCmd_MoveCursorRight();

        //  当调用成功
        if(re == 1)
        {
            //  回显右移动
            VT100_Send_CursorRight(1);
        }
    }
}

//  匹配Delete键回调函数
void VT100_MATCH_DELETE(void)
{
    //  当命令行为空闲的时候
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //USER_UI_printf("Recv Delete Key!!\n");

        //  执行delete
        int re = ShellCmd_KeyDelete();

        //  检查是否成功
        if(re == 1)
        {
            //  保存光标
            VT100_Send_SaveCursorPos();

            //  刷新显示
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

            //  恢复光标
            VT100_Send_UnsaveCursorPos();
        }
    }
}

//  匹配PageUp键回调函数
void VT100_MATCH_PAGEUP(void)
{
    //USER_UI_printf("Recv PageUp Key!!\n");
}

//  匹配PageDown键回调函数
void VT100_MATCH_PAGEDOWN(void)
{
    //USER_UI_printf("Recv PageDown Key!!\n");
}

//  匹配Home键回调函数
void VT100_MATCH_HOME(void)
{
    //USER_UI_printf("Recv Home Key!!\n");
}

//  匹配End键回调函数
void VT100_MATCH_END(void)
{
    //USER_UI_printf("Recv End Key!!\n");
}

//  匹配Insert键回调函数
void VT100_MATCH_INSERT(void)
{
    //USER_UI_printf("Recv Insert Key!!\n");
}

//  匹配F1键回调函数
void VT100_MATCH_F1(void)
{
    //USER_UI_printf("Recv F1 Key!!\n");
}

//  匹配F2键回调函数
void VT100_MATCH_F2(void)
{
    //USER_UI_printf("Recv F2 Key!!\n");
}

//  匹配F3键回调函数
void VT100_MATCH_F3(void)
{
    //USER_UI_printf("Recv F3 Key!!\n");
}

//  匹配F4键回调函数
void VT100_MATCH_F4(void)
{
    //USER_UI_printf("Recv F4 Key!!\n");
}

//  匹配F5键回调函数
void VT100_MATCH_F5(void)
{
    //USER_UI_printf("Recv F5 Key!!\n");
}

//  匹配F6键回调函数
void VT100_MATCH_F6(void)
{
    //USER_UI_printf("Recv F6 Key!!\n");
}

//  匹配F7键回调函数
void VT100_MATCH_F7(void)
{
    //USER_UI_printf("Recv F7 Key!!\n");
}

//  匹配F8键回调函数
void VT100_MATCH_F8(void)
{
   // USER_UI_printf("Recv F8 Key!!\n");
}

//  匹配F9键回调函数
void VT100_MATCH_F9(void)
{
    //USER_UI_printf("Recv F9 Key!!\n");
}

//  匹配F10键回调函数
void VT100_MATCH_F10(void)
{
    //USER_UI_printf("Recv F10 Key!!\n");
}

//  匹配F11键回调函数
void VT100_MATCH_F11(void)
{
    //USER_UI_printf("Recv F11 Key!!\n");
}

//  匹配F12键回调函数
void VT100_MATCH_F12(void)
{
    //USER_UI_printf("Recv F12 Key!!\n");
}

//  匹配到报告光标位置
void VT100_MATCH_REPORT_CURSOR_POS(int x, int y)
{
    //USER_UI_printf("Recv Cursor Pos X:%d  Y:%d\n", x, y);
}

//  匹配到连续按下N个TAB的提示
void VT100_MATCH_HINTTAB(void)
{
    //USER_UI_printf("Recv Hint Message!!\n");
}


//-----------------------------------------------------------------------------------
//  VT100的底层封装部分  发送部分
//  发送一个字符
void VT100_Send_OneChar(char in)
{
    VT100_SEND_TO_PHY_DEVICE(&in, 1);
}

//  发送一个字符串
void VT100_Send_String(char* str)
{
    int str_len = strlen(str);
    VT100_SEND_TO_PHY_DEVICE(str, str_len);
}

//  VT100的printf支持
#if VT100_PRINTF_EN
void VT100_printf(char* str, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, str);

    char s[VT100_PRINTF_BUFSIZE];
    memset(s,0x00,sizeof(s));

    vsprintf(s,str,arg_ptr);
    va_end(arg_ptr);

    VT100_Send_String(s);
}
#endif

//  清除屏幕
void VT100_Send_ClearScreen(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x32, 0x4A};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  清除一行
void VT100_Send_ClearLine(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x32, 0x4B};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  隐藏光标
void VT100_Send_HideCursor(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x3F, 0x32, 0x35, 0x6C};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  显示光标
void VT100_Send_ShowCursor(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x3F, 0x32, 0x35, 0x68};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  光标的相对位移   char_num移动的字符数目    1基序   dir_code移动方向的代码
void VT100_Send_CursorRelatiovePos(int char_num, unsigned char dir_code)
{
    //  检查参数
    if(char_num <= 0) return;
    if(char_num > 999) return;

    //  构造VT100数据
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x00, 0x00, 0x00, 0x00};

    //  设置移动方向的代码
    memset(tmp_buf + 2, dir_code, 4);

    //  分析移动数目
    int num_buf[3];
    num_buf[0] = (char_num / 100) % 10;
    num_buf[1] = (char_num / 10) % 10;
    num_buf[2] = (char_num / 1) % 10;

    //  计算发送总数,并构造数值
    int send_len = 0;
    //  发送有效数字为3位数字
    if(char_num > 99)
    {
        send_len = 3 + 3;
        tmp_buf[2] = num_buf[0] + 0x30;
        tmp_buf[3] = num_buf[1] + 0x30;
        tmp_buf[4] = num_buf[2] + 0x30;
    }
    //  发送有效数字为2位数字
    else if(char_num > 9)
    {
        send_len = 2 + 3;
        tmp_buf[2] = num_buf[1] + 0x30;
        tmp_buf[3] = num_buf[2] + 0x30;
    }
    //  发送有效数字为1位数字
    else
    {
        send_len = 1 + 3;
        tmp_buf[2] = num_buf[2] + 0x30;
    }

    //  发送数据
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, send_len);
}


//  光标向上   char_num移动的字符数目  1基序
void VT100_Send_CursorUp(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x41);
}

//  光标向下   char_num移动的字符数目  1基序
void VT100_Send_CursorDown(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x42);
}

//  光标向左   char_num移动的字符数目  1基序
void VT100_Send_CursorLeft(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x44);
}

//  光标向右   char_num移动的字符数目  1基序
void VT100_Send_CursorRight(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x43);
}

//  将光标移动到指定位置  1基序
void VT100_Send_CursorMoveTo(int x, int y)
{
    //  检查参数
    if(x <= 0) return;          if(x > 999) return;
    if(y <= 0) return;          if(y > 999) return;

    //  构造VT100数据
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    //  分析移动数目
    int x_buf[3];
    x_buf[0] = (x / 100) % 10;
    x_buf[1] = (x / 10) % 10;
    x_buf[2] = (x / 1) % 10;
    int y_buf[3];
    y_buf[0] = (y / 100) % 10;
    y_buf[1] = (y / 10) % 10;
    y_buf[2] = (y / 1) % 10;

    //  计算x部分,并构造数值
    int send_len = 2;
    //  发送有效数字为3位数字
    if(y > 99)
    {
        send_len += 3;
        tmp_buf[2] = y_buf[0] + 0x30;
        tmp_buf[3] = y_buf[1] + 0x30;
        tmp_buf[4] = y_buf[2] + 0x30;
    }
    //  发送有效数字为2位数字
    else if(y > 9)
    {
        send_len += 2;
        tmp_buf[2] = y_buf[1] + 0x30;
        tmp_buf[3] = y_buf[2] + 0x30;
    }
    //  发送有效数字为1位数字
    else
    {
        send_len += 1;
        tmp_buf[2] = y_buf[2] + 0x30;
    }

    //  设置分号
    tmp_buf[send_len] = 0x3B;
    send_len++;

    //  计算y部分,并构造数值
    //  发送有效数字为3位数字
    if(x > 99)
    {
        tmp_buf[send_len + 0] = x_buf[0] + 0x30;
        tmp_buf[send_len + 1] = x_buf[1] + 0x30;
        tmp_buf[send_len + 2] = x_buf[2] + 0x30;
        send_len += 3;
    }
    //  发送有效数字为2位数字
    else if(x > 9)
    {
        tmp_buf[send_len + 0] = x_buf[1] + 0x30;
        tmp_buf[send_len + 1] = x_buf[2] + 0x30;
        send_len += 2;
    }
    //  发送有效数字为1位数字
    else
    {
        tmp_buf[send_len + 0] = x_buf[2] + 0x30;
        send_len += 1;
    }

    //  设置字母结尾
    tmp_buf[send_len] = 0x48;
    send_len++;

    //  发送数据
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, send_len);
}

//  获取当前光标的位置
void VT100_Send_GetCursorPos(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x36, 0x6E};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  开启换行
void VT100_Send_EnableWrap(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x37, 0x68};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  关闭换行
void VT100_Send_DisableWrap(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x37, 0x6C};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  设置显示属性
void VT100_Send_DisplayAttributes(EVT100DispAttrib attrib)
{
    //  获取当前的属性代码值
    int code = (int)attrib;

    //  检查属性值
    if(code < 0) return;
    if(code > 99) return;

    //  构造VT100数据
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x6D, 0x6D, 0x6D};

    //  分析数目
    int num_buf[2];
    num_buf[0] = (code / 10) % 10;
    num_buf[1] = (code / 1) % 10;

    //  计算发送数目，并填充数值
    int send_len = 0;
    //  当为2位数
    if(code > 9)
    {
        send_len = 3 + 2;
        tmp_buf[2] = num_buf[0] + 0x30;
        tmp_buf[3] = num_buf[1] + 0x30;
    }
    //  当为1位数
    else
    {
        send_len = 3 + 1;
        tmp_buf[2] = num_buf[1] + 0x30;
    }

    //  发送数据
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, send_len);
}

//  发送退格
void VT100_Send_Backspace(void)
{
    unsigned char tmp_buf[] = {0x7F};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  发送保存光标位置
void VT100_Send_SaveCursorPos(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x73};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  发送恢复光标位置
void VT100_Send_UnsaveCursorPos(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x75};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//-----------------------------------------------------------------------------------
//  VT100的底层封装部分  接收部分

//  定义接收缓冲区相关
static unsigned char VT100_RecvBuf[VT100_RECV_BUF_SIZE];        //  环形缓冲区
static int VT100_RecvBuf_in_pos = 0;                            //  入队位置
static int VT100_RecvBuf_out_pos = 0;                           //  出队位置

//  获取缓冲区当前已经存在的字节个数
int VT100_GetRecvBufCount(void)
{
    //  当位置相同
    if(VT100_RecvBuf_in_pos == VT100_RecvBuf_out_pos)
    {
        //  缓冲区为空
        return 0;
    }
    //  当插入位置大于读出位置
    else if(VT100_RecvBuf_in_pos > VT100_RecvBuf_out_pos)
    {
        return VT100_RecvBuf_in_pos - VT100_RecvBuf_out_pos;
    }
    //  否则，当读出位置大于插入位置
    else
    {
        return (VT100_RECV_BUF_SIZE - VT100_RecvBuf_out_pos) + VT100_RecvBuf_in_pos;
    }
}

//  插入一个字符
void VT100_InsertRecvBuf(unsigned char in)
{
    //  检查缓冲区已经使用的字节
    int already_len = VT100_GetRecvBufCount();

    //  检查缓冲区是否满
    if(already_len >= (VT100_RECV_BUF_SIZE - 1))
    {
        //  调用用户回调函数，提示缓冲区已经满
        VT100_RECV_BUF_FULL();

        //  返回
        return;
    }

    //  执行插入操作
    VT100_RecvBuf[VT100_RecvBuf_in_pos] = in;

    //  计算下一次的插入位置
    if(VT100_RecvBuf_in_pos >= (VT100_RECV_BUF_SIZE - 1))  VT100_RecvBuf_in_pos = 0;
    else                                                   VT100_RecvBuf_in_pos++;
}

//  读出一个字符，不删除       pos为从读出开始的位置，0基序   成功返回0~255的数，失败返回-1
static int VT100_ReadOutRecvBuf_WithoutDelete(int pos)
{
    //  检查缓冲区已经使用的字节
    int already_len = VT100_GetRecvBufCount();

    //  检查请求的位置是否超出缓冲区范围
    if(already_len <= 0) return -1;
    if((pos + 1) > already_len) return -1;
    if(pos < 0) return -1;

    //  计算访问的实际位置
    int read_pos = VT100_RecvBuf_out_pos + pos;
    if(read_pos > (VT100_RECV_BUF_SIZE - 1)) read_pos = read_pos - VT100_RECV_BUF_SIZE;
    if(read_pos > (VT100_RECV_BUF_SIZE - 1)) return -1;

    //  取出数据
    return VT100_RecvBuf[read_pos] & 0x0FF;
}

//  从读出侧删除数据  cnt为要删除的个数  失败返回-1  成功返回0
static int VT100_RecvBufDeleteReadOut(int cnt)
{
    //  检查缓冲区已经使用的字节
    int already_len = VT100_GetRecvBufCount();

    //  检查请求的位置是否超出缓冲区范围
    if(already_len <= 0) return -1;
    if(cnt > already_len) return -1;
    if(cnt <= 0) return -1;

    //  计算删除的实际位置
    int pos = VT100_RecvBuf_out_pos + cnt;
    if(pos > (VT100_RECV_BUF_SIZE - 1)) pos = pos - VT100_RECV_BUF_SIZE;
    if(pos > (VT100_RECV_BUF_SIZE - 1)) return -1;

    //  执行变更
    VT100_RecvBuf_out_pos = pos;

    //  返回成功
    return 0;
}

//  清空全部缓冲区
void VT100_ClearRecvBuf(void)
{
    //  设置入队出队位置
    VT100_RecvBuf_in_pos = 0;
    VT100_RecvBuf_out_pos = 0;

    //  清空缓存
    memset(VT100_RecvBuf, 0, sizeof(VT100_RecvBuf));
}

//  接收匹配 上键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Up(int len)
{
    //  上键正常按下为 1B 5B 41

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x41};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_UP();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 下键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Down(int len)
{
    //  下键正常按下为 1B 5B 42

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x42};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_DOWN();                    //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 左键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Left(int len)
{
    //  左键正常按下为 1B 5B 44

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x44};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_LEFT();                    //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 右键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Right(int len)
{
    //  右键正常按下为 1B 5B 43

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x43};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_RIGHT();                   //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 Delete键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Delete(int len)
{
    //  Delete键正常按下为 1B 5B 33 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x33, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_DELETE();                  //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}


//  接收匹配 PageUp键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_PageUp(int len)
{
    //  PageUp键正常按下为 1B 5B 35 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x35, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_PAGEUP();                  //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 PageDown键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_PageDown(int len)
{
    //  PageDown键正常按下为 1B 5B 36 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x36, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_PAGEDOWN();                //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 Home键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Home(int len)
{
    //  Home键正常按下为 1B 5B 31 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_HOME();                    //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 End键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_End(int len)
{
    //  End键正常按下为 1B 5B 34 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x34, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_END();                     //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 Insert键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_Insert(int len)
{
    //  Insert键正常按下为 1B 5B 32 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_INSERT();                  //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F1键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F1(int len)
{
    //  F1键正常按下为 1B 5B 31 31 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x31, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F1();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F2键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F2(int len)
{
    //  F2键正常按下为 1B 5B 31 31 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x32, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F2();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F3键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F3(int len)
{
    //  F3键正常按下为 1B 5B 31 33 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x33, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F3();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F4键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F4(int len)
{
    //  F4键正常按下为 1B 5B 31 34 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x34, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F4();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F5键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F5(int len)
{
    //  F5键正常按下为 1B 5B 31 35 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x35, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F5();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F6键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F6(int len)
{
    //  F6键正常按下为 1B 5B 31 37 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x37, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F6();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F7键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F7(int len)
{
    //  F7键正常按下为 1B 5B 31 38 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x38, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F7();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F8键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F8(int len)
{
    //  F8键正常按下为 1B 5B 31 39 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x39, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F8();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F9键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F9(int len)
{
    //  F9键正常按下为 1B 5B 32 30 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x30, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F9();                      //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F10键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F10(int len)
{
    //  F10键正常按下为 1B 5B 32 31 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x31, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F10();                     //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F11键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F11(int len)
{
    //  F11键正常按下为 1B 5B 32 33 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x33, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F11();                     //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  接收匹配 F12键   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_F12(int len)
{
    //  F12键正常按下为 1B 5B 32 34 7E

    //  定义缓冲区
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x34, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_F12();                     //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  从某个起始位置检查是否为数字  begin_pos为要开始检查的位置    pnum_len为返回的数字长度
//  错误和不符合都返回0,  匹配到非有效数字（0开头的），返回1，  匹配到有效数字（非0开头的）返回2
static int VT100_Recv_CheckNum(int begin_pos, int* pnum_len)
{
    //  获得当前缓冲区的有效数据数目
    int total_len = VT100_GetRecvBufCount();

    //  检查
    if(total_len <= 0) return 0;
    if(begin_pos < 0) return 0;
    if(begin_pos > (total_len - 1)) return 0;
    if(pnum_len == 0) return 0;

    //  定义返回值
    int re_value = 0;

    //  定义当前获取的位置
    int pos = begin_pos;

    //  检查第一个
    unsigned char tmp_byte = VT100_ReadOutRecvBuf_WithoutDelete(pos);
    //  当为合法的有效数字
    if((0x31 <= tmp_byte) && (tmp_byte <= 0x39))
    {
        pnum_len[0] = 1;       //  找到1个数字
        re_value = 2;          //  设置未来的返回值
        pos++;                 //  统计已经处理的字符
    }
    //  当为合法数字,但并不是有效数字
    else if(tmp_byte == 0x30)
    {
        pnum_len[0] = 1;       //  找到1个数字
        re_value = 1;          //  设置未来的返回值
        pos++;                 //  统计已经处理的字符
    }
    //  其他字符
    else
    {
        pnum_len[0] = 0;
        return 0;
    }

    //  计算剩余的字符长度
    int last_len = (total_len - 1) - pos;

    //  检查剩余字节
    if(last_len <= 0) return re_value;

    //  遍历剩余字符
    int i=0;
    for(i=0;i<last_len;i++)
    {
        //  获取一个字符
        tmp_byte = VT100_ReadOutRecvBuf_WithoutDelete(pos);

        //  当为合法的有效数字
        if((0x31 <= tmp_byte) && (tmp_byte <= 0x39))
        {
            pnum_len[0] += 1;      //  找到1个数字
            pos++;                 //  统计已经处理的字符
        }
        //  当为合法数字,但并不是有效数字
        else if(tmp_byte == 0x30)
        {
            pnum_len[0] += 1;      //  找到1个数字
            pos++;                 //  统计已经处理的字符
        }
        //  其他字符
        else
        {
            return re_value;
        }
    }

    //  当缓存信息不完整
    return re_value;
}

//  将ASCII字符串转换为int的数字,成功返回1，失败返回0
//  pos_str为输入ASCII字符串的所在缓冲区的起始位置   str_len为字符串的有效长度
//  pout_value为输出数值的指针
static int VT100_ASCIIstrToInt(int pos_str, int str_len, int* pout_value)
{
    //  获得当前缓冲区的有效数据数目
    int total_len = VT100_GetRecvBufCount();


    //  检查输入
    if(pos_str < 0) return 0;
    if(pos_str > (total_len - 1)) return 0;
    if(str_len <= 0) return 0;
    if((pos_str + str_len) > (total_len - 1)) return 0;
    if(pout_value == 0) return 0;

    //  理论计算只考虑3位数字
    if(str_len > 3) return 0;

    //  获取ASCII代码
    unsigned char tmp_buf[3];
    //  三位数字
    if(str_len == 3)
    {
        tmp_buf[0] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 0);
        tmp_buf[1] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 1);
        tmp_buf[2] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 2);
    }
    //  两位数字
    else if(str_len == 2)
    {
        tmp_buf[0] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 0);
        tmp_buf[1] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 1);
    }
    //  一位数字
    else
    {
        tmp_buf[0] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 0);
    }

    //  检查输入范围
    //  三位数字
    if(str_len == 3)
    {
        if((tmp_buf[0] < 0x30) || (tmp_buf[0] > 0x39)) return 0;
        if((tmp_buf[1] < 0x30) || (tmp_buf[1] > 0x39)) return 0;
        if((tmp_buf[2] < 0x30) || (tmp_buf[2] > 0x39)) return 0;
    }
    //  两位数字
    else if(str_len == 2)
    {
        if((tmp_buf[0] < 0x30) || (tmp_buf[0] > 0x39)) return 0;
        if((tmp_buf[1] < 0x30) || (tmp_buf[1] > 0x39)) return 0;
    }
    //  一位数字
    else
    {
        if((tmp_buf[0] < 0x30) || (tmp_buf[0] > 0x39)) return 0;
    }

    //  填充
    pout_value[0] = 0;
    //  三位数字
    if(str_len == 3)
    {
        pout_value[0] += ((tmp_buf[0] & 0x0FF) - 0x30) * 100;
        pout_value[0] += ((tmp_buf[1] & 0x0FF) - 0x30) * 10;
        pout_value[0] += ((tmp_buf[2] & 0x0FF) - 0x30) * 1;
    }
    //  两位数字
    else if(str_len == 2)
    {
        pout_value[0] += ((tmp_buf[0] & 0x0FF) - 0x30) * 10;
        pout_value[0] += ((tmp_buf[1] & 0x0FF) - 0x30) * 1;
    }
    //  一位数字
    else
    {
        pout_value[0] += ((tmp_buf[0] & 0x0FF) - 0x30) * 1;
    }

    //  返回成功
    return 1;
}

//  接收匹配 报告光标位置   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_ReportCursorPos(int len)
{
    //  一般会收到  0x1B 0x5B 0x32 0x3B 0x36 0x52
    //  所以它的格式特征可以理解为 0x1B 0x5B 开头， 然后是一些ASCII格式的数字，然后是0x3B，然后又是ASCII格式的数字，最后结尾为0x52
    //  这里假定X和Y的光标位置最高为3位数字，并且必须以非0的有效数字开头

    //  检查
    if(len <= 0) return 0;

    //  定义有效数字的长度
    int x_begin = 0;
    int x_len = 0;
    int y_begin = 0;
    int y_len = 0;

    //  已经查找的数据长度
    int already_byte_cnt = 0;

    //  定义找到的数字的临时长度
    int tmp_len = 0;

    //  当有效长度为1个字节的时候
    if(len == 1)
    {
        if(VT100_ReadOutRecvBuf_WithoutDelete(0) == 0x1B) return 1;   //  匹配一部分
        else                                              return 0;   //  不匹配
    }
    //  当有效长度为2个字节的时候
    else if(len == 2)
    {
        if((VT100_ReadOutRecvBuf_WithoutDelete(0) == 0x1B) &&
           (VT100_ReadOutRecvBuf_WithoutDelete(1) == 0x5B)) return 1;   //  匹配一部分
        else                                                return 0;   //  不匹配
    }
    //  当有效长度大于2个字节的时候
    else
    {
        //  当数据头匹配
        if((VT100_ReadOutRecvBuf_WithoutDelete(0) == 0x1B) &&
           (VT100_ReadOutRecvBuf_WithoutDelete(1) == 0x5B))
        {
            //  设置已经解析的数据长度
            already_byte_cnt = 2;

            //  检查在此位置上是否存在有效数字
            int re = VT100_Recv_CheckNum(already_byte_cnt, &tmp_len);

            //  检查数字是否获取成功，当获取成功，匹配到有效数字
            if(re == 2)
            {
                //  匹配Y的相关参数
                y_begin = already_byte_cnt;
                y_len = tmp_len;

                //  统计目前已经达到的位置
                already_byte_cnt += tmp_len;

                //  检查当前位置是否已经到达结尾
                if(already_byte_cnt >= len)
                {
                    return 1;  //  匹配一半
                }

                //  检查本字符是否为间隔分割符号
                if(VT100_ReadOutRecvBuf_WithoutDelete(already_byte_cnt) != 0x3B) return 0;  //  不匹配

                //  统计目前已经达到的位置
                already_byte_cnt += 1;

                //  检查当前位置是否已经到达结尾
                if(already_byte_cnt >= len)
                {
                    return 1;  //  匹配一半
                }

                //  检查在此位置上是否存在有效数字
                re = VT100_Recv_CheckNum(already_byte_cnt, &tmp_len);

                //  检查数字是否获取成功，当获取成功，匹配到有效数字
                if(re == 2)
                {
                    //  匹配X的相关参数
                    x_begin = already_byte_cnt;
                    x_len = tmp_len;

                    //  统计目前已经达到的位置
                    already_byte_cnt += tmp_len;

                    //  检查当前位置是否已经到达结尾
                    if(already_byte_cnt >= len)
                    {
                        return 1;  //  匹配一半
                    }

                    //  检查本字符是否为报告的结尾
                    if(VT100_ReadOutRecvBuf_WithoutDelete(already_byte_cnt) != 0x52) return 0;  //  不匹配

                    //  统计目前已经达到的位置
                    already_byte_cnt += 1;

                    //  计算解析数据
                    int x_value = 0;
                    int y_value = 0;
                    re = VT100_ASCIIstrToInt(2, y_len, &y_value);
                    if(re == 0) return 0;
                    re = VT100_ASCIIstrToInt(2 + y_len + 1, x_len, &x_value);
                    if(re == 0) return 0;

                    //  删除这些匹配的数据
                    VT100_RecvBufDeleteReadOut(2 + y_len + 1 + x_len + 1);

                    //  报告数据
                    VT100_MATCH_REPORT_CURSOR_POS(x_value, y_value);

                    //  完全匹配
                    return 2;
                }
                //  当获取失败 或者获取到非有效数字
                else
                {
                    return 0;   //  不匹配
                }
            }
            //  当获取失败 或者获取到非有效数字
            else
            {
                return 0;   //  不匹配
            }
        }
        //  不匹配
        else
        {
            return 0;   //  不匹配
        }
    }
}

//  接收匹配 连续按n个Tab键的触发提示功能   len为当前缓冲区存在数据的长度  未知错误 和 没有匹配成功返回0  匹配一半返回1  完全匹配返回2
static int VT100_RecvMatch_HintTab(int len)
{
    //  TAB键的代码为 0x09

    //  定义缓冲区
    unsigned char target_buf[VT100_RECV_HINT_TAB_NUM];
    memset(target_buf, VT100_KEY_TAB, sizeof(target_buf));
    unsigned char tmp_buf[sizeof(target_buf)];

    //  检查
    if(len <= 0) return 0;

    //  计算实际读取长度
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  读出队列内容到缓冲区
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  对比，当匹配
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  为完全匹配的时候
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  删除这些匹配到的数据
            VT100_MATCH_HINTTAB();                 //  用户回调
            return 2;                              //  完全匹配
        }
        //  非完全匹配
        else
        {
            return 1;
        }
    }
    //  当不匹配
    else
    {
        return 0;       //  不匹配
    }
}

//  整体的接收处理
void VT100_RecvHandle(void)
{
    //  获得当前缓冲区数据的数目
    int recv_len = VT100_GetRecvBufCount();

    //  当缓冲区为空
    if(recv_len <= 0) return;

    //  定义匹配的计数器
    int match_cnt = 0;
    int match_re = 0;

    //  执行匹配操作

    //  匹配上键                               当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Up(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配下键                               当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Down(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配左键                               当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Left(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配右键                               当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Right(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配Delete键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Delete(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配PageUp键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_PageUp(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配PageDown键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_PageDown(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配Home键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Home(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配End键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_End(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配Insert键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_Insert(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F1键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F1(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F2键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F2(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F3键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F3(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F4键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F4(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F5键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F5(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F6键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F6(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F7键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F7(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F8键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F8(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F9键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F9(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F10键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F10(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F11键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F11(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配F12键                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_F12(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配获取光标位置                           当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_ReportCursorPos(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  匹配连续按n个Tab键的触发提示功能         当完全匹配，退出                否则统计匹配结果
    match_re = VT100_RecvMatch_HintTab(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}

    //  当没有找到任何一个匹配
    if(match_cnt <= 0)
    {
        //  获取当前缓冲区中的1个字符
        unsigned char tmp_char = VT100_ReadOutRecvBuf_WithoutDelete(0);

        //  读取完成后将其删除
        VT100_RecvBufDeleteReadOut(1);

        //  检查当前字符是否为  常规控制字符范围
        if(((0x01 <= tmp_char) && (tmp_char <= 0x1F)) || (tmp_char == 0x7F))
        {
            VT100_USER_CTRL_CHAR(tmp_char);
        }
        //  当为 常规的可显示的ASCII字符范围
        else if((0x20 <= tmp_char) && (tmp_char <= 0x7E))
        {
            VT100_USER_ASCII_CHAR(tmp_char);
        }
        //  否则其他特殊字符
        else
        {
            VT100_USER_OTHER_CHAR(tmp_char);
        }

        //  全部字符的回调处理
        VT100_USER_ALL_CHAR(tmp_char);
    }
}
