
//  程序版本 REV 0.1

//-----------------------------------------------------------------------------------
//  包含头文件
#include "../MCU_Shell/MCU_Shell.h"
#include <string.h>


//-----------------------------------------------------------------------------------
//  用户定义的回调函数
//  当没有找到命令的时候的回调提示
static void ShellCmd_NotFoundUserCmd_CallBack(char* cmd_str, int len)
{
    VT100_Send_String("\r\n");
    VT100_Send_String("  Not Found Command ---- '");
    VT100_Send_String(cmd_str);
    VT100_Send_String("'");
}

//-----------------------------------------------------------------------------------
//  定义动态字符缓存
static SCharUnit ShellCmd_CharBuf[SHELL_CMD_MAX_CHAR];

//  定义当前命令的光标所在位置
static int ShellCmd_CursorPos = 0;

//  定义当前命令行状态  0=等待用户输入命令  1=执行命令
static int ShellCmd_CmdLineStatus = SHELL_CMD_LINE_STATUS_READY;

//  设置命令行状态
//  in为指定的状态，取值为SHELL_CMD_LINE_STATUS_READY或者SHELL_CMD_LINE_STATUS_BUSY
static void ShellCmd_SetCmdLineStatus(int in)
{
    //  设置
    ShellCmd_CmdLineStatus = in;
}

//  获取命令行状态
int ShellCmd_GetCmdLineStatus(void)
{
    return ShellCmd_CmdLineStatus;
}


//  初始化相关
void ShellCmd_Init(void)
{
    //  初始化缓存
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        ShellCmd_CharBuf[i].dat = -1;
        ShellCmd_CharBuf[i].next = -1;
    }

    //  初始化光标位置
    ShellCmd_CursorPos = 0;

    //  设置命令行的初始状态
    ShellCmd_SetCmdLineStatus(SHELL_CMD_LINE_STATUS_READY);

    //  复位历史命令
#if CMD_HISTORY_EN
    CmdHistory_Init();
#endif
}

//  当硬件初始化完成后，开始shell
void ShellCmd_ShellBegin(void)
{
    //  清除屏幕
    VT100_Send_ClearScreen();

    //  光标归位
    VT100_Send_CursorMoveTo(1,1);

    //  刷新显示
    ShellCmd_RefreshLineText(SHELL_CMD_HEAD_STR);
}

//  结束一行命令的处理
void ShellCmd_EndOfCmdLine(void)
{
    //  初始化缓存
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        ShellCmd_CharBuf[i].dat = -1;
        ShellCmd_CharBuf[i].next = -1;
    }

    //  初始化光标位置
    ShellCmd_CursorPos = 0;
}

//  找到缓存中存在最近的一个空的 成功返回大于等于0  失败返回-1
static int ShellCmd_FindNullPos(void)
{
    //  遍历缓存
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  当内容为不可用的时候
        if(ShellCmd_CharBuf[i].dat == -1)
        {
            return i;
        }
    }

    //  返回失败
    return -1;
}

//  找到结尾的缓存位置， 成功返回大于等于0，失败返回-1
static int ShellCmd_FindEndPos(void)
{
    //  检查第一个位置是否为空,当为空的时候
    if(ShellCmd_CharBuf[0].dat == -1)
    {
        //  缓存为空
        return -1;
    }

    //  有限次数查找
    int cur_pos = 0;        //  当前位置
    int next_pos = 0;       //  下一个位置
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  获取下次的位置
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  当下一个位置为不可用的时候
        if(next_pos == -1)
        {
            //  当前位置即为结尾
            return cur_pos;
        }

        //  更新位置
        cur_pos = next_pos;
    }

    //  未找到匹配的结尾
    return -1;
}

//  获取缓存已经使用数量
int ShellCmd_GetAlreadyUseCount(void)
{
    //  定义已经使用的数目
    int use_cnt = 0;

    //  遍历缓存
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  当内容为可用的时候
        if(ShellCmd_CharBuf[i].dat != -1)
        {
            //  统计可用的个数
            use_cnt++;
        }
    }

    //  返回数目
    return use_cnt;
}

//  检查当前光标位置的情况  失败返回-1  空缓存的开始返回0  缓存的最末尾返回1   缓存中间返回2
static int ShellCmd_CheckCursorPos(void)
{
    //  定义相关变量
    int cur_pos = 0;       //  当前位置
    int next_pos = 0;      //  下一个位置

    //  当光标指向0位置
    if(ShellCmd_CursorPos <= 0) return 0;

    //  遍历缓存，光标在哪里，就遍历光标之前存在的数据
    int i=0;
    for(i=0;i<(ShellCmd_CursorPos-1);i++)
    {
        //  当内容为可用的时候
        if(ShellCmd_CharBuf[cur_pos].dat != -1)
        {
            //  获取下一个位置
            next_pos = ShellCmd_CharBuf[cur_pos].next;

            //  检查下一个位置，下一个位置可用
            if(next_pos != -1)
            {
                //  更新位置
                cur_pos = next_pos;
            }
            //  下一个位置不可用
            else
            {
                //  光标指向了一个超出范围的位置
                return -1;
            }
        }
        //  当当前内容不可用
        else
        {
            //  数据出错
            return -1;
        }
    }

    //  检查结果合法性
    if(cur_pos != next_pos) return -1;    //  出现了不可能出现的未知情况

    //  获取下一个位置
    next_pos = ShellCmd_CharBuf[cur_pos].next;

    //  当达到光标位置后，如果下一个位置为不可用
    if(next_pos == -1)
    {
        //  到达最末尾
        return 1;
    }
    //  当下一个光标位置可用
    else
    {
        //  达到中间位置
        return 2;
    }
}

//  在当前光标位置用户输入一个字符  成功返回1， 失败返回0
int ShellCmd_InsertChar(unsigned char in)
{
    //  检查当前光标位置
    if(ShellCmd_CursorPos >= SHELL_CMD_MAX_CHAR) return 0;

    //  检查缓存是否满了
    int total_len = ShellCmd_GetAlreadyUseCount();
    if(total_len >= SHELL_CMD_MAX_CHAR) return 0;

    //  检查当前光标所在位置
    int cursor_pos_type = ShellCmd_CheckCursorPos();

    //  当在空缓存开始的位置
    if(cursor_pos_type == 0)
    {
        /*
        //  使用第一个位置
        ShellCmd_CharBuf[0].dat = in & 0x0FF;
        ShellCmd_CharBuf[0].next = -1;

        //  设置光标位置
        ShellCmd_CursorPos = 1;

        //  操作成功
        return 1;
        */

        //  检查第一个位置是否已经使用，当已经使用
        if(ShellCmd_CharBuf[0].dat != -1)
        {
            //  找到最近的一个空位置
            int null_char_pos = ShellCmd_FindNullPos();

            //  当获取空位置失败
            if(null_char_pos == -1) return 0;

            //  将本位置的内容复制到新位置中
            ShellCmd_CharBuf[null_char_pos].dat = ShellCmd_CharBuf[0].dat;
            ShellCmd_CharBuf[null_char_pos].next = ShellCmd_CharBuf[0].next;

            //  使用第一个位置
            ShellCmd_CharBuf[0].dat = in & 0x0FF;
            ShellCmd_CharBuf[0].next = null_char_pos;

            //  设置光标位置
            ShellCmd_CursorPos = 1;

            //  操作成功
            return 1;

        }
        //  当没有被使用
        else
        {
            //  使用第一个位置
            ShellCmd_CharBuf[0].dat = in & 0x0FF;
            ShellCmd_CharBuf[0].next = -1;

            //  设置光标位置
            ShellCmd_CursorPos = 1;

            //  操作成功
            return 1;
        }



    }
    //  当在缓存最末尾
    else if(cursor_pos_type == 1)
    {
        //  找到最后一个字符的位置
        int last_char_pos = ShellCmd_FindEndPos();

        //  当获取位置失败
        if(last_char_pos == -1)
        {
            //  使用第一个位置
            ShellCmd_CharBuf[0].dat = in & 0x0FF;
            ShellCmd_CharBuf[0].next = -1;

            //  设置光标位置
            ShellCmd_CursorPos = 1;

            //  操作成功
            return 1;
        }

        //  找到最近的一个空位置
        int null_char_pos = ShellCmd_FindNullPos();

        //  当获取空位置失败
        if(null_char_pos == -1) return 0;

        //  链接结尾达到新位置
        ShellCmd_CharBuf[last_char_pos].next = null_char_pos;

        //  填充数据
        ShellCmd_CharBuf[null_char_pos].dat = in & 0x0FF;
        ShellCmd_CharBuf[null_char_pos].next = -1;

        //  设置光标位置
        ShellCmd_CursorPos++;

        //  插入成功
        return 1;
    }
    //  当在缓存中间
    else if(cursor_pos_type == 2)
    {
        //  检查当前光标合法性
        if(ShellCmd_CursorPos <= 0) return 0;
        if(ShellCmd_CursorPos >= total_len) return 0;

        //  找到最近的一个空位置
        int null_char_pos = ShellCmd_FindNullPos();

        //  当获取空位置失败
        if(null_char_pos == -1) return 0;

        //  找到当前光标指向的前一个字符的位置
        int cur_pos = 0;       //  当前位置
        int next_pos = 0;      //  下一个位置
        int i=0;
        for(i=0;i<(ShellCmd_CursorPos-1);i++)
        {
            //  当内容为可用的时候
            if(ShellCmd_CharBuf[cur_pos].dat != -1)
            {
                //  获取下一个位置
                next_pos = ShellCmd_CharBuf[cur_pos].next;

                //  检查下一个位置，下一个位置可用
                if(next_pos != -1)
                {
                    //  更新位置
                    cur_pos = next_pos;
                }
                //  下一个位置不可用
                else
                {
                    //  光标指向了一个超出范围的位置
                    return 0;
                }
            }
            //  当当前内容不可用
            else
            {
                //  数据出错
                return 0;
            }
        }
        //  检查结果合法性
        if(cur_pos != next_pos) return 0;    //  出现了不可能出现的未知情况

        //  记录该位置原来指向的下一个位置
        int old_next = ShellCmd_CharBuf[cur_pos].next;

        //  插入新位置
        ShellCmd_CharBuf[cur_pos].next = null_char_pos;
        ShellCmd_CharBuf[null_char_pos].dat = in & 0x0FF;
        ShellCmd_CharBuf[null_char_pos].next = old_next;

        //  设置光标位置
        ShellCmd_CursorPos++;

        //  插入成功
        return 1;
    }
    //  当遇到错误
    else
    {
        return 0;
    }

}

//  插入字符串,成功返回插入的字符串长度,失败返回小于等于0
int ShellCmd_InsertString(char* str)
{
    //  检查输入
    if(str == 0) return 0;

    //  获取字符串的长度
    int str_len = strlen(str);

    //  检查长度
    if(str_len <= 0) return 0;

    //  遍历插入
    int i=0;
    for(i=0;i<str_len;i++)
    {
        ShellCmd_InsertChar(str[i]);
    }

    //  返回成功
    return str_len;
}

//  将动态字符转换成字符串  返回转换成功的字符个数   pstr为输出字符串的首地址， max_buf为缓冲区的最多大小
int ShellCmd_CurCmdLineToString(char* pstr, int max_buf)
{
    //  获取当前缓冲区存在的字符数量
    int total_len = ShellCmd_GetAlreadyUseCount();

    //  检查字符数量
    if(total_len > max_buf) return 0;

    //  检查缓冲区
    if(pstr == 0) return 0;

    //  有限次数查找
    int cur_pos = 0;        //  当前位置
    int next_pos = 0;       //  下一个位置
    int out_pos = 0;        //  输出字符串的位置
    int i=0;
    for(i=0;i<SHELL_CMD_MAX_CHAR;i++)
    {
        //  检查当前位置是否可用
        if(ShellCmd_CharBuf[cur_pos].dat == -1)
        {
            //  返回已经转换的数量
            return out_pos;
        }

        //  转换一个
        pstr[out_pos] = ShellCmd_CharBuf[cur_pos].dat;
        out_pos++;

        //  获取下次的位置
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  当下一个位置为不可用的时候
        if(next_pos == -1)
        {
            //  返回已经转换的数量
            return out_pos;
        }

        //  更新位置
        cur_pos = next_pos;
    }

    //  返回已经转换的数量
    return out_pos;
}

//  获取当前光标位置
int ShellCmd_GetCursorPos(void)
{
    return ShellCmd_CursorPos;
}

//  光标向左移动 移动成功返回1  失败返回0
int ShellCmd_MoveCursorLeft(void)
{
    //  检查当前光标位置是否合法
    if(ShellCmd_CursorPos <= 0) return 0;

    //  移动
    ShellCmd_CursorPos--;

    //  返回成功
    return 1;
}

//  光标向右移动  移动成功返回1  失败返回0
int ShellCmd_MoveCursorRight(void)
{
    //  获取已经使用的缓存大小
    int total_len = ShellCmd_GetAlreadyUseCount();

    //  检查当前移动是否合法
    if(ShellCmd_CursorPos >= total_len) return 0;

    //  移动
    ShellCmd_CursorPos++;

    //  返回成功
    return 1;
}

//  刷新当前行的显示  head_str为命令行提示的开头字符串
void ShellCmd_RefreshLineText(char* head_str)
{
    //  首先清空该行
    VT100_Send_ClearLine();

    //  然后让光标移动到行首
    unsigned char tmp_char = VT100_CTRL_ENTER_R;
    VT100_SEND_TO_PHY_DEVICE(&tmp_char, 1);

    //  打印字符前缀头部
    int head_str_len = strlen(head_str);
    VT100_SEND_TO_PHY_DEVICE(head_str, head_str_len);

    //  获取当前正在编辑的命令内容 并追加打印
    char tmp_cmd[SHELL_CMD_MAX_CHAR];
    memset(tmp_cmd, 0, SHELL_CMD_MAX_CHAR);
    int cmd_len = ShellCmd_CurCmdLineToString(tmp_cmd, SHELL_CMD_MAX_CHAR);
    VT100_SEND_TO_PHY_DEVICE(tmp_cmd, cmd_len);
}

//  从命令行字符串中移除一个字符   pos为要移除字符的位置,0基序   返回1表示移除成功， 返回0表示失败
int ShellCmd_RemoveOneChar(int pos)
{
    //  获取当前缓冲区存在的字符数量
    int total_len = ShellCmd_GetAlreadyUseCount();

    //  检查移除位置
    if(total_len <= 0) return 0;
    if(pos < 0) return 0;
    if(pos > (total_len - 1)) return 0;

    //  当为移除第一个
    if(pos == 0)
    {
        //  检查该元素本身是否合法
        if(ShellCmd_CharBuf[0].dat == -1) return 0;

        //  获取第一个指向的元素位置
        int next_index = ShellCmd_CharBuf[0].next;

        //  检查该元素是否合法，当指向的元素不合法，删除我自己
        if(next_index == -1)
        {
            //  删除本元素
            ShellCmd_CharBuf[0].dat = -1;
            ShellCmd_CharBuf[0].next = -1;
        }
        //  当指向的元素合法，拼接到首部
        else
        {
            //  获取下一个元素的数据相关
            int tmp_dat = ShellCmd_CharBuf[next_index].dat;
            int tmp_next = ShellCmd_CharBuf[next_index].next;

            //  复制到第一个元素里面
            ShellCmd_CharBuf[0].dat = tmp_dat;
            ShellCmd_CharBuf[0].next = tmp_next;

            //  释放空间
            ShellCmd_CharBuf[next_index].dat = -1;
            ShellCmd_CharBuf[next_index].next = -1;
        }

    }
    //  当为移除最后一个
    else if(pos == (total_len - 1))
    {
        //  找到当前光标指向的前一个字符的位置
        int cur_pos = 0;       //  当前位置
        int next_pos = 0;      //  下一个位置
        int i=0;
        for(i=0;i<(pos-1);i++)
        {
            //  当内容为可用的时候
            if(ShellCmd_CharBuf[cur_pos].dat != -1)
            {
                //  获取下一个位置
                next_pos = ShellCmd_CharBuf[cur_pos].next;

                //  检查下一个位置，下一个位置可用
                if(next_pos != -1)
                {
                    //  更新位置
                    cur_pos = next_pos;
                }
                //  下一个位置不可用
                else
                {
                    //  光标指向了一个超出范围的位置
                    return 0;
                }
            }
            //  当当前内容不可用
            else
            {
                //  数据出错
                return 0;
            }
        }
        //  检查结果合法性
        if(cur_pos != next_pos) return 0;    //  出现了不可能出现的未知情况

        //  获取当前指向的下一个位置
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  检查位置合法性，当合法
        if(next_pos != -1)
        {
            //  检查下一个位置是否为结尾
            //  当不是结尾
            if(ShellCmd_CharBuf[next_pos].next != -1) return 0;  //  出现未知错误

            //  释放下面的位置
            ShellCmd_CharBuf[next_pos].dat = -1;
            ShellCmd_CharBuf[next_pos].next = -1;

            //  添加结尾
            ShellCmd_CharBuf[cur_pos].next = -1;
        }
        //  当位置不合法
        else
        {
            //  删除失败
            return 0;
        }
    }
    //  当为移除中间位置的
    else
    {
        //  找到当前光标指向的前一个字符的位置
        int cur_pos = 0;       //  当前位置
        int next_pos = 0;      //  下一个位置
        int i=0;
        for(i=0;i<(pos-1);i++)
        {
            //  当内容为可用的时候
            if(ShellCmd_CharBuf[cur_pos].dat != -1)
            {
                //  获取下一个位置
                next_pos = ShellCmd_CharBuf[cur_pos].next;

                //  检查下一个位置，下一个位置可用
                if(next_pos != -1)
                {
                    //  更新位置
                    cur_pos = next_pos;
                }
                //  下一个位置不可用
                else
                {
                    //  光标指向了一个超出范围的位置
                    return 0;
                }
            }
            //  当当前内容不可用
            else
            {
                //  数据出错
                return 0;
            }
        }
        //  检查结果合法性
        if(cur_pos != next_pos) return 0;    //  出现了不可能出现的未知情况

        //  获取当前指向的下一个位置
        next_pos = ShellCmd_CharBuf[cur_pos].next;

        //  检查位置合法性，当合法
        if(next_pos != -1)
        {
            //  检查下一个位置是否为结尾
            //  当是结尾
            if(ShellCmd_CharBuf[next_pos].next == -1) return 0;  //  出现未知错误

            //  备份拼接位置
            int tmp_next = ShellCmd_CharBuf[next_pos].next;

            //  释放下面的位置
            ShellCmd_CharBuf[next_pos].dat = -1;
            ShellCmd_CharBuf[next_pos].next = -1;

            //  拼接
            ShellCmd_CharBuf[cur_pos].next = tmp_next;
        }
        //  当位置不合法
        else
        {
            //  删除失败
            return 0;
        }
    }

    //  返回成功
    return 1;
}

//  删除按键  返回1表示有效  返回0表示无效
int ShellCmd_KeyDelete(void)
{
    //  删除光标所在位置的字符
    return ShellCmd_RemoveOneChar(ShellCmd_GetCursorPos());
}

//  退格按键  返回1表示有效  返回0表示无效
int ShellCmd_KeyBackspace(void)
{
    //  获取当前光标位置
    int cursor_pos = ShellCmd_GetCursorPos();

    //  当当前位置合法
    if(cursor_pos > 0)
    {
        //  删除光标所在位置的字符
        int re = ShellCmd_RemoveOneChar(cursor_pos - 1);

        //  如果光标大于0
        if(ShellCmd_CursorPos > 0)
        {
            //  移动光标
            ShellCmd_CursorPos--;
        }

        //  返回结果
        return re;
    }
    //  当前非法
    else
    {
        return 0;
    }
}

//  将命令字符串进行分段处理，成功返回1，失败返回小于等于0
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度
int ShellCmd_SplitCmd(char* cmd_str, int len)
{
    //  检查参数
    if(cmd_str == 0) return 0;
    if(len <= 0) return 0;

    //  遍历所有字符
    int i=0;
    for(i=0;i<len;i++)
    {
        //  获取当前字符
        char tmp_char = cmd_str[i];

        //  如果当前字符为分隔类
        if( (tmp_char == 0x20) ||   //  空格
            (tmp_char == 0x0D) ||   //  回车
            (tmp_char == 0x0A) ||   //  换行
            (tmp_char == 0x09)      //  TAB
          )
        {
            //  进行分隔
            cmd_str[i] = 0x00;
        }
    }

    //  返回成功
    return 1;
}

//  逆向分隔处理，成功返回1，失败返回小于等于0
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度
int ShellCmd_InvSplitCmd(char* cmd_str, int len)
{
    //  检查参数
    if(cmd_str == 0) return 0;
    if(len <= 0) return 0;

    //  遍历所有字符
    int i=0;
    for(i=0;i<len;i++)
    {
        //  获取当前字符
        char tmp_char = cmd_str[i];

        //  当为空间隔
        if(tmp_char == 0x00)
        {
            //  设置为空格
            cmd_str[i] = 0x20;
        }
    }

    //  处理成功
    return 1;
}

//  获取分隔段落的总数,成功返回段落的总数，失败返回小于等于0
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度
int ShellCmd_GetSplitTotalNum(char* cmd_str, int len)
{
    //  检查参数
    if(cmd_str == 0) return 0;
    if(len <= 0) return 0;

    //  定义变量
    int have_end_zero = 1;    //  当为0结束的标记,默认为从结尾开始
    int seg_count = 0;        //  段落计数器

    //  遍历所有字符
    int i=0;
    for(i=0;i<len;i++)
    {
        //  获取当前字符
        char tmp_char = cmd_str[i];

        //  当当前字符为0x00
        if(tmp_char == 0x00)
        {
            //  设置监测到0
            have_end_zero = 1;
        }
        //  当为有效字符
        else
        {
            //  当上一个字符为结尾的时候
            if(have_end_zero != 0)
            {
                seg_count++;  //  统计段落的个数
            }
            //  为非结尾
            else
            {
                //  保持不变
            }

            //  更新监测标志，监测到非0
            have_end_zero = 0;
        }
    }

    //  返回段落数目
    return seg_count;
}

//  根据指定的段落个数，获取该段落所在的位置序号，成功返回大于等于0，失败返回小于0
//  cmd_str为命令字符串的首地址,len为字符串的字符总数长度,seg_index为指定的段落序号，0基序号
int ShellCmd_GetSegPos(char* cmd_str, int len, int seg_index)
{
    //  检查参数
    if(cmd_str == 0) return -1;
    if(len <= 0) return -1;
    if(seg_index < 0) return -1;

    //  定义变量
    int have_end_zero = 1;    //  当为0结束的标记,默认为从结尾开始
    int seg_count = 0;        //  段落计数器

    //  遍历所有字符
    int i=0;
    for(i=0;i<len;i++)
    {
        //  获取当前字符
        char tmp_char = cmd_str[i];

        //  当当前字符为0x00
        if(tmp_char == 0x00)
        {
            //  设置监测到0
            have_end_zero = 1;
        }
        //  当为有效字符
        else
        {
            //  当上一个字符为结尾的时候
            if(have_end_zero != 0)
            {
                //  找到了一个段落，尝试匹配指定的段落值
                if(seg_count == seg_index)
                {
                    //  匹配成功
                    return i;
                }

                //  没有找到
                seg_count++;  //  统计段落的个数
            }
            //  为非结尾
            else
            {
                //  保持不变
            }

            //  更新监测标志，监测到非0
            have_end_zero = 0;
        }
    }

    //  没有找到
    return -1;
}


//  回车按键  成功返回获取到的用户命令的长度， 失败或者没有命令返回0
int ShellCmd_KeyEnter(void)
{
    //  获取当前正在编辑的命令内容
    char tmp_cmd[SHELL_CMD_MAX_CHAR];
    memset(tmp_cmd, 0, SHELL_CMD_MAX_CHAR);
    int cmd_len = ShellCmd_CurCmdLineToString(tmp_cmd, SHELL_CMD_MAX_CHAR);

    //  提示收到的命令
    //USER_UI_printf("Recv CMD:%s\r\n", tmp_cmd);

    //  分段处理
    ShellCmd_SplitCmd(tmp_cmd, cmd_len);

    //  获取段落个数
    int seg_num = ShellCmd_GetSplitTotalNum(tmp_cmd, cmd_len);

    //  提示段落个数
    //USER_UI_printf("Split Seg Num:%d\r\n", seg_num);

    //  循环打印每个段落中的字符内容
    /*
    int i=0;
    for(i=0;i<seg_num;i++)
    {
        //  尝试获取当前段落的内容的所在位置
        int seg_pos = ShellCmd_GetSegPos(tmp_cmd, cmd_len, i);

        //  检查是否获取成功
        if(seg_pos < 0)
        {
            //  提示当前段落获取失败
            USER_UI_printf("Error!! Get Seg Pos Error!!  pos value = %d\r\n", i);

            //  继续下一个段落
            continue;
        }

        //  获取成功，提示段落内容
        USER_UI_printf("Seg[%d]:%s\r\n", i, tmp_cmd + seg_pos);
    }
    */

    //  检查段落个数
    if(seg_num <= 0)
    {
        //  清空当前缓存
        ShellCmd_EndOfCmdLine();

        //  返回
        return 0;
    }

    //  获取用户命令的总数
    int total_cmd_num = UserCmd_GetCmdTotalNum();

    //  遍历所有用户命令
    int cmd_index = 0;
    for(cmd_index=0;cmd_index<total_cmd_num;cmd_index++)
    {
        //  获取当前位置的命令信息指针
        SUserCmd* ptmp_cmd = UserCmd_GetCmdInfo(cmd_index);

        //  检查指针
        if(ptmp_cmd == 0)
        {
            //  清空当前缓存
            ShellCmd_EndOfCmdLine();

            //  返回错误
            return 0;    //  执行出错，理论上不可能
        }

        //  获取命令字符串所在的位置
        int cmd_pos = ShellCmd_GetSegPos(tmp_cmd, cmd_len, 0);

        //  获取失败
        if(cmd_pos < 0)
        {
            //  清空当前缓存
            ShellCmd_EndOfCmdLine();

            //  返回错误
            return 0;    //  执行出错，理论上不可能
        }

        //  保存该命令到历史中
#if CMD_HISTORY_EN
        //  当不为内建历史命令的时候
        if(strcmp(tmp_cmd + cmd_pos, CMD_HISTORY_STR) != 0)
        {
            //  执行逆向分隔
            ShellCmd_InvSplitCmd(tmp_cmd, cmd_len);

            //  保存到历史
            CmdHistory_InsertNewCmd(tmp_cmd);

            //  分隔处理
            ShellCmd_SplitCmd(tmp_cmd, cmd_len);
        }

#endif  //  CMD_HISTORY_EN

        //  对比命令字符串
        if(strcmp(tmp_cmd + cmd_pos, ptmp_cmd->pCmdStr) == 0)
        {
            //  检查执行命令的函数指针
            if(ptmp_cmd->pUserCmdFun_do == 0)
            {
                //  清空当前缓存
                ShellCmd_EndOfCmdLine();

                //  返回错误
                return 0;    //  执行出错，理论上不可能
            }

            //  设置命令行的状态，用户程序开始执行，shell解释器释放键盘
            ShellCmd_SetCmdLineStatus(SHELL_CMD_LINE_STATUS_BUSY);

            //  执行命令
            ptmp_cmd->pUserCmdFun_do(tmp_cmd, cmd_len);

            //  设置命令行的状态，用户程序已经忙完，当前处于空闲
            ShellCmd_SetCmdLineStatus(SHELL_CMD_LINE_STATUS_READY);

            //  清空当前缓存
            ShellCmd_EndOfCmdLine();

            //  返回成功
            return cmd_len;
        }
    }

    //  调用没有找到用户命令的回调处理
    ShellCmd_NotFoundUserCmd_CallBack(tmp_cmd, cmd_len);

    //  清空当前缓存
    ShellCmd_EndOfCmdLine();

    //  返回失败，此时所有命令都已经遍历光了也没找到用户输入的
    return 0;
}

