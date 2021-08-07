
//  ����汾 REV 0.1

//-----------------------------------------------------------------------------------
//  ����ͷ�ļ�
#include "../MCU_Shell/MCU_Shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "ics.h"
#include "uart.h"

#include "derivative.h" /* include peripheral declarations SSKEAZN64M2 */


//  ���巢������
void Send_UART1(char* pdat, int len);

//-----------------------------------------------------------------------------------
//  �û�����ص�����
//  �������ն��豸��������
void VT100_SEND_TO_PHY_DEVICE(unsigned char* pdat, int len)
{
    //  ���÷��ͺ���
    //COM_Send(pdat, len);
	Send_UART1(pdat, len);
}

//  ���뻺�����Ѿ�����
void VT100_RECV_BUF_FULL(void)
{
    //USER_UI_printf("----VT100_RECV_BUF_FULL----\n");
}

//  ʶ�����ڿ����ַ�
void VT100_USER_CTRL_CHAR(unsigned char in)
{
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  ��Ϊbackspace���µ�ʱ��
        if(in == VT100_KEY_BACKSPACE)
        {
            //  ִ��backspace
            int re = ShellCmd_KeyBackspace();

            //  ���ִ�н��
            if(re == 1)
            {
                //  ���û���
                VT100_SEND_TO_PHY_DEVICE(&in, 1);

                //  ������
                VT100_Send_SaveCursorPos();

                //  ˢ����ʾ
                ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

                //  �ָ����
                VT100_Send_UnsaveCursorPos();
            }
        }
        //  ��Ϊ�س���
        else if(in == VT100_CTRL_ENTER_R)
        {
            //  ִ�лس�
            int re = ShellCmd_KeyEnter();

            //  ��û�гɹ�ִ�������ʱ��
            if(re <= 0)
            {
                //  ���ͻ���
                unsigned char tmp_char = VT100_CTRL_ENTER_N;
                VT100_SEND_TO_PHY_DEVICE(&tmp_char, 1);
            }

            //  ˢ����ʾ
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

            //  �����ʷ���λ��
#if CMD_HISTORY_EN
            CmdHistory_CleanHistoryPos();
#endif
        }
        //  ��������
        else
        {
            //USER_UI_printf("Recv Other Ctrl char 0x%02X\n", in & 0x0FF);
        }
    }
}

//  ʶ���û�����ʾ��ASCII�ַ�
void VT100_USER_ASCII_CHAR(unsigned char in)
{
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  ����shell������
        ShellCmd_InsertChar(in);

        //  ���û���
        VT100_SEND_TO_PHY_DEVICE(&in, 1);

        //  ������
        VT100_Send_SaveCursorPos();

        //  ˢ����ʾ
        ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

        //  �ָ����
        VT100_Send_UnsaveCursorPos();
    }
}

//  ʶ�������û��ַ�
void VT100_USER_OTHER_CHAR(unsigned char in)
{
    //USER_UI_printf("Recv Ohter User char 0x%02X\n", in & 0x0FF);
}

//  ȫ���ַ��ص�
void VT100_USER_ALL_CHAR(unsigned char in)
{
    //USER_UI_printf("Recv char 0x%02X\n", in & 0x0FF);
}

//  ƥ���ϼ��ص�����
void VT100_MATCH_UP(void)
{
    //USER_UI_printf("Recv Up Key!!\n");

#if CMD_HISTORY_EN
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  ִ����ʷ����
        int re = CmdHistory_HistoryUp();

        //  ��ִ�гɹ�
        if(re == 1)
        {
            //  ��ȡ��ʷ�ַ���
            char* tmp_str = CmdHistory_GetHistoryCurStr();

            //  ���shell��ǰ�е�ԭʼ����
            ShellCmd_EndOfCmdLine();

            //  ������ַ�����shell��
            ShellCmd_InsertString(tmp_str);

            //  ˢ����ʾ
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
        }
        //  ִ��ʧ��
        else
        {
            //  ��ʱ �ﵽ��˵�ʱ�򣬲����
        }
    }
#endif
}

//  ƥ���¼��ص�����
void VT100_MATCH_DOWN(void)
{
    //USER_UI_printf("Recv Down Key!!\n");

#if CMD_HISTORY_EN
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  ִ����ʷ����
        int re = CmdHistory_HistoryDown();

        //  ��ִ�гɹ�
        if(re == 1)
        {
            //  ��ȡ��ʷ�ַ���
            char* tmp_str = CmdHistory_GetHistoryCurStr();

            //  ���shell��ǰ�е�ԭʼ����
            ShellCmd_EndOfCmdLine();

            //  ������ַ�����shell��
            ShellCmd_InsertString(tmp_str);

            //  ˢ����ʾ
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
        }
    }
    //  ִ��ʧ��
    else
    {
        //  ���shell��ǰ�е�ԭʼ����
        ShellCmd_EndOfCmdLine();

        //  ˢ����ʾ
        ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
    }
#endif
}

//  ƥ������ص�����
void VT100_MATCH_LEFT(void)
{
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  ������ʾ
        //USER_UI_printf("Recv Left Key!!\n");

        //  ����shell������
        int re = ShellCmd_MoveCursorLeft();

        //  �����óɹ�
        if(re == 1)
        {
            //  �������ƶ�
            VT100_Send_CursorLeft(1);
        }
    }
}

//  ƥ���Ҽ��ص�����
void VT100_MATCH_RIGHT(void)
{
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //  ������ʾ
        //USER_UI_printf("Recv Right Key!!\n");

        //  ����shell������
        int re = ShellCmd_MoveCursorRight();

        //  �����óɹ�
        if(re == 1)
        {
            //  �������ƶ�
            VT100_Send_CursorRight(1);
        }
    }
}

//  ƥ��Delete���ص�����
void VT100_MATCH_DELETE(void)
{
    //  ��������Ϊ���е�ʱ��
    if(ShellCmd_GetCmdLineStatus() == SHELL_CMD_LINE_STATUS_READY)
    {
        //USER_UI_printf("Recv Delete Key!!\n");

        //  ִ��delete
        int re = ShellCmd_KeyDelete();

        //  ����Ƿ�ɹ�
        if(re == 1)
        {
            //  ������
            VT100_Send_SaveCursorPos();

            //  ˢ����ʾ
            ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);

            //  �ָ����
            VT100_Send_UnsaveCursorPos();
        }
    }
}

//  ƥ��PageUp���ص�����
void VT100_MATCH_PAGEUP(void)
{
    //USER_UI_printf("Recv PageUp Key!!\n");
}

//  ƥ��PageDown���ص�����
void VT100_MATCH_PAGEDOWN(void)
{
    //USER_UI_printf("Recv PageDown Key!!\n");
}

//  ƥ��Home���ص�����
void VT100_MATCH_HOME(void)
{
    //USER_UI_printf("Recv Home Key!!\n");
}

//  ƥ��End���ص�����
void VT100_MATCH_END(void)
{
    //USER_UI_printf("Recv End Key!!\n");
}

//  ƥ��Insert���ص�����
void VT100_MATCH_INSERT(void)
{
    //USER_UI_printf("Recv Insert Key!!\n");
}

//  ƥ��F1���ص�����
void VT100_MATCH_F1(void)
{
    //USER_UI_printf("Recv F1 Key!!\n");
}

//  ƥ��F2���ص�����
void VT100_MATCH_F2(void)
{
    //USER_UI_printf("Recv F2 Key!!\n");
}

//  ƥ��F3���ص�����
void VT100_MATCH_F3(void)
{
    //USER_UI_printf("Recv F3 Key!!\n");
}

//  ƥ��F4���ص�����
void VT100_MATCH_F4(void)
{
    //USER_UI_printf("Recv F4 Key!!\n");
}

//  ƥ��F5���ص�����
void VT100_MATCH_F5(void)
{
    //USER_UI_printf("Recv F5 Key!!\n");
}

//  ƥ��F6���ص�����
void VT100_MATCH_F6(void)
{
    //USER_UI_printf("Recv F6 Key!!\n");
}

//  ƥ��F7���ص�����
void VT100_MATCH_F7(void)
{
    //USER_UI_printf("Recv F7 Key!!\n");
}

//  ƥ��F8���ص�����
void VT100_MATCH_F8(void)
{
   // USER_UI_printf("Recv F8 Key!!\n");
}

//  ƥ��F9���ص�����
void VT100_MATCH_F9(void)
{
    //USER_UI_printf("Recv F9 Key!!\n");
}

//  ƥ��F10���ص�����
void VT100_MATCH_F10(void)
{
    //USER_UI_printf("Recv F10 Key!!\n");
}

//  ƥ��F11���ص�����
void VT100_MATCH_F11(void)
{
    //USER_UI_printf("Recv F11 Key!!\n");
}

//  ƥ��F12���ص�����
void VT100_MATCH_F12(void)
{
    //USER_UI_printf("Recv F12 Key!!\n");
}

//  ƥ�䵽������λ��
void VT100_MATCH_REPORT_CURSOR_POS(int x, int y)
{
    //USER_UI_printf("Recv Cursor Pos X:%d  Y:%d\n", x, y);
}

//  ƥ�䵽��������N��TAB����ʾ
void VT100_MATCH_HINTTAB(void)
{
    //USER_UI_printf("Recv Hint Message!!\n");
}


//-----------------------------------------------------------------------------------
//  VT100�ĵײ��װ����  ���Ͳ���
//  ����һ���ַ�
void VT100_Send_OneChar(char in)
{
    VT100_SEND_TO_PHY_DEVICE(&in, 1);
}

//  ����һ���ַ���
void VT100_Send_String(char* str)
{
    int str_len = strlen(str);
    VT100_SEND_TO_PHY_DEVICE(str, str_len);
}

//  VT100��printf֧��
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

//  �����Ļ
void VT100_Send_ClearScreen(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x32, 0x4A};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ���һ��
void VT100_Send_ClearLine(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x32, 0x4B};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ���ع��
void VT100_Send_HideCursor(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x3F, 0x32, 0x35, 0x6C};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ��ʾ���
void VT100_Send_ShowCursor(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x3F, 0x32, 0x35, 0x68};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  �������λ��   char_num�ƶ����ַ���Ŀ    1����   dir_code�ƶ�����Ĵ���
void VT100_Send_CursorRelatiovePos(int char_num, unsigned char dir_code)
{
    //  ������
    if(char_num <= 0) return;
    if(char_num > 999) return;

    //  ����VT100����
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x00, 0x00, 0x00, 0x00};

    //  �����ƶ�����Ĵ���
    memset(tmp_buf + 2, dir_code, 4);

    //  �����ƶ���Ŀ
    int num_buf[3];
    num_buf[0] = (char_num / 100) % 10;
    num_buf[1] = (char_num / 10) % 10;
    num_buf[2] = (char_num / 1) % 10;

    //  ���㷢������,��������ֵ
    int send_len = 0;
    //  ������Ч����Ϊ3λ����
    if(char_num > 99)
    {
        send_len = 3 + 3;
        tmp_buf[2] = num_buf[0] + 0x30;
        tmp_buf[3] = num_buf[1] + 0x30;
        tmp_buf[4] = num_buf[2] + 0x30;
    }
    //  ������Ч����Ϊ2λ����
    else if(char_num > 9)
    {
        send_len = 2 + 3;
        tmp_buf[2] = num_buf[1] + 0x30;
        tmp_buf[3] = num_buf[2] + 0x30;
    }
    //  ������Ч����Ϊ1λ����
    else
    {
        send_len = 1 + 3;
        tmp_buf[2] = num_buf[2] + 0x30;
    }

    //  ��������
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, send_len);
}


//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorUp(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x41);
}

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorDown(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x42);
}

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorLeft(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x44);
}

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorRight(int char_num)
{
    VT100_Send_CursorRelatiovePos(char_num, 0x43);
}

//  ������ƶ���ָ��λ��  1����
void VT100_Send_CursorMoveTo(int x, int y)
{
    //  ������
    if(x <= 0) return;          if(x > 999) return;
    if(y <= 0) return;          if(y > 999) return;

    //  ����VT100����
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    //  �����ƶ���Ŀ
    int x_buf[3];
    x_buf[0] = (x / 100) % 10;
    x_buf[1] = (x / 10) % 10;
    x_buf[2] = (x / 1) % 10;
    int y_buf[3];
    y_buf[0] = (y / 100) % 10;
    y_buf[1] = (y / 10) % 10;
    y_buf[2] = (y / 1) % 10;

    //  ����x����,��������ֵ
    int send_len = 2;
    //  ������Ч����Ϊ3λ����
    if(y > 99)
    {
        send_len += 3;
        tmp_buf[2] = y_buf[0] + 0x30;
        tmp_buf[3] = y_buf[1] + 0x30;
        tmp_buf[4] = y_buf[2] + 0x30;
    }
    //  ������Ч����Ϊ2λ����
    else if(y > 9)
    {
        send_len += 2;
        tmp_buf[2] = y_buf[1] + 0x30;
        tmp_buf[3] = y_buf[2] + 0x30;
    }
    //  ������Ч����Ϊ1λ����
    else
    {
        send_len += 1;
        tmp_buf[2] = y_buf[2] + 0x30;
    }

    //  ���÷ֺ�
    tmp_buf[send_len] = 0x3B;
    send_len++;

    //  ����y����,��������ֵ
    //  ������Ч����Ϊ3λ����
    if(x > 99)
    {
        tmp_buf[send_len + 0] = x_buf[0] + 0x30;
        tmp_buf[send_len + 1] = x_buf[1] + 0x30;
        tmp_buf[send_len + 2] = x_buf[2] + 0x30;
        send_len += 3;
    }
    //  ������Ч����Ϊ2λ����
    else if(x > 9)
    {
        tmp_buf[send_len + 0] = x_buf[1] + 0x30;
        tmp_buf[send_len + 1] = x_buf[2] + 0x30;
        send_len += 2;
    }
    //  ������Ч����Ϊ1λ����
    else
    {
        tmp_buf[send_len + 0] = x_buf[2] + 0x30;
        send_len += 1;
    }

    //  ������ĸ��β
    tmp_buf[send_len] = 0x48;
    send_len++;

    //  ��������
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, send_len);
}

//  ��ȡ��ǰ����λ��
void VT100_Send_GetCursorPos(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x36, 0x6E};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ��������
void VT100_Send_EnableWrap(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x37, 0x68};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  �رջ���
void VT100_Send_DisableWrap(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x37, 0x6C};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ������ʾ����
void VT100_Send_DisplayAttributes(EVT100DispAttrib attrib)
{
    //  ��ȡ��ǰ�����Դ���ֵ
    int code = (int)attrib;

    //  �������ֵ
    if(code < 0) return;
    if(code > 99) return;

    //  ����VT100����
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x6D, 0x6D, 0x6D};

    //  ������Ŀ
    int num_buf[2];
    num_buf[0] = (code / 10) % 10;
    num_buf[1] = (code / 1) % 10;

    //  ���㷢����Ŀ���������ֵ
    int send_len = 0;
    //  ��Ϊ2λ��
    if(code > 9)
    {
        send_len = 3 + 2;
        tmp_buf[2] = num_buf[0] + 0x30;
        tmp_buf[3] = num_buf[1] + 0x30;
    }
    //  ��Ϊ1λ��
    else
    {
        send_len = 3 + 1;
        tmp_buf[2] = num_buf[1] + 0x30;
    }

    //  ��������
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, send_len);
}

//  �����˸�
void VT100_Send_Backspace(void)
{
    unsigned char tmp_buf[] = {0x7F};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ���ͱ�����λ��
void VT100_Send_SaveCursorPos(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x73};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//  ���ͻָ����λ��
void VT100_Send_UnsaveCursorPos(void)
{
    unsigned char tmp_buf[] = {0x1B, 0x5B, 0x75};
    VT100_SEND_TO_PHY_DEVICE(tmp_buf, sizeof(tmp_buf));
}

//-----------------------------------------------------------------------------------
//  VT100�ĵײ��װ����  ���ղ���

//  ������ջ��������
static unsigned char VT100_RecvBuf[VT100_RECV_BUF_SIZE];        //  ���λ�����
static int VT100_RecvBuf_in_pos = 0;                            //  ���λ��
static int VT100_RecvBuf_out_pos = 0;                           //  ����λ��

//  ��ȡ��������ǰ�Ѿ����ڵ��ֽڸ���
int VT100_GetRecvBufCount(void)
{
    //  ��λ����ͬ
    if(VT100_RecvBuf_in_pos == VT100_RecvBuf_out_pos)
    {
        //  ������Ϊ��
        return 0;
    }
    //  ������λ�ô��ڶ���λ��
    else if(VT100_RecvBuf_in_pos > VT100_RecvBuf_out_pos)
    {
        return VT100_RecvBuf_in_pos - VT100_RecvBuf_out_pos;
    }
    //  ���򣬵�����λ�ô��ڲ���λ��
    else
    {
        return (VT100_RECV_BUF_SIZE - VT100_RecvBuf_out_pos) + VT100_RecvBuf_in_pos;
    }
}

//  ����һ���ַ�
void VT100_InsertRecvBuf(unsigned char in)
{
    //  ��黺�����Ѿ�ʹ�õ��ֽ�
    int already_len = VT100_GetRecvBufCount();

    //  ��黺�����Ƿ���
    if(already_len >= (VT100_RECV_BUF_SIZE - 1))
    {
        //  �����û��ص���������ʾ�������Ѿ���
        VT100_RECV_BUF_FULL();

        //  ����
        return;
    }

    //  ִ�в������
    VT100_RecvBuf[VT100_RecvBuf_in_pos] = in;

    //  ������һ�εĲ���λ��
    if(VT100_RecvBuf_in_pos >= (VT100_RECV_BUF_SIZE - 1))  VT100_RecvBuf_in_pos = 0;
    else                                                   VT100_RecvBuf_in_pos++;
}

//  ����һ���ַ�����ɾ��       posΪ�Ӷ�����ʼ��λ�ã�0����   �ɹ�����0~255������ʧ�ܷ���-1
static int VT100_ReadOutRecvBuf_WithoutDelete(int pos)
{
    //  ��黺�����Ѿ�ʹ�õ��ֽ�
    int already_len = VT100_GetRecvBufCount();

    //  ��������λ���Ƿ񳬳���������Χ
    if(already_len <= 0) return -1;
    if((pos + 1) > already_len) return -1;
    if(pos < 0) return -1;

    //  ������ʵ�ʵ��λ��
    int read_pos = VT100_RecvBuf_out_pos + pos;
    if(read_pos > (VT100_RECV_BUF_SIZE - 1)) read_pos = read_pos - VT100_RECV_BUF_SIZE;
    if(read_pos > (VT100_RECV_BUF_SIZE - 1)) return -1;

    //  ȡ������
    return VT100_RecvBuf[read_pos] & 0x0FF;
}

//  �Ӷ�����ɾ������  cntΪҪɾ���ĸ���  ʧ�ܷ���-1  �ɹ�����0
static int VT100_RecvBufDeleteReadOut(int cnt)
{
    //  ��黺�����Ѿ�ʹ�õ��ֽ�
    int already_len = VT100_GetRecvBufCount();

    //  ��������λ���Ƿ񳬳���������Χ
    if(already_len <= 0) return -1;
    if(cnt > already_len) return -1;
    if(cnt <= 0) return -1;

    //  ����ɾ����ʵ��λ��
    int pos = VT100_RecvBuf_out_pos + cnt;
    if(pos > (VT100_RECV_BUF_SIZE - 1)) pos = pos - VT100_RECV_BUF_SIZE;
    if(pos > (VT100_RECV_BUF_SIZE - 1)) return -1;

    //  ִ�б��
    VT100_RecvBuf_out_pos = pos;

    //  ���سɹ�
    return 0;
}

//  ���ȫ��������
void VT100_ClearRecvBuf(void)
{
    //  ������ӳ���λ��
    VT100_RecvBuf_in_pos = 0;
    VT100_RecvBuf_out_pos = 0;

    //  ��ջ���
    memset(VT100_RecvBuf, 0, sizeof(VT100_RecvBuf));
}

//  ����ƥ�� �ϼ�   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Up(int len)
{
    //  �ϼ���������Ϊ 1B 5B 41

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x41};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_UP();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� �¼�   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Down(int len)
{
    //  �¼���������Ϊ 1B 5B 42

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x42};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_DOWN();                    //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� ���   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Left(int len)
{
    //  �����������Ϊ 1B 5B 44

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x44};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_LEFT();                    //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� �Ҽ�   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Right(int len)
{
    //  �Ҽ���������Ϊ 1B 5B 43

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x43};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_RIGHT();                   //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� Delete��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Delete(int len)
{
    //  Delete����������Ϊ 1B 5B 33 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x33, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_DELETE();                  //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}


//  ����ƥ�� PageUp��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_PageUp(int len)
{
    //  PageUp����������Ϊ 1B 5B 35 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x35, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_PAGEUP();                  //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� PageDown��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_PageDown(int len)
{
    //  PageDown����������Ϊ 1B 5B 36 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x36, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_PAGEDOWN();                //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� Home��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Home(int len)
{
    //  Home����������Ϊ 1B 5B 31 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_HOME();                    //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� End��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_End(int len)
{
    //  End����������Ϊ 1B 5B 34 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x34, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_END();                     //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� Insert��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_Insert(int len)
{
    //  Insert����������Ϊ 1B 5B 32 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_INSERT();                  //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F1��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F1(int len)
{
    //  F1����������Ϊ 1B 5B 31 31 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x31, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F1();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F2��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F2(int len)
{
    //  F2����������Ϊ 1B 5B 31 31 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x32, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F2();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F3��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F3(int len)
{
    //  F3����������Ϊ 1B 5B 31 33 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x33, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F3();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F4��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F4(int len)
{
    //  F4����������Ϊ 1B 5B 31 34 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x34, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F4();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F5��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F5(int len)
{
    //  F5����������Ϊ 1B 5B 31 35 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x35, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F5();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F6��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F6(int len)
{
    //  F6����������Ϊ 1B 5B 31 37 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x37, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F6();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F7��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F7(int len)
{
    //  F7����������Ϊ 1B 5B 31 38 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x38, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F7();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F8��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F8(int len)
{
    //  F8����������Ϊ 1B 5B 31 39 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x31, 0x39, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F8();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F9��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F9(int len)
{
    //  F9����������Ϊ 1B 5B 32 30 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x30, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F9();                      //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F10��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F10(int len)
{
    //  F10����������Ϊ 1B 5B 32 31 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x31, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F10();                     //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F11��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F11(int len)
{
    //  F11����������Ϊ 1B 5B 32 33 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x33, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F11();                     //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����ƥ�� F12��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_F12(int len)
{
    //  F12����������Ϊ 1B 5B 32 34 7E

    //  ���建����
    const unsigned char target_buf[] = {0x1B, 0x5B, 0x32, 0x34, 0x7E};
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_F12();                     //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ��ĳ����ʼλ�ü���Ƿ�Ϊ����  begin_posΪҪ��ʼ����λ��    pnum_lenΪ���ص����ֳ���
//  ����Ͳ����϶�����0,  ƥ�䵽����Ч���֣�0��ͷ�ģ�������1��  ƥ�䵽��Ч���֣���0��ͷ�ģ�����2
static int VT100_Recv_CheckNum(int begin_pos, int* pnum_len)
{
    //  ��õ�ǰ����������Ч������Ŀ
    int total_len = VT100_GetRecvBufCount();

    //  ���
    if(total_len <= 0) return 0;
    if(begin_pos < 0) return 0;
    if(begin_pos > (total_len - 1)) return 0;
    if(pnum_len == 0) return 0;

    //  ���巵��ֵ
    int re_value = 0;

    //  ���嵱ǰ��ȡ��λ��
    int pos = begin_pos;

    //  ����һ��
    unsigned char tmp_byte = VT100_ReadOutRecvBuf_WithoutDelete(pos);
    //  ��Ϊ�Ϸ�����Ч����
    if((0x31 <= tmp_byte) && (tmp_byte <= 0x39))
    {
        pnum_len[0] = 1;       //  �ҵ�1������
        re_value = 2;          //  ����δ���ķ���ֵ
        pos++;                 //  ͳ���Ѿ�������ַ�
    }
    //  ��Ϊ�Ϸ�����,����������Ч����
    else if(tmp_byte == 0x30)
    {
        pnum_len[0] = 1;       //  �ҵ�1������
        re_value = 1;          //  ����δ���ķ���ֵ
        pos++;                 //  ͳ���Ѿ�������ַ�
    }
    //  �����ַ�
    else
    {
        pnum_len[0] = 0;
        return 0;
    }

    //  ����ʣ����ַ�����
    int last_len = (total_len - 1) - pos;

    //  ���ʣ���ֽ�
    if(last_len <= 0) return re_value;

    //  ����ʣ���ַ�
    int i=0;
    for(i=0;i<last_len;i++)
    {
        //  ��ȡһ���ַ�
        tmp_byte = VT100_ReadOutRecvBuf_WithoutDelete(pos);

        //  ��Ϊ�Ϸ�����Ч����
        if((0x31 <= tmp_byte) && (tmp_byte <= 0x39))
        {
            pnum_len[0] += 1;      //  �ҵ�1������
            pos++;                 //  ͳ���Ѿ�������ַ�
        }
        //  ��Ϊ�Ϸ�����,����������Ч����
        else if(tmp_byte == 0x30)
        {
            pnum_len[0] += 1;      //  �ҵ�1������
            pos++;                 //  ͳ���Ѿ�������ַ�
        }
        //  �����ַ�
        else
        {
            return re_value;
        }
    }

    //  ��������Ϣ������
    return re_value;
}

//  ��ASCII�ַ���ת��Ϊint������,�ɹ�����1��ʧ�ܷ���0
//  pos_strΪ����ASCII�ַ��������ڻ���������ʼλ��   str_lenΪ�ַ�������Ч����
//  pout_valueΪ�����ֵ��ָ��
static int VT100_ASCIIstrToInt(int pos_str, int str_len, int* pout_value)
{
    //  ��õ�ǰ����������Ч������Ŀ
    int total_len = VT100_GetRecvBufCount();


    //  �������
    if(pos_str < 0) return 0;
    if(pos_str > (total_len - 1)) return 0;
    if(str_len <= 0) return 0;
    if((pos_str + str_len) > (total_len - 1)) return 0;
    if(pout_value == 0) return 0;

    //  ���ۼ���ֻ����3λ����
    if(str_len > 3) return 0;

    //  ��ȡASCII����
    unsigned char tmp_buf[3];
    //  ��λ����
    if(str_len == 3)
    {
        tmp_buf[0] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 0);
        tmp_buf[1] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 1);
        tmp_buf[2] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 2);
    }
    //  ��λ����
    else if(str_len == 2)
    {
        tmp_buf[0] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 0);
        tmp_buf[1] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 1);
    }
    //  һλ����
    else
    {
        tmp_buf[0] = VT100_ReadOutRecvBuf_WithoutDelete(pos_str + 0);
    }

    //  ������뷶Χ
    //  ��λ����
    if(str_len == 3)
    {
        if((tmp_buf[0] < 0x30) || (tmp_buf[0] > 0x39)) return 0;
        if((tmp_buf[1] < 0x30) || (tmp_buf[1] > 0x39)) return 0;
        if((tmp_buf[2] < 0x30) || (tmp_buf[2] > 0x39)) return 0;
    }
    //  ��λ����
    else if(str_len == 2)
    {
        if((tmp_buf[0] < 0x30) || (tmp_buf[0] > 0x39)) return 0;
        if((tmp_buf[1] < 0x30) || (tmp_buf[1] > 0x39)) return 0;
    }
    //  һλ����
    else
    {
        if((tmp_buf[0] < 0x30) || (tmp_buf[0] > 0x39)) return 0;
    }

    //  ���
    pout_value[0] = 0;
    //  ��λ����
    if(str_len == 3)
    {
        pout_value[0] += ((tmp_buf[0] & 0x0FF) - 0x30) * 100;
        pout_value[0] += ((tmp_buf[1] & 0x0FF) - 0x30) * 10;
        pout_value[0] += ((tmp_buf[2] & 0x0FF) - 0x30) * 1;
    }
    //  ��λ����
    else if(str_len == 2)
    {
        pout_value[0] += ((tmp_buf[0] & 0x0FF) - 0x30) * 10;
        pout_value[0] += ((tmp_buf[1] & 0x0FF) - 0x30) * 1;
    }
    //  һλ����
    else
    {
        pout_value[0] += ((tmp_buf[0] & 0x0FF) - 0x30) * 1;
    }

    //  ���سɹ�
    return 1;
}

//  ����ƥ�� ������λ��   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_ReportCursorPos(int len)
{
    //  һ����յ�  0x1B 0x5B 0x32 0x3B 0x36 0x52
    //  �������ĸ�ʽ�����������Ϊ 0x1B 0x5B ��ͷ�� Ȼ����һЩASCII��ʽ�����֣�Ȼ����0x3B��Ȼ������ASCII��ʽ�����֣�����βΪ0x52
    //  ����ٶ�X��Y�Ĺ��λ�����Ϊ3λ���֣����ұ����Է�0����Ч���ֿ�ͷ

    //  ���
    if(len <= 0) return 0;

    //  ������Ч���ֵĳ���
    int x_begin = 0;
    int x_len = 0;
    int y_begin = 0;
    int y_len = 0;

    //  �Ѿ����ҵ����ݳ���
    int already_byte_cnt = 0;

    //  �����ҵ������ֵ���ʱ����
    int tmp_len = 0;

    //  ����Ч����Ϊ1���ֽڵ�ʱ��
    if(len == 1)
    {
        if(VT100_ReadOutRecvBuf_WithoutDelete(0) == 0x1B) return 1;   //  ƥ��һ����
        else                                              return 0;   //  ��ƥ��
    }
    //  ����Ч����Ϊ2���ֽڵ�ʱ��
    else if(len == 2)
    {
        if((VT100_ReadOutRecvBuf_WithoutDelete(0) == 0x1B) &&
           (VT100_ReadOutRecvBuf_WithoutDelete(1) == 0x5B)) return 1;   //  ƥ��һ����
        else                                                return 0;   //  ��ƥ��
    }
    //  ����Ч���ȴ���2���ֽڵ�ʱ��
    else
    {
        //  ������ͷƥ��
        if((VT100_ReadOutRecvBuf_WithoutDelete(0) == 0x1B) &&
           (VT100_ReadOutRecvBuf_WithoutDelete(1) == 0x5B))
        {
            //  �����Ѿ����������ݳ���
            already_byte_cnt = 2;

            //  ����ڴ�λ�����Ƿ������Ч����
            int re = VT100_Recv_CheckNum(already_byte_cnt, &tmp_len);

            //  ��������Ƿ��ȡ�ɹ�������ȡ�ɹ���ƥ�䵽��Ч����
            if(re == 2)
            {
                //  ƥ��Y����ز���
                y_begin = already_byte_cnt;
                y_len = tmp_len;

                //  ͳ��Ŀǰ�Ѿ��ﵽ��λ��
                already_byte_cnt += tmp_len;

                //  ��鵱ǰλ���Ƿ��Ѿ������β
                if(already_byte_cnt >= len)
                {
                    return 1;  //  ƥ��һ��
                }

                //  ��鱾�ַ��Ƿ�Ϊ����ָ����
                if(VT100_ReadOutRecvBuf_WithoutDelete(already_byte_cnt) != 0x3B) return 0;  //  ��ƥ��

                //  ͳ��Ŀǰ�Ѿ��ﵽ��λ��
                already_byte_cnt += 1;

                //  ��鵱ǰλ���Ƿ��Ѿ������β
                if(already_byte_cnt >= len)
                {
                    return 1;  //  ƥ��һ��
                }

                //  ����ڴ�λ�����Ƿ������Ч����
                re = VT100_Recv_CheckNum(already_byte_cnt, &tmp_len);

                //  ��������Ƿ��ȡ�ɹ�������ȡ�ɹ���ƥ�䵽��Ч����
                if(re == 2)
                {
                    //  ƥ��X����ز���
                    x_begin = already_byte_cnt;
                    x_len = tmp_len;

                    //  ͳ��Ŀǰ�Ѿ��ﵽ��λ��
                    already_byte_cnt += tmp_len;

                    //  ��鵱ǰλ���Ƿ��Ѿ������β
                    if(already_byte_cnt >= len)
                    {
                        return 1;  //  ƥ��һ��
                    }

                    //  ��鱾�ַ��Ƿ�Ϊ����Ľ�β
                    if(VT100_ReadOutRecvBuf_WithoutDelete(already_byte_cnt) != 0x52) return 0;  //  ��ƥ��

                    //  ͳ��Ŀǰ�Ѿ��ﵽ��λ��
                    already_byte_cnt += 1;

                    //  �����������
                    int x_value = 0;
                    int y_value = 0;
                    re = VT100_ASCIIstrToInt(2, y_len, &y_value);
                    if(re == 0) return 0;
                    re = VT100_ASCIIstrToInt(2 + y_len + 1, x_len, &x_value);
                    if(re == 0) return 0;

                    //  ɾ����Щƥ�������
                    VT100_RecvBufDeleteReadOut(2 + y_len + 1 + x_len + 1);

                    //  ��������
                    VT100_MATCH_REPORT_CURSOR_POS(x_value, y_value);

                    //  ��ȫƥ��
                    return 2;
                }
                //  ����ȡʧ�� ���߻�ȡ������Ч����
                else
                {
                    return 0;   //  ��ƥ��
                }
            }
            //  ����ȡʧ�� ���߻�ȡ������Ч����
            else
            {
                return 0;   //  ��ƥ��
            }
        }
        //  ��ƥ��
        else
        {
            return 0;   //  ��ƥ��
        }
    }
}

//  ����ƥ�� ������n��Tab���Ĵ�����ʾ����   lenΪ��ǰ�������������ݵĳ���  δ֪���� �� û��ƥ��ɹ�����0  ƥ��һ�뷵��1  ��ȫƥ�䷵��2
static int VT100_RecvMatch_HintTab(int len)
{
    //  TAB���Ĵ���Ϊ 0x09

    //  ���建����
    unsigned char target_buf[VT100_RECV_HINT_TAB_NUM];
    memset(target_buf, VT100_KEY_TAB, sizeof(target_buf));
    unsigned char tmp_buf[sizeof(target_buf)];

    //  ���
    if(len <= 0) return 0;

    //  ����ʵ�ʶ�ȡ����
    int read_len = 0;
    if(len >= sizeof(tmp_buf)) read_len = sizeof(tmp_buf);
    else                       read_len = len;

    //  �����������ݵ�������
    int i=0;
    for(i=0;i<read_len;i++)
    {
        tmp_buf[i] = VT100_ReadOutRecvBuf_WithoutDelete(i);
    }

    //  �Աȣ���ƥ��
    if(memcmp(tmp_buf, target_buf, read_len) == 0)
    {
        //  Ϊ��ȫƥ���ʱ��
        if(read_len == sizeof(tmp_buf))
        {
            VT100_RecvBufDeleteReadOut(read_len);  //  ɾ����Щƥ�䵽������
            VT100_MATCH_HINTTAB();                 //  �û��ص�
            return 2;                              //  ��ȫƥ��
        }
        //  ����ȫƥ��
        else
        {
            return 1;
        }
    }
    //  ����ƥ��
    else
    {
        return 0;       //  ��ƥ��
    }
}

//  ����Ľ��մ���
void VT100_RecvHandle(void)
{
    //  ��õ�ǰ���������ݵ���Ŀ
    int recv_len = VT100_GetRecvBufCount();

    //  ��������Ϊ��
    if(recv_len <= 0) return;

    //  ����ƥ��ļ�����
    int match_cnt = 0;
    int match_re = 0;

    //  ִ��ƥ�����

    //  ƥ���ϼ�                               ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Up(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ���¼�                               ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Down(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ�����                               ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Left(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ���Ҽ�                               ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Right(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��Delete��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Delete(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��PageUp��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_PageUp(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��PageDown��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_PageDown(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��Home��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Home(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��End��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_End(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��Insert��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_Insert(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F1��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F1(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F2��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F2(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F3��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F3(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F4��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F4(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F5��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F5(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F6��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F6(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F7��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F7(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F8��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F8(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F9��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F9(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F10��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F10(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F11��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F11(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��F12��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_F12(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ���ȡ���λ��                           ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_ReportCursorPos(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}
    //  ƥ��������n��Tab���Ĵ�����ʾ����         ����ȫƥ�䣬�˳�                ����ͳ��ƥ����
    match_re = VT100_RecvMatch_HintTab(recv_len); if(match_re == 2) {return;} else {match_cnt += match_re;}

    //  ��û���ҵ��κ�һ��ƥ��
    if(match_cnt <= 0)
    {
        //  ��ȡ��ǰ�������е�1���ַ�
        unsigned char tmp_char = VT100_ReadOutRecvBuf_WithoutDelete(0);

        //  ��ȡ��ɺ���ɾ��
        VT100_RecvBufDeleteReadOut(1);

        //  ��鵱ǰ�ַ��Ƿ�Ϊ  ��������ַ���Χ
        if(((0x01 <= tmp_char) && (tmp_char <= 0x1F)) || (tmp_char == 0x7F))
        {
            VT100_USER_CTRL_CHAR(tmp_char);
        }
        //  ��Ϊ ����Ŀ���ʾ��ASCII�ַ���Χ
        else if((0x20 <= tmp_char) && (tmp_char <= 0x7E))
        {
            VT100_USER_ASCII_CHAR(tmp_char);
        }
        //  �������������ַ�
        else
        {
            VT100_USER_OTHER_CHAR(tmp_char);
        }

        //  ȫ���ַ��Ļص�����
        VT100_USER_ALL_CHAR(tmp_char);
    }
}
