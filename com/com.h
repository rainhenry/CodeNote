
/*---------------------------------------------------------------------------
2016.07.15
	����ͨ��ͷ�ļ����������໯��������
	����ɨ�裬��ô����б��г�������Ϣ
	�򿪡��ر��豸
	��ȡ�豸״̬
	���ͺͽ��ղ���1���߳�
	������Ӧ�������ú���ָ�����ʽ����
	��̬�ڴ�������new �� delete

��������		������
���̴�������	2016��7��15��
�����������	2016��7��24��
��ƣ�		rainhenry
���룺		rainhenry
����/У�ԣ�	rainhenry
���Ի�����	VS2010
����ƽ̨��	Windows XP SP3 
����汾��	REV 2.1

�汾�޶���
	0.1�汾		����Windows API���ʴ���
	1.0�汾		�������װ�ṹ
	2.0�汾		���Ӷ�MFC�ؼ�֧��
				���ӱ���������������
				����STL vector������Ϊ���ݴ�����
	2.1�汾		�Ż��߳�ռ��CPU��ʹ����

˵����	��������Ա��κ���ʹ�ú�ת�أ���ת��/ʹ�ñ�����ʱ���뱣����ע�͡�
		����������ο���ѧϰ������������ҵ��;
----------------------------------------------------------------------------*/
#ifndef __com_h__
#define __com_h__

//	����sprintf�������
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

//	���ڲ��þ�̬�⣬������DWORD���͵ļ���
#ifndef DWORD
//#error Not define DWORD!
#ifndef _AFXDLL
#define _AFXDLL
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

//#include <targetver.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#endif


/*********************************************************************
ʹ��ע��

1������ͨ������SetComNumber�����ú������Կؼ�ָ����ߴ��ڲ�����������
   ��ָ���˿ؼ������Խ�����������
2��Ȼ�����Initiation�������г�ʼ���������������а�������Ч�Ŀռ�ָ�룬
   ������صĿؼ�һͬ��ʼ�������򽫲����ؼ�
   ��ʼ���ؼ���ʱ���Ĭ��ѡ���һ����Ȼ�������Ҫ����ѡȡ��������Ҫͨ��
   �ⲿ������е���
   ͬʱ�������շ�����
   ��ʱ�����ֶ����ò�����麯�����м�飬Ҳ����ֱ�������²������������
   �˲������Ĺ���
3���򿪴��ڣ���ʱ��������������̡߳�
4���շ�����
5���رմ��ڣ��̻߳��Զ�������
7����������Ҫ����๤����ʱ�򣬿��Ե���Release�����ͷ������ڴ棬���ҽ���
   �շ������߳�
8������Ҫ���´��ڲ�����ʱ����Ҫ�رմ��ڣ����õ����ͷź�����Ȼ��������
   ��֮��Ҳ���Ե��ø��º�����Ȼ���ٴδ򿪴��ڼ��ɡ�
   ���ڴ��ڴ򿪵�ʱ�����ò�����ʱ��Ҳ�������óɹ������ǵ�ǰ�Ѿ��򿪵Ĵ�
   �ڵ�����Ϊ��һ�ε����ã����رյ�ǰ���ڣ�����һ�δ򿪵�ʱ�򣬵�ǰ���ò�
   ����Ч��

---------------------------------------------------------------------
����ԭ�����
	����ͨ���ⲿ�������������Ĺ��к�������صĲ������������

	Ȼ���ʱ���Ե��ô�������������ʵ�ֶԿؼ��Ĳ���
	��δָ���ؼ�ָ���ʱ�򴮿�����������Ч

	Ȼ����ó�ʼ���������Ի���������Ҫ�������м��
	���ͨ�������ò������ñ�־����ʽ֪ͨ�����������Խ��й���

	�ڴ�ʱ���Ե��ô򿪺��������������̣߳������Ĵ�API�����߳���ִ�е�
	�߳��л�ͨ��һ�����п��ƴ������ж��Ƿ������սᡣ


---------------------------------------------------------------------
�����ص�
	����ͬ��IO�ķ�ʽ�����߳��ж�ռ��Դ
	���ͺͽ�����ͬһ���߳��й���
	����API������������
	���ñ�����ʽ��������
	���ԶԲ������м�飬֧����Դ����
	���պ������õ����ⲿ����ָ��ķ�ʽ
	���й��ܾ��������װ�ķ�ʽ������ɸ���

---------------------------------------------------------------------
Ӧ�þ���

��������������user.h�У�������������󣬺ͽ�����Ӧ����

	extern CSerialPort local_serial_port;

	//	�����������ݽ��յ���ʱ�򣬵����������ָ����ָ��ĺ���
	void LocalSerialRecvFunc(
		int nbytes,					//	���뻺�������ֽ���
		unsigned char* dat			//	���ݵ��׵�ַ
	);

Ȼ����user.cpp������ʵ��

	CSerialPort local_serial_port;


	//	�����������ݽ��յ���ʱ�򣬵����������ָ����ָ��ĺ���
	void LocalSerialRecvFunc(
		int nbytes,					//	���뻺�������ֽ���
		unsigned char* dat			//	���ݵ��׵�ַ
	)
	{
		//	�˴�Ϊ�û������ʵ��
		............
	}

Ȼ��ִ����س�ʼ����m_com_numberΪCComboBox�ؼ���ע�����иÿؼ���sort����ҪΪfalse

	local_serial_port.SetComNumber(&m_com_number);				//	���ô��ںſؼ�
	local_serial_port.SetBaudRate(&m_baud);						//	���ò����ʿؼ�
	local_serial_port.SetParity(&m_parity);						//	����У��λ
	local_serial_port.SetStopBits(&m_stop);						//	����ֹͣλ
	local_serial_port.SetDataBits(8);							//	8λ����λ���˴�Ϊ�̶�����

	int re;
	if ((re = local_serial_port.Initiation(&LocalSerialRecvFunc,1024,1024)) == INIT_OK)
	{
		//AfxMessageBox(L"��ʼ���ɹ�");
	} 
	else
	{
	CString str;
	str.Format(L"��ʼ��ʧ�ܣ��������:%d",re);
	AfxMessageBox(str);
	}

Ȼ���������������ʹ��

	local_serial_port.UpdateWidgetToConfig();				//	���¿ؼ����õ��ṹ����
	if (local_serial_port.CheckSetup() == SETUP_OK)
	{
		//	��ʱΪ���ڲ���������ȷ
		//MessageBox(L"����������ȷ��");
		//	�����û�����
		.........
	} 
	else
	{
		MessageBox(L"����Ĳ������ã�");
	}

�Դ��ڵĲ���
��		local_serial_port.Open();
����		local_serial_port.Send(length,pbuff);		//	lenghtΪҪ���͵��ֽ���Ŀ
													//	pbuffΪҪ�����������ڵ��׵�ַ

���մ�����
void LocalSerialRecvFunc(
int nbytes,					//	���뻺�������ֽ���
unsigned char* dat			//	���ݵ��׵�ַ
)
{
	//	�ڴ���ӽ����û�����
	//	nbytesΪ�����յ����ֽ���Ŀ
	//	datΪ�յ��������ڵ��׵�ַ
}
�ر�		local_serial_port.Close();		
*********************************************************************/

//-----------------------------------------------------------------------
//	WinCE6.0֧��
#include <CommCtrl.h>

#ifndef INVALID_LINK_INDEX
#define INVALID_LINK_INDEX  (-1)
#endif

//==================================================================================================
//	�궨��

//	��ʼ���������
#define INIT_OK							0							//	��ʼ��OK
#define INIT_MISSING_MAIN_CONFIG		1							//	ȱ����Ҫ����,�������ò���ȷ
#define INIT_RECVFUNC_ERROR				2							//	���մ�����ָ��Ƿ�
#define INIT_BUFFSIZE_ERROR				3							//	��������С���ô���
#define INIT_MALLOC_ERROR				4							//	�����ڴ����
#define INIT_THREADCREAT_FAIL			5							//	�̴߳���ʧ��

#define MIN_BUFFER_SIZE					128							//	��С��������СΪ128���ֽ�

//	�򿪴��ڵķ���ֵ
#define OPEN_OK							0							//	�򿪳ɹ�
#define OPEN_NO_INIT					1							//	��û�г�ʼ��
#define OPEN_YET						2							//	�����Ѿ���
#define OPEN_COM_NUMBER_ERROR			3							//	���ںŴ���
#define OPEN_FAIL						4							//	��ʧ��
#define OPEN_COM_DCB_ERROR				5							//	���ڲ������ô���

//	�������ݵķ���ֵ�������ش���0��ʱ��Ϊ�ѷ������ֽ���Ŀ����С�ڵ���0ʱ����������
#define SEND_OK							0							//	���ͳɹ�
#define SEND_COM_HANDLE_ERROR			1							//	���ھ������,���ߴ���û�д�
#define SEND_OVER_BUFFER_SIZE			2							//	ʵ��Ҫ���͵����ݳ����������Ĵ�С
#define SEND_FAIL						3							//	����ʧ�ܣ����Ե���GetLastError������ȡ��ϸ������Ϣ
#define SEND_BUSY						4							//	����æ

//	�رմ��ڵķ���ֵ
#define	CLOSE_OK						0							//	�رճɹ�
#define CLOSE_YET						1							//	�����Ѿ��ر�

//	��鴮�ںźϷ��Եķ���ֵ
#define CHECK_NCOM_STR_OK				0							//	���ͨ��
#define CHECK_NCOM_STR_FAIL				-1							//	���Ϸ�

//	�������ü�麯������ֵ
#define CHECK_DCB_OK					0							//	���ͨ��
#define CHECK_DCB_BAUD_ERROR			1							//	�Ƿ�������
#define	CHECK_DCB_DATABITS_ERROR		2							//	�Ƿ�����λ
#define CHECK_DCB_PARITY_ERROR			3							//	�Ƿ�У��λ
#define	CHECK_DCB_STOPBITS_ERROR		4							//	�Ƿ�ֹͣλ
#define CHECK_DCB_NULL_POINT			5							//	�յ�DCB�ṹָ��

//	��������ֵ�Ķ���
#define CONFIG_NULL						0							//	����û��׼����
#define CONFIG_OK						1							//	�����Ϸ�

//	�߳̿��ƴ��붨��
#define THREAD_STOP						0							//	�߳�ֹͣ
#define THREAD_RUN						1							//	�߳�����

//	������������
#define MIN_SERIALPORT_NUM				1							//	��С���ں�
#define	MAX_SERIALPORT_NUM				100							//	��󴮿ں�

//	������ú����ķ���ֵ,��������麯���ķ���ֵ
#define SETUP_OK						0							//	���óɹ�
#define SETUP_FAIL						1							//	����ʧ��

//	�̷߳��ش���
#define THREAD_EXIT						0							//	�߳������˳�
#define THREAD_COM_STR_ERROR			1							//	��鵽�����ַ�������
#define THREAD_COM_DCB_ERROR			2							//	��鵽�������ýṹ�����
#define THREAD_UNKNOW_ERROR				3							//	����δ֪����
#define THREAD_SEND_ERROR				4							//	���ʹ���
#define THREAD_RECV_ERROR				5							//	���մ���
#define THREAD_OPEN_FAIL				6							//	�򿪴���ʧ��
#define THREAD_CREATE_FAIL				7							//	�̵߳Ĵ����ʹ��ݲ�������

//	������æ�������ض���
//#define SEND_BUSY													//	æ���ض���
#define SEND_READY						0							//	����׼����

//==================================================================================================
//	�������ء�ѡ��
//#define SERIALPORT_DEBUG_LOG										//	������־����ѡ��

//==================================================================================================
//	ȫ���̺߳�������
//	�ú���Ϊ�̺߳��������ڴ����̵߳�ʱ������Ϊÿ���̷߳����ڴ�
//	����Ϊ�����CreateThread������ʹ�ã��������ﶨ��Ϊ���ⲿ�ĺ���
DWORD WINAPI CSerialPort_ThreadFunction(LPVOID lpParameter);

//==================================================================================================
//	�ඨ��
class CSerialPort
{
//	���е�
public:
	//	���캯��
	CSerialPort();

	//	��������
	~CSerialPort();

	//-----------------------------------------------------------------------------------------
	//	��ʼ��
	int Initiation(
		void (*pFunc)(int nbytes,unsigned char* dat),				//	���յ����ݵĴ�����ָ��
		int buff_size,												//	�������ߴ�
		int send_buff_size											//	���ͻ������ߴ�
		);

	//-----------------------------------------------------------------------------------------
	//	�ͷź���
	void Release(void);

	//-----------------------------------------------------------------------------------------
	//	ֱ�����ô��ں�
	int SetComNumber(
		CString ncom_str											//	��������Ϊ���ڵĴ��ַ���������"\\\\.\\COM2"
		);			
	//	ͨ���ؼ��������ô��ںţ�֧��������ѡ����
	int SetComNumber(
		CComboBox* pcom												//	ָ����CComboBox�ؼ�������������ѡ�񴮿ں�
		);

	//-----------------------------------------------------------------------------------------
	//	ֱ�����ò�����
	int SetBaudRate(
		int baud													//	ֱ��ָ�������ʣ���CBR_115200�����Ķ��壬������WINBASE.H�ļ���
		);
	//	ͨ���ؼ����ò�����
	int SetBaudRate(
		CComboBox* pbaud											//	ָ����CComboBox�ؼ����������ò�����
		);

	//-----------------------------------------------------------------------------------------
	//	����У��λ
	int SetParity(
		int parity													//	ֱ��ָ��У��λ����NOPARITY�����Ķ��壬������WINBASE.H�ļ���
		);
	//	ͨ���ؼ�����У��λ
	int SetParity(
		CComboBox* pparity											//	ָ����CComboBox�ؼ�����������У��λ
		);

	//-----------------------------------------------------------------------------------------
	//	����ֹͣλ
	int SetStopBits(
		int stopbits												//	ֱ��ָ��ֹͣλ����ONESTOPBIT�����Ķ��壬������WINBASE.H�ļ���
		);
	//	ͨ���ؼ�����ֹͣλ
	int SetStopBits(
		CComboBox* pstopbits										//	ָ����CComboBox�ؼ�����������ֹͣλ
		);

	//-----------------------------------------------------------------------------------------
	//	��������λ
	int SetDataBits(
		int databits												//	ֱ��ָ������λ��ȡֵ��Χһ����6~8
		);
	//	ͨ���ؼ���������λ
	int SetDataBits(
		CComboBox* pdatabits										//	ָ����CComboBox�ؼ���������������λ
		);
	
	//-----------------------------------------------------------------------------------------
	//	����DCB�ṹ���������
	int LoadConfig(
		DCB* pdcb													//	Ҫ�������õ�DCB�ṹ��ָ��
		);

	//----------------------------------------------------------------------------------------
	//	���¿ؼ�����Ĳ���������
	//	��������Ϊ SETUP_OK  ���� SETUP_FAIL
	int UpdateWidgetToConfig(void);

	//-----------------------------------------------------------------------------------------
	//	��������
	//	������ñ�������������Сֵ���δ򿪵����ֵ
	//	�ú����ķ���ֵΪ�������Ĵ�����Ŀ
	int SearchSerialPort(void);

	//-----------------------------------------------------------------------------------------
	//	�򿪴���
	int Open(void);

	//-----------------------------------------------------------------------------------------
	//	�رմ���
	void Close(void);

	//-----------------------------------------------------------------------------------------
	//	��������
	int Send(
		int length,													//	Ҫ���͵����ݵĳ���
		unsigned char* dat											//	�������ڵ��׵�ַ
		);
	
	//-----------------------------------------------------------------------------------------
	//	���������Ժ���,��鴮�ں����ã���DCB�ṹ
	int CheckSetup(void);

	//-----------------------------------------------------------------------------------------
	//	�����̺߳���
	DWORD ComThreadProc(LPVOID lpParameter);

	//-----------------------------------------------------------------------------------------
	CString log_str;					//	��־�ַ���
	
	//-----------------------------------------------------------------------------------------
	int SendBusy(void);					//	����æ�жϺ���

//	˽�е�
private:
	//	�������׵�ַָ��
	unsigned char* pSendData_Buffer;
	unsigned char* pRecvData_Buffer;

	//	�����������ݽ��յ���ʱ�򣬵����������ָ����ָ��ĺ���
	void (*pSerialRecv_Func)(
		int nbytes,					//	���뻺�������ֽ���
		unsigned char* dat			//	���ݵ��׵�ַ
		);
		
	//-----------------------------------------------------------------------------------------
	//	һЩ�ؼ���ı���
	//	Ĭ���������Щ������ΪNULL,ֻ�е�ָ�������ݲŻ����Ӧ�Ŀؼ����в���
	CComboBox* pComboBox_ComNumber;					//	���ں�
	CComboBox* pComboBox_BaudRate;					//	������
	CComboBox* pComboBox_Parity;					//	У��λ
	CComboBox* pComboBox_StopBits;					//	ֹͣλ
	CComboBox* pComboBox_DataBits;					//	����λ

	//-----------------------------------------------------------------------------------------
	//	ֱ�ӿ��ƴ������õĲ���
	HANDLE com_handle;								//	�򿪺�Ĵ��ھ��		���������ֱ�ӷ�Ӧ���ڵ�״̬����ΪNULLʱ������Ϊ�ر�״̬������Ϊ��
													//	�ò������߳�����Ϊ��д���ԣ��߳���Ϊֻ������ʼ����ʱ��Ϊ��д
	CString com_number_str;							//	���ں��ַ���
													//	�ò������߳�����Ϊ�����ԣ��߳���Ϊ��д����
	DCB	com_config;									//	���ڲ������ýṹ��
													//	�ò������߳�����Ϊ�����ԣ��߳���Ϊ��д����
	HANDLE hThreadHandle;							//	�߳̾��
	int ThreadControlCode;							//	�߳̿��ƴ���
													//	THREAD_STOPΪ�̹߳رգ�THREAD_RUNΪ�߳�����
	DWORD ThreadErrorCode;							//	����������
	DWORD RecvBytesNum;								//	ÿ�ν��յ����ֽ���
	DWORD SendBytesNum;								//	ÿ�η��͵��ֽ���


	//-----------------------------------------------------------------------------------------
	//	���������Ӻ���
	//	��鴮�ں��ַ����ĺϷ���
	int CheckComNumberString(CString str);

	//	��鴮������
	int CheckComDCB(DCB* pdcb);

//	���ܵ�
protected:

};

#endif		//	__com_h__

