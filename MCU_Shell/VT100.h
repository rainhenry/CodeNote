/************************************************************************************
 *
 *   �������ƣ����ڵ�Ƭ����VT100Э��
 *   ��Ʊ�д��rainhenry
 *   ����汾��REV 0.1
 *   ����޸ģ�20191021
 *
 ***********************************************************************************/
//-----------------------------------------------------------------------------------
//  �ض��屣��
#ifndef __VT100_H__
#define __VT100_H__

//-----------------------------------------------------------------------------------
//  ����ͷ�ļ�

//-----------------------------------------------------------------------------------
//  C++���ݴ���
#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------------------------------------------------------------
//  ��غ궨��
//  ���ÿ����ַ�(�����ַ���Χ0x01~0x1F,0x7F)
#define VT100_KEY_BACKSPACE           0x7F                //  �˸�
#define VT100_KEY_TAB                 0x09                //  TAB��
#define VT100_CTRL_ENTER_R            0x0D                //  �س� '\r'  �ص���ǰ�е�����  ͬʱputty���س�ʱ���͵�Ҳ����Ϊ���ַ�
#define VT100_CTRL_ENTER_N            0x0A                //  ���� '\n'  ������һ�У���λ�ò���
#define VT100_CTRL_C                  0x03                //  ǿ����ֹ

//-----------------------------------------------------------------------------------
//  ���������������
typedef enum
{
    //  ��ʾ��ʽ������
    VT100_DISP_RESETALL = 0,         //  ��λ��������
    VT100_DISP_BRIGHT = 1,           //  ��ʾ����
    VT100_DISP_DIM = 2,              //  ��ʾ�䰵
    VT100_DISP_UNDERSCORE = 4,       //  �»���
    VT100_DISP_BLINK = 5,            //  ��˸
    VT100_DISP_REVERSE = 7,          //  ����
    VT100_DISP_HIDDEN = 8,           //  ����

    //  �ַ���ɫ
    VT100_TEXTCOLOR_BLACK = 30,      //  ��ɫ
    VT100_TEXTCOLOR_RED = 31,        //  ��ɫ
    VT100_TEXTCOLOR_GREEN = 32,      //  ��ɫ
    VT100_TEXTCOLOR_YELLOW = 33,     //  ��ɫ
    VT100_TEXTCOLOR_BLUE = 34,       //  ��ɫ
    VT100_TEXTCOLOR_MAGENTA = 35,    //  ��ɫ
    VT100_TEXTCOLOR_CYAN = 36,       //  ��ɫ/����ɫ
    VT100_TEXTCOLOR_WHITE = 37,      //  ��ɫ

    //  ������ɫ
    VT100_BACKCOLOR_BLACK = 40,      //  ��ɫ
    VT100_BACKCOLOR_RED = 41,        //  ��ɫ
    VT100_BACKCOLOR_GREEN = 42,      //  ��ɫ
    VT100_BACKCOLOR_YELLOW = 43,     //  ��ɫ
    VT100_BACKCOLOR_BLUE = 44,       //  ��ɫ
    VT100_BACKCOLOR_MAGENTA = 45,    //  ��ɫ
    VT100_BACKCOLOR_CYAN = 46,       //  ��ɫ/����ɫ
    VT100_BACKCOLOR_WHITE = 47,      //  ��ɫ
}EVT100DispAttrib;

//-----------------------------------------------------------------------------------
//  ��������  ���Ͳ���
//  ����һ���ַ�
void VT100_Send_OneChar(char in);

//  ����һ���ַ���
void VT100_Send_String(char* str);

//  VT100��printf֧��
void VT100_printf(char* str, ...);

//  �������ն��豸��������
void VT100_SEND_TO_PHY_DEVICE(unsigned char* pdat, int len);

//  �����Ļ
void VT100_Send_ClearScreen(void);

//  ���һ��
void VT100_Send_ClearLine(void);

//  ���ع��
void VT100_Send_HideCursor(void);

//  ��ʾ���
void VT100_Send_ShowCursor(void);

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorUp(int char_num);

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorDown(int char_num);

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorLeft(int char_num);

//  �������   char_num�ƶ����ַ���Ŀ  1����
void VT100_Send_CursorRight(int char_num);

//  ������ƶ���ָ��λ��  1����
void VT100_Send_CursorMoveTo(int x, int y);

//  ��ȡ��ǰ����λ��
void VT100_Send_GetCursorPos(void);

//  ��������
void VT100_Send_EnableWrap(void);

//  �رջ���
void VT100_Send_DisableWrap(void);

//  ������ʾ����
void VT100_Send_DisplayAttributes(EVT100DispAttrib attrib);

//  �����˸�
void VT100_Send_Backspace(void);

//  ���ͱ�����λ��
void VT100_Send_SaveCursorPos(void);

//  ���ͻָ����λ��
void VT100_Send_UnsaveCursorPos(void);

//-----------------------------------------------------------------------------------
//  ��������  ���ղ���
//  ��ȡ��������ǰ�Ѿ����ڵ��ֽڸ���
int VT100_GetRecvBufCount(void);

//  ���ȫ��������
void VT100_ClearRecvBuf(void);

//  ����һ���ַ�
void VT100_InsertRecvBuf(unsigned char in);

//  ����Ľ��մ���
void VT100_RecvHandle(void);

//-----------------------------------------------------------------------------------
//  C++���ݴ���
#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------
#endif  //  __VT100_H__
