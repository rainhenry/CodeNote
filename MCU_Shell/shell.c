
//  ����汾 REV 0.1

//-----------------------------------------------------------------------------------
//  ����ͷ�ļ�
#include "../MCU_Shell/MCU_Shell.h"
#include <string.h>


//-----------------------------------------------------------------------------------
//  �û�����Ļص�����
//  ��û���ҵ������ʱ��Ļص���ʾ
static void ShellCmd_NotFoundUserCmd_CallBack(char* cmd_str, int len)
{
    VT100_Send_String("\r\n");
    VT100_Send_String("  Not Found Command ---- '");
    VT100_Send_String(cmd_str);
    VT100_Send_String("'");
}

//-----------------------------------------------------------------------------------
//  ���嶯̬�ַ�����
static SCharUnit ShellCmd_CharBuf[SHELL_CMD_MAX_CHAR];

//  ���嵱ǰ����Ĺ������λ��
static int ShellCmd_CursorPos = 0;

//  ���嵱ǰ������״̬  0=�ȴ��û���������  1=ִ������
static int ShellCmd_CmdLineStatus = SHELL_CMD_LINE_STATUS_READY;

//  ����������״̬
//  inΪָ����״̬��ȡֵΪSHELL_CMD_LINE_STATUS_READY����SHELL_CMD_LINE_STATUS_BUSY
static void ShellCmd_SetCmdLineStatus(int in)
{
    //  ����
    ShellCmd_CmdLineStatus = in;
}

//  ��ȡ������״̬
int ShellCmd_GetCmdLineStatus(void)
{
    return ShellCmd_CmdLineStatus;
}


//  ��ʼ�����
void ShellCmd_Init(void)
{
    //  ��ʼ������
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        ShellCmd_CharBuf[i].dat = -1;
        ShellCmd_CharBuf[i].next = -1;
    }

    //  ��ʼ�����λ��
    ShellCmd_CursorPos = 0;

    //  ���������еĳ�ʼ״̬
    ShellCmd_SetCmdLineStatus(SHELL_CMD_LINE_STATUS_READY);

    //  ��λ��ʷ����
#if CMD_HISTORY_EN
    CmdHistory_Init();
#endif
}

//  ��Ӳ����ʼ����ɺ󣬿�ʼshell
void ShellCmd_ShellBegin(void)
{
    //  �����Ļ
    VT100_Send_ClearScreen();

    //  ����λ
    VT100_Send_CursorMoveTo(1,1);

    //  ˢ����ʾ
    ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
}

//  ����һ������Ĵ���
void ShellCmd_EndOfCmdLine(void)
{
    //  ��ʼ������
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        ShellCmd_CharBuf[i].dat = -1;
        ShellCmd_CharBuf[i].next = -1;
    }

    //  ��ʼ�����λ��
    ShellCmd_CursorPos = 0;
}

//  �ҵ������д��������һ���յ� �ɹ����ش��ڵ���0  ʧ�ܷ���-1
static int ShellCmd_FindNullPos(void)
{
    //  ��������
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  ������Ϊ�����õ�ʱ��
        if(ShellCmd_CharBuf[i].dat == -1)
        {
            return i;
        }
    }

    //  ����ʧ��
    return -1;
}

//  �ҵ���β�Ļ���λ�ã� �ɹ����ش��ڵ���0��ʧ�ܷ���-1
static int ShellCmd_FindEndPos(void)
{
    //  ����һ��λ���Ƿ�Ϊ��,��Ϊ�յ�ʱ��
    if(ShellCmd_CharBuf[0].dat == -1)
    {
        //  ����Ϊ��
        return -1;
    }

    //  ���޴�������
    int cur_pos = 0;        //  ��ǰλ��
    int next_pos = 0;       //  ��һ��λ��
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  ��ȡ�´ε�λ��
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  ����һ��λ��Ϊ�����õ�ʱ��
        if(next_pos == -1)
        {
            //  ��ǰλ�ü�Ϊ��β
            return cur_pos;
        }

        //  ����λ��
        cur_pos = next_pos;
    }

    //  δ�ҵ�ƥ��Ľ�β
    return -1;
}

//  ��ȡ�����Ѿ�ʹ������
int ShellCmd_GetAlreadyUseCount(void)
{
    //  �����Ѿ�ʹ�õ���Ŀ
    int use_cnt = 0;

    //  ��������
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  ������Ϊ���õ�ʱ��
        if(ShellCmd_CharBuf[i].dat != -1)
        {
            //  ͳ�ƿ��õĸ���
            use_cnt++;
        }
    }

    //  ������Ŀ
    return use_cnt;
}

//  ��鵱ǰ���λ�õ����  ʧ�ܷ���-1  �ջ���Ŀ�ʼ����0  �������ĩβ����1   �����м䷵��2
static int ShellCmd_CheckCursorPos(void)
{
    //  ������ر���
    int cur_pos = 0;       //  ��ǰλ��
    int next_pos = 0;      //  ��һ��λ��

    //  �����ָ��0λ��
    if(ShellCmd_CursorPos <= 0) return 0;

    //  �������棬���������ͱ������֮ǰ���ڵ�����
    int i=0;
    for(i=0;i<(ShellCmd_CursorPos-1);i++)
    {
        //  ������Ϊ���õ�ʱ��
        if(ShellCmd_CharBuf[cur_pos].dat != -1)
        {
            //  ��ȡ��һ��λ��
            next_pos = ShellCmd_CharBuf[cur_pos].next;

            //  �����һ��λ�ã���һ��λ�ÿ���
            if(next_pos != -1)
            {
                //  ����λ��
                cur_pos = next_pos;
            }
            //  ��һ��λ�ò�����
            else
            {
                //  ���ָ����һ��������Χ��λ��
                return -1;
            }
        }
        //  ����ǰ���ݲ�����
        else
        {
            //  ���ݳ���
            return -1;
        }
    }

    //  ������Ϸ���
    if(cur_pos != next_pos) return -1;    //  �����˲����ܳ��ֵ�δ֪���

    //  ��ȡ��һ��λ��
    next_pos = ShellCmd_CharBuf[cur_pos].next;

    //  ���ﵽ���λ�ú������һ��λ��Ϊ������
    if(next_pos == -1)
    {
        //  ������ĩβ
        return 1;
    }
    //  ����һ�����λ�ÿ���
    else
    {
        //  �ﵽ�м�λ��
        return 2;
    }
}

//  �ڵ�ǰ���λ���û�����һ���ַ�  �ɹ�����1�� ʧ�ܷ���0
int ShellCmd_InsertChar(unsigned char in)
{
    //  ��鵱ǰ���λ��
    if(ShellCmd_CursorPos >= SHELL_CMD_MAX_CHAR) return 0;

    //  ��黺���Ƿ�����
    int total_len = ShellCmd_GetAlreadyUseCount();
    if(total_len >= SHELL_CMD_MAX_CHAR) return 0;

    //  ��鵱ǰ�������λ��
    int cursor_pos_type = ShellCmd_CheckCursorPos();

    //  ���ڿջ��濪ʼ��λ��
    if(cursor_pos_type == 0)
    {
        /*
        //  ʹ�õ�һ��λ��
        ShellCmd_CharBuf[0].dat = in & 0x0FF;
        ShellCmd_CharBuf[0].next = -1;

        //  ���ù��λ��
        ShellCmd_CursorPos = 1;

        //  �����ɹ�
        return 1;
        */

        //  ����һ��λ���Ƿ��Ѿ�ʹ�ã����Ѿ�ʹ��
        if(ShellCmd_CharBuf[0].dat != -1)
        {
            //  �ҵ������һ����λ��
            int null_char_pos = ShellCmd_FindNullPos();

            //  ����ȡ��λ��ʧ��
            if(null_char_pos == -1) return 0;

            //  ����λ�õ����ݸ��Ƶ���λ����
            ShellCmd_CharBuf[null_char_pos].dat = ShellCmd_CharBuf[0].dat;
            ShellCmd_CharBuf[null_char_pos].next = ShellCmd_CharBuf[0].next;

            //  ʹ�õ�һ��λ��
            ShellCmd_CharBuf[0].dat = in & 0x0FF;
            ShellCmd_CharBuf[0].next = null_char_pos;

            //  ���ù��λ��
            ShellCmd_CursorPos = 1;

            //  �����ɹ�
            return 1;

        }
        //  ��û�б�ʹ��
        else
        {
            //  ʹ�õ�һ��λ��
            ShellCmd_CharBuf[0].dat = in & 0x0FF;
            ShellCmd_CharBuf[0].next = -1;

            //  ���ù��λ��
            ShellCmd_CursorPos = 1;

            //  �����ɹ�
            return 1;
        }



    }
    //  ���ڻ�����ĩβ
    else if(cursor_pos_type == 1)
    {
        //  �ҵ����һ���ַ���λ��
        int last_char_pos = ShellCmd_FindEndPos();

        //  ����ȡλ��ʧ��
        if(last_char_pos == -1)
        {
            //  ʹ�õ�һ��λ��
            ShellCmd_CharBuf[0].dat = in & 0x0FF;
            ShellCmd_CharBuf[0].next = -1;

            //  ���ù��λ��
            ShellCmd_CursorPos = 1;

            //  �����ɹ�
            return 1;
        }

        //  �ҵ������һ����λ��
        int null_char_pos = ShellCmd_FindNullPos();

        //  ����ȡ��λ��ʧ��
        if(null_char_pos == -1) return 0;

        //  ���ӽ�β�ﵽ��λ��
        ShellCmd_CharBuf[last_char_pos].next = null_char_pos;

        //  �������
        ShellCmd_CharBuf[null_char_pos].dat = in & 0x0FF;
        ShellCmd_CharBuf[null_char_pos].next = -1;

        //  ���ù��λ��
        ShellCmd_CursorPos++;

        //  ����ɹ�
        return 1;
    }
    //  ���ڻ����м�
    else if(cursor_pos_type == 2)
    {
        //  ��鵱ǰ���Ϸ���
        if(ShellCmd_CursorPos <= 0) return 0;
        if(ShellCmd_CursorPos >= total_len) return 0;

        //  �ҵ������һ����λ��
        int null_char_pos = ShellCmd_FindNullPos();

        //  ����ȡ��λ��ʧ��
        if(null_char_pos == -1) return 0;

        //  �ҵ���ǰ���ָ���ǰһ���ַ���λ��
        int cur_pos = 0;       //  ��ǰλ��
        int next_pos = 0;      //  ��һ��λ��
        int i=0;
        for(i=0;i<(ShellCmd_CursorPos-1);i++)
        {
            //  ������Ϊ���õ�ʱ��
            if(ShellCmd_CharBuf[cur_pos].dat != -1)
            {
                //  ��ȡ��һ��λ��
                next_pos = ShellCmd_CharBuf[cur_pos].next;

                //  �����һ��λ�ã���һ��λ�ÿ���
                if(next_pos != -1)
                {
                    //  ����λ��
                    cur_pos = next_pos;
                }
                //  ��һ��λ�ò�����
                else
                {
                    //  ���ָ����һ��������Χ��λ��
                    return 0;
                }
            }
            //  ����ǰ���ݲ�����
            else
            {
                //  ���ݳ���
                return 0;
            }
        }
        //  ������Ϸ���
        if(cur_pos != next_pos) return 0;    //  �����˲����ܳ��ֵ�δ֪���

        //  ��¼��λ��ԭ��ָ�����һ��λ��
        int old_next = ShellCmd_CharBuf[cur_pos].next;

        //  ������λ��
        ShellCmd_CharBuf[cur_pos].next = null_char_pos;
        ShellCmd_CharBuf[null_char_pos].dat = in & 0x0FF;
        ShellCmd_CharBuf[null_char_pos].next = old_next;

        //  ���ù��λ��
        ShellCmd_CursorPos++;

        //  ����ɹ�
        return 1;
    }
    //  ����������
    else
    {
        return 0;
    }

}

//  �����ַ���,�ɹ����ز�����ַ�������,ʧ�ܷ���С�ڵ���0
int ShellCmd_InsertString(char* str)
{
    //  �������
    if(str == 0) return 0;

    //  ��ȡ�ַ����ĳ���
    int str_len = strlen(str);

    //  ��鳤��
    if(str_len <= 0) return 0;

    //  ��������
    int i=0;
    for(i=0;i<str_len;i++)
    {
        ShellCmd_InsertChar(str[i]);
    }

    //  ���سɹ�
    return str_len;
}

//  ����̬�ַ�ת�����ַ���  ����ת���ɹ����ַ�����   pstrΪ����ַ������׵�ַ�� max_bufΪ������������С
int ShellCmd_CurCmdLineToString(char* pstr, int max_buf)
{
    //  ��ȡ��ǰ���������ڵ��ַ�����
    int total_len = ShellCmd_GetAlreadyUseCount();

    //  ����ַ�����
    if(total_len > max_buf) return 0;

    //  ��黺����
    if(pstr == 0) return 0;

    //  ���޴�������
    int cur_pos = 0;        //  ��ǰλ��
    int next_pos = 0;       //  ��һ��λ��
    int out_pos = 0;        //  ����ַ�����λ��
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  ��鵱ǰλ���Ƿ����
        if(ShellCmd_CharBuf[cur_pos].dat == -1)
        {
            //  �����Ѿ�ת��������
            return out_pos;
        }

        //  ת��һ��
        pstr[out_pos] = ShellCmd_CharBuf[cur_pos].dat;
        out_pos++;

        //  ��ȡ�´ε�λ��
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  ����һ��λ��Ϊ�����õ�ʱ��
        if(next_pos == -1)
        {
            //  �����Ѿ�ת��������
            return out_pos;
        }

        //  ����λ��
        cur_pos = next_pos;
    }

    //  �����Ѿ�ת��������
    return out_pos;
}

//  ��ȡ��ǰ���λ��
int ShellCmd_GetCursorPos(void)
{
    return ShellCmd_CursorPos;
}

//  ��������ƶ� �ƶ��ɹ�����1  ʧ�ܷ���0
int ShellCmd_MoveCursorLeft(void)
{
    //  ��鵱ǰ���λ���Ƿ�Ϸ�
    if(ShellCmd_CursorPos <= 0) return 0;

    //  �ƶ�
    ShellCmd_CursorPos--;

    //  ���سɹ�
    return 1;
}

//  ��������ƶ�  �ƶ��ɹ�����1  ʧ�ܷ���0
int ShellCmd_MoveCursorRight(void)
{
    //  ��ȡ�Ѿ�ʹ�õĻ����С
    int total_len = ShellCmd_GetAlreadyUseCount();

    //  ��鵱ǰ�ƶ��Ƿ�Ϸ�
    if(ShellCmd_CursorPos >= total_len) return 0;

    //  �ƶ�
    ShellCmd_CursorPos++;

    //  ���سɹ�
    return 1;
}

//  ˢ�µ�ǰ�е���ʾ  head_strΪ��������ʾ�Ŀ�ͷ�ַ���
void ShellCmd_RefreshLineText(char* head_str)
{
    //  ������ո���
    VT100_Send_ClearLine();

    //  Ȼ���ù���ƶ�������
    unsigned char tmp_char = VT100_CTRL_ENTER_R;
    VT100_SEND_TO_PHY_DEVICE(&tmp_char, 1);

    //  ��ӡ�ַ�ǰ׺ͷ��
    int head_str_len = strlen(head_str);
    VT100_SEND_TO_PHY_DEVICE(head_str, head_str_len);

    //  ��ȡ��ǰ���ڱ༭���������� ��׷�Ӵ�ӡ
    char tmp_cmd[SHELL_CMD_MAX_CHAR];
    memset(tmp_cmd, 0, SHELL_CMD_MAX_CHAR);
    int cmd_len = ShellCmd_CurCmdLineToString(tmp_cmd, SHELL_CMD_MAX_CHAR);
    VT100_SEND_TO_PHY_DEVICE(tmp_cmd, cmd_len);
}

//  ���������ַ������Ƴ�һ���ַ�   posΪҪ�Ƴ��ַ���λ��,0����   ����1��ʾ�Ƴ��ɹ��� ����0��ʾʧ��
int ShellCmd_RemoveOneChar(int pos)
{
    //  ��ȡ��ǰ���������ڵ��ַ�����
    int total_len = ShellCmd_GetAlreadyUseCount();

    //  ����Ƴ�λ��
    if(total_len <= 0) return 0;
    if(pos < 0) return 0;
    if(pos > (total_len - 1)) return 0;

    //  ��Ϊ�Ƴ���һ��
    if(pos == 0)
    {
        //  ����Ԫ�ر����Ƿ�Ϸ�
        if(ShellCmd_CharBuf[0].dat == -1) return 0;

        //  ��ȡ��һ��ָ���Ԫ��λ��
        int next_index = ShellCmd_CharBuf[0].next;

        //  ����Ԫ���Ƿ�Ϸ�����ָ���Ԫ�ز��Ϸ���ɾ�����Լ�
        if(next_index == -1)
        {
            //  ɾ����Ԫ��
            ShellCmd_CharBuf[0].dat = -1;
            ShellCmd_CharBuf[0].next = -1;
        }
        //  ��ָ���Ԫ�غϷ���ƴ�ӵ��ײ�
        else
        {
            //  ��ȡ��һ��Ԫ�ص��������
            int tmp_dat = ShellCmd_CharBuf[next_index].dat;
            int tmp_next = ShellCmd_CharBuf[next_index].next;

            //  ���Ƶ���һ��Ԫ������
            ShellCmd_CharBuf[0].dat = tmp_dat;
            ShellCmd_CharBuf[0].next = tmp_next;

            //  �ͷſռ�
            ShellCmd_CharBuf[next_index].dat = -1;
            ShellCmd_CharBuf[next_index].next = -1;
        }

    }
    //  ��Ϊ�Ƴ����һ��
    else if(pos == (total_len - 1))
    {
        //  �ҵ���ǰ���ָ���ǰһ���ַ���λ��
        int cur_pos = 0;       //  ��ǰλ��
        int next_pos = 0;      //  ��һ��λ��
        int i=0;
        for(i=0;i<(pos-1);i++)
        {
            //  ������Ϊ���õ�ʱ��
            if(ShellCmd_CharBuf[cur_pos].dat != -1)
            {
                //  ��ȡ��һ��λ��
                next_pos = ShellCmd_CharBuf[cur_pos].next;

                //  �����һ��λ�ã���һ��λ�ÿ���
                if(next_pos != -1)
                {
                    //  ����λ��
                    cur_pos = next_pos;
                }
                //  ��һ��λ�ò�����
                else
                {
                    //  ���ָ����һ��������Χ��λ��
                    return 0;
                }
            }
            //  ����ǰ���ݲ�����
            else
            {
                //  ���ݳ���
                return 0;
            }
        }
        //  ������Ϸ���
        if(cur_pos != next_pos) return 0;    //  �����˲����ܳ��ֵ�δ֪���

        //  ��ȡ��ǰָ�����һ��λ��
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  ���λ�úϷ��ԣ����Ϸ�
        if(next_pos != -1)
        {
            //  �����һ��λ���Ƿ�Ϊ��β
            //  �����ǽ�β
            if(ShellCmd_CharBuf[next_pos].next != -1) return 0;  //  ����δ֪����

            //  �ͷ������λ��
            ShellCmd_CharBuf[next_pos].dat = -1;
            ShellCmd_CharBuf[next_pos].next = -1;

            //  ��ӽ�β
            ShellCmd_CharBuf[cur_pos].next = -1;
        }
        //  ��λ�ò��Ϸ�
        else
        {
            //  ɾ��ʧ��
            return 0;
        }
    }
    //  ��Ϊ�Ƴ��м�λ�õ�
    else
    {
        //  �ҵ���ǰ���ָ���ǰһ���ַ���λ��
        int cur_pos = 0;       //  ��ǰλ��
        int next_pos = 0;      //  ��һ��λ��
        int i=0;
        for(i=0;i<(pos-1);i++)
        {
            //  ������Ϊ���õ�ʱ��
            if(ShellCmd_CharBuf[cur_pos].dat != -1)
            {
                //  ��ȡ��һ��λ��
                next_pos = ShellCmd_CharBuf[cur_pos].next;

                //  �����һ��λ�ã���һ��λ�ÿ���
                if(next_pos != -1)
                {
                    //  ����λ��
                    cur_pos = next_pos;
                }
                //  ��һ��λ�ò�����
                else
                {
                    //  ���ָ����һ��������Χ��λ��
                    return 0;
                }
            }
            //  ����ǰ���ݲ�����
            else
            {
                //  ���ݳ���
                return 0;
            }
        }
        //  ������Ϸ���
        if(cur_pos != next_pos) return 0;    //  �����˲����ܳ��ֵ�δ֪���

        //  ��ȡ��ǰָ�����һ��λ��
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  ���λ�úϷ��ԣ����Ϸ�
        if(next_pos != -1)
        {
            //  �����һ��λ���Ƿ�Ϊ��β
            //  ���ǽ�β
            if(ShellCmd_CharBuf[next_pos].next == -1) return 0;  //  ����δ֪����

            //  ����ƴ��λ��
            int tmp_next = ShellCmd_CharBuf[next_pos].next;

            //  �ͷ������λ��
            ShellCmd_CharBuf[next_pos].dat = -1;
            ShellCmd_CharBuf[next_pos].next = -1;

            //  ƴ��
            ShellCmd_CharBuf[cur_pos].next = tmp_next;
        }
        //  ��λ�ò��Ϸ�
        else
        {
            //  ɾ��ʧ��
            return 0;
        }
    }

    //  ���سɹ�
    return 1;
}

//  ɾ������  ����1��ʾ��Ч  ����0��ʾ��Ч
int ShellCmd_KeyDelete(void)
{
    //  ɾ���������λ�õ��ַ�
    return ShellCmd_RemoveOneChar(ShellCmd_GetCursorPos());
}

//  �˸񰴼�  ����1��ʾ��Ч  ����0��ʾ��Ч
int ShellCmd_KeyBackspace(void)
{
    //  ��ȡ��ǰ���λ��
    int cursor_pos = ShellCmd_GetCursorPos();

    //  ����ǰλ�úϷ�
    if(cursor_pos > 0)
    {
        //  ɾ���������λ�õ��ַ�
        int re = ShellCmd_RemoveOneChar(cursor_pos - 1);

        //  ���������0
        if(ShellCmd_CursorPos > 0)
        {
            //  �ƶ����
            ShellCmd_CursorPos--;
        }

        //  ���ؽ��
        return re;
    }
    //  ��ǰ�Ƿ�
    else
    {
        return 0;
    }
}

//  �������ַ������зֶδ����ɹ�����1��ʧ�ܷ���С�ڵ���0
//  cmd_strΪ�����ַ������׵�ַ,lenΪ�ַ������ַ���������
int ShellCmd_SplitCmd(char* cmd_str, int len)
{
    //  ������
    if(cmd_str == 0) return 0;
    if(len <= 0) return 0;

    //  ���������ַ�
    int i=0;
    for(i=0;i<len;i++)
    {
        //  ��ȡ��ǰ�ַ�
        char tmp_char = cmd_str[i];

        //  �����ǰ�ַ�Ϊ�ָ���
        if( (tmp_char == 0x20) ||   //  �ո�
            (tmp_char == 0x0D) ||   //  �س�
            (tmp_char == 0x0A) ||   //  ����
            (tmp_char == 0x09)      //  TAB
          )
        {
            //  ���зָ�
            cmd_str[i] = 0x00;
        }
    }

    //  ���سɹ�
    return 1;
}

//  ����ָ������ɹ�����1��ʧ�ܷ���С�ڵ���0
//  cmd_strΪ�����ַ������׵�ַ,lenΪ�ַ������ַ���������
int ShellCmd_InvSplitCmd(char* cmd_str, int len)
{
    //  ������
    if(cmd_str == 0) return 0;
    if(len <= 0) return 0;

    //  ���������ַ�
    int i=0;
    for(i=0;i<len;i++)
    {
        //  ��ȡ��ǰ�ַ�
        char tmp_char = cmd_str[i];

        //  ��Ϊ�ռ��
        if(tmp_char == 0x00)
        {
            //  ����Ϊ�ո�
            cmd_str[i] = 0x20;
        }
    }

    //  ����ɹ�
    return 1;
}

//  ��ȡ�ָ����������,�ɹ����ض����������ʧ�ܷ���С�ڵ���0
//  cmd_strΪ�����ַ������׵�ַ,lenΪ�ַ������ַ���������
int ShellCmd_GetSplitTotalNum(char* cmd_str, int len)
{
    //  ������
    if(cmd_str == 0) return 0;
    if(len <= 0) return 0;

    //  �������
    int have_end_zero = 1;    //  ��Ϊ0�����ı��,Ĭ��Ϊ�ӽ�β��ʼ
    int seg_count = 0;        //  ���������

    //  ���������ַ�
    int i=0;
    for(i=0;i<len;i++)
    {
        //  ��ȡ��ǰ�ַ�
        char tmp_char = cmd_str[i];

        //  ����ǰ�ַ�Ϊ0x00
        if(tmp_char == 0x00)
        {
            //  ���ü�⵽0
            have_end_zero = 1;
        }
        //  ��Ϊ��Ч�ַ�
        else
        {
            //  ����һ���ַ�Ϊ��β��ʱ��
            if(have_end_zero != 0)
            {
                seg_count++;  //  ͳ�ƶ���ĸ���
            }
            //  Ϊ�ǽ�β
            else
            {
                //  ���ֲ���
            }

            //  ���¼���־����⵽��0
            have_end_zero = 0;
        }
    }

    //  ���ض�����Ŀ
    return seg_count;
}

//  ����ָ���Ķ����������ȡ�ö������ڵ�λ����ţ��ɹ����ش��ڵ���0��ʧ�ܷ���С��0
//  cmd_strΪ�����ַ������׵�ַ,lenΪ�ַ������ַ���������,seg_indexΪָ���Ķ�����ţ�0�����
int ShellCmd_GetSegPos(char* cmd_str, int len, int seg_index)
{
    //  ������
    if(cmd_str == 0) return -1;
    if(len <= 0) return -1;
    if(seg_index < 0) return -1;

    //  �������
    int have_end_zero = 1;    //  ��Ϊ0�����ı��,Ĭ��Ϊ�ӽ�β��ʼ
    int seg_count = 0;        //  ���������

    //  ���������ַ�
    int i=0;
    for(i=0;i<len;i++)
    {
        //  ��ȡ��ǰ�ַ�
        char tmp_char = cmd_str[i];

        //  ����ǰ�ַ�Ϊ0x00
        if(tmp_char == 0x00)
        {
            //  ���ü�⵽0
            have_end_zero = 1;
        }
        //  ��Ϊ��Ч�ַ�
        else
        {
            //  ����һ���ַ�Ϊ��β��ʱ��
            if(have_end_zero != 0)
            {
                //  �ҵ���һ�����䣬����ƥ��ָ���Ķ���ֵ
                if(seg_count == seg_index)
                {
                    //  ƥ��ɹ�
                    return i;
                }

                //  û���ҵ�
                seg_count++;  //  ͳ�ƶ���ĸ���
            }
            //  Ϊ�ǽ�β
            else
            {
                //  ���ֲ���
            }

            //  ���¼���־����⵽��0
            have_end_zero = 0;
        }
    }

    //  û���ҵ�
    return -1;
}


//  �س�����  �ɹ����ػ�ȡ�����û�����ĳ��ȣ� ʧ�ܻ���û�������0
int ShellCmd_KeyEnter(void)
{
    //  ��ȡ��ǰ���ڱ༭����������
    char tmp_cmd[SHELL_CMD_MAX_CHAR];
    memset(tmp_cmd, 0, SHELL_CMD_MAX_CHAR);
    int cmd_len = ShellCmd_CurCmdLineToString(tmp_cmd, SHELL_CMD_MAX_CHAR);

    //  ��ʾ�յ�������
    //USER_UI_printf("Recv CMD:%s\r\n", tmp_cmd);

    //  �ֶδ���
    ShellCmd_SplitCmd(tmp_cmd, cmd_len);

    //  ��ȡ�������
    int seg_num = ShellCmd_GetSplitTotalNum(tmp_cmd, cmd_len);

    //  ��ʾ�������
    //USER_UI_printf("Split Seg Num:%d\r\n", seg_num);

    //  ѭ����ӡÿ�������е��ַ�����
    /*
    int i=0;
    for(i=0;i<seg_num;i++)
    {
        //  ���Ի�ȡ��ǰ��������ݵ�����λ��
        int seg_pos = ShellCmd_GetSegPos(tmp_cmd, cmd_len, i);

        //  ����Ƿ��ȡ�ɹ�
        if(seg_pos < 0)
        {
            //  ��ʾ��ǰ�����ȡʧ��
            USER_UI_printf("Error!! Get Seg Pos Error!!  pos value = %d\r\n", i);

            //  ������һ������
            continue;
        }

        //  ��ȡ�ɹ�����ʾ��������
        USER_UI_printf("Seg[%d]:%s\r\n", i, tmp_cmd + seg_pos);
    }
    */

    //  ���������
    if(seg_num <= 0)
    {
        //  ��յ�ǰ����
        ShellCmd_EndOfCmdLine();

        //  ����
        return 0;
    }

    //  ��ȡ�û����������
    int total_cmd_num = UserCmd_GetCmdTotalNum();

    //  ���������û�����
    int cmd_index = 0;
    for(cmd_index=0;cmd_index<total_cmd_num;cmd_index++)
    {
        //  ��ȡ��ǰλ�õ�������Ϣָ��
        SUserCmd* ptmp_cmd = UserCmd_GetCmdInfo(cmd_index);

        //  ���ָ��
        if(ptmp_cmd == 0)
        {
            //  ��յ�ǰ����
            ShellCmd_EndOfCmdLine();

            //  ���ش���
            return 0;    //  ִ�г��������ϲ�����
        }

        //  ��ȡ�����ַ������ڵ�λ��
        int cmd_pos = ShellCmd_GetSegPos(tmp_cmd, cmd_len, 0);

        //  ��ȡʧ��
        if(cmd_pos < 0)
        {
            //  ��յ�ǰ����
            ShellCmd_EndOfCmdLine();

            //  ���ش���
            return 0;    //  ִ�г��������ϲ�����
        }

        //  ����������ʷ��
#if CMD_HISTORY_EN
        //  ����Ϊ�ڽ���ʷ�����ʱ��
        if(strcmp(tmp_cmd + cmd_pos, CMD_HISTORY_STR) != 0)
        {
            //  ִ������ָ�
            ShellCmd_InvSplitCmd(tmp_cmd, cmd_len);

            //  ���浽��ʷ
            CmdHistory_InsertNewCmd(tmp_cmd);

            //  �ָ�����
            ShellCmd_SplitCmd(tmp_cmd, cmd_len);
        }

#endif  //  CMD_HISTORY_EN

        //  �Ա������ַ���
        if(strcmp(tmp_cmd + cmd_pos, ptmp_cmd->pCmdStr) == 0)
        {
            //  ���ִ������ĺ���ָ��
            if(ptmp_cmd->pUserCmdFun_do == 0)
            {
                //  ��յ�ǰ����
                ShellCmd_EndOfCmdLine();

                //  ���ش���
                return 0;    //  ִ�г��������ϲ�����
            }

            //  ���������е�״̬���û�����ʼִ�У�shell�������ͷż���
            ShellCmd_SetCmdLineStatus(SHELL_CMD_LINE_STATUS_BUSY);

            //  ִ������
            ptmp_cmd->pUserCmdFun_do(tmp_cmd, cmd_len);

            //  ���������е�״̬���û������Ѿ�æ�꣬��ǰ���ڿ���
            ShellCmd_SetCmdLineStatus(SHELL_CMD_LINE_STATUS_READY);

            //  ��յ�ǰ����
            ShellCmd_EndOfCmdLine();

            //  ���سɹ�
            return cmd_len;
        }
    }

    //  ����û���ҵ��û�����Ļص�����
    ShellCmd_NotFoundUserCmd_CallBack(tmp_cmd, cmd_len);

    //  ��յ�ǰ����
    ShellCmd_EndOfCmdLine();

    //  ����ʧ�ܣ���ʱ��������Ѿ���������Ҳû�ҵ��û������
    return 0;
}

