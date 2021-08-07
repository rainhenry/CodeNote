/************************************************************************************
 *
 *   �������ƣ��û��������
 *   ��Ʊ�д��rainhenry
 *   ����汾��REV 0.1
 *   ����޸ģ�20191112
 *
 *   ���˵��
 *     Ϊ��ʵ�ֽ��������к���Ȼ���Խ���VT100Э�齻���������ܹ�����MCU��ʹ����Դ�����ù��ܷ���Ļص�����
 *   ���б�д��������������¼�ռ��һ���ص�����ʱ�����ò�ͬ�����������֡�Ȼ�󴫵ݲ������ǲ���voidָ�����
 *   ʽ�����������˴���Ŀ���ֲ�ԺͿ���չ�ԡ�
 *     �����ֽ׶ν������ڼ򵥵�MCU����ʹ�õĻ�����δʵ�ֺ��û�����Ľ����������룬��Ϊʵ��������Ҫ���ö�
 *   ���񲢷�ִ�е�Ҫ�󣬼���Ҫ����ϵͳ��֧�֡�������ʱ�����ù��ܡ�
 *
 ***********************************************************************************/
//-----------------------------------------------------------------------------------
//  �ض��屣��
#ifndef __USER_CMD_H__
#define __USER_CMD_H__

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

//-----------------------------------------------------------------------------------
//  ���������������

//  ������ƻص����ͷ���
typedef enum
{
    FunCtrlType_BufFull = 0,           //  VT100�Ļ������Ѿ�����
    FunCtrlType_CtrlChar,              //  �յ���������ַ�
    FunCtrlType_ASCII_Char,            //  �յ�ASCII�ַ�
    FunCtrlType_OhterChar,             //  �յ������ַ�
    FunCtrlType_Up,                    //  �ϼ�
    FunCtrlType_Down,                  //  �¼�
    FunCtrlType_Left,                  //  ���
    FunCtrlType_Right,                 //  �Ҽ�
    FunCtrlType_Delete,                //  ɾ����
    FunCtrlType_PageUp,                //  �Ϸ�ҳ
    FunCtrlType_PageDown,              //  �·�ҳ
    FunCtrlType_Home,                  //  Home��
    FunCtrlType_End,                   //  End��
    FunCtrlType_Insert,                //  Insert��
    FunCtrlType_F1,                    //  F1��
    FunCtrlType_F2,                    //  F2��
    FunCtrlType_F3,                    //  F3��
    FunCtrlType_F4,                    //  F4��
    FunCtrlType_F5,                    //  F5��
    FunCtrlType_F6,                    //  F6��
    FunCtrlType_F7,                    //  F7��
    FunCtrlType_F8,                    //  F8��
    FunCtrlType_F9,                    //  F9��
    FunCtrlType_F10,                   //  F10��
    FunCtrlType_F11,                   //  F11��
    FunCtrlType_F12,                   //  F12��
    FunCtrlType_ReportCursorPos,       //  �õ���ǰ���λ��
    FunCtrlType_HintTab,               //  ƥ�䵽N��TAB����ʾ����
}EFunCtrlType;

//  ��������ĺ���ָ��
//  �ɹ�����0��ʧ�ܷ��ط�0  Ԥ��
//  cmd_strΪ�����ַ������׵�ַ,lenΪ�ַ������ַ���������
typedef int (*TpUserCmdFun_do)(char* cmd_str, int len);

//  ��������Ŀ��ƻص�����ָ��
//  �ɹ�����0��ʧ�ܷ��ط�0  Ԥ��
//  typeΪ��ǰ�Ŀ�������,pUserDataΪԤ�����û�����ָ�루�������в���ʱ��ʹ�ã�
typedef int (*TpUserCmdFun_ctrl)(EFunCtrlType type, void* pUserData);

//  ���ڶ���һ���û���������ݽṹ
typedef struct
{
    TpUserCmdFun_do pUserCmdFun_do;      //  �û�ִ�к���ָ��
    TpUserCmdFun_ctrl pUserCmdFun_ctrl;  //  �û����ƺ���ָ�룬��ִ���û������ʱ�򣬸ú����������ؽ�����Ϣ��ʵ���û�����Ľ�����������
    const char* pCmdStr;                 //  �����ַ���
    const char* pHelpStr;                //  �����ַ���
}SUserCmd;


//-----------------------------------------------------------------------------------
//  ��������

//  ��ȡ��ǰ���������
int UserCmd_GetCmdTotalNum(void);

//  ��ȡָ���������Ľṹ��ָ��
//  �ɹ�����ָ�룬ʧ�ܷ���0
//  indexΪָ�������
const SUserCmd* UserCmd_GetCmdInfo(int index);

//-----------------------------------------------------------------------------------
//  C++���ݴ���
#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------
#endif  //  __USER_CMD_H__
