
/*---------------------------------------------------------------------------
2016.07.15
	串口通信头文件，将串口类化，功能有
	串口扫描，获得串口列表、列出串口信息
	打开、关闭设备
	获取设备状态
	发送和接收采用1个线程
	接收响应函数采用函数指针的形式传递
	动态内存分配采用new 与 delete

工程名称		串口类
工程创建日期	2016年7月15日
工程完结日期	2016年7月24日
设计：		rainhenry
代码：		rainhenry
调试/校对：	rainhenry
测试环境：	VS2010
测试平台：	Windows XP SP3 
代码版本：	REV 2.1

版本修订：
	0.1版本		采用Windows API访问串口
	1.0版本		采用类封装结构
	2.0版本		增加对MFC控件支持
				增加暴力串口搜索功能
				采用STL vector容器作为数据处理缓存
	2.1版本		优化线程占用CPU的使用率

说明：	本代码可以被任何人使用和转载，但转载/使用本代码时，请保留本注释。
		本代码仅供参考和学习，切勿用于商业用途
----------------------------------------------------------------------------*/
#ifndef __com_h__
#define __com_h__

//	消除sprintf警告兼容
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

//	由于采用静态库，加入了DWORD类型的兼容
#ifndef DWORD
//#error Not define DWORD!
#ifndef _AFXDLL
#define _AFXDLL
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

//#include <targetver.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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
使用注释

1、首先通过调用SetComNumber等配置函数，对控件指针或者串口参数进行设置
   若指定了控件，可以进行搜索串口
2、然后调用Initiation函数进行初始化，若各个参数中包含了有效的空间指针，
   则会对相关的控件一同初始换，否则将不理会控件
   初始化控件的时候会默认选择第一个，然后如果需要控制选取其他，需要通过
   外部程序进行调用
   同时会申请收发缓存
   此时可以手动调用参数检查函数进行检查，也可以直接做如下操作，里面包含
   了参数检查的功能
3、打开串口，此时会检查参数并创建线程。
4、收发数据
5、关闭串口，线程会自动结束。
7、当不再需要这个类工作的时候，可以调用Release函数释放所有内存，并且结束
   收发控制线程
8、当需要更新串口参数的时候，需要关闭串口，不用调用释放函数，然后更改完参
   数之后，也可以调用更新函数，然后再次打开串口即可。
   当在串口打开的时候设置参数的时候，也可以设置成功，但是当前已经打开的串
   口的设置为上一次的配置，当关闭当前串口，并再一次打开的时候，当前配置才
   会生效。

---------------------------------------------------------------------
工作原理介绍
	首先通过外部程序调用设置类的共有函数对相关的参数表进行设置

	然后此时可以调用串口搜索函数，实现对控件的操作
	当未指定控件指针的时候串口搜索功能无效

	然后调用初始化函数，对缓冲区和主要参数进行检查
	检查通过后，设置参数可用标志的形式通知其他函数可以进行工作

	在此时可以调用打开函数，用于启动线程，真正的打开API是在线程中执行的
	线程中会通过一个运行控制代码来判断是否自我终结。


---------------------------------------------------------------------
技术特点
	采用同步IO的方式，在线程中独占资源
	发送和接收在同一个线程中工作
	采用API函数操作串口
	采用暴力方式搜索串口
	可以对参数进行检查，支持资源管理
	接收函数采用调用外部函数指针的方式
	所有功能均采用类封装的方式，代码可复用

---------------------------------------------------------------------
应用举例

首先声明对象在user.h中，包括声明类对象，和接收响应函数

	extern CSerialPort local_serial_port;

	//	当串口有数据接收到的时候，调用这个函数指针所指向的函数
	void LocalSerialRecvFunc(
		int nbytes,					//	进入缓冲区的字节数
		unsigned char* dat			//	数据的首地址
	);

然后在user.cpp中声明实例

	CSerialPort local_serial_port;


	//	当串口有数据接收到的时候，调用这个函数指针所指向的函数
	void LocalSerialRecvFunc(
		int nbytes,					//	进入缓冲区的字节数
		unsigned char* dat			//	数据的首地址
	)
	{
		//	此处为用户代码的实现
		............
	}

然后执行相关初始化，m_com_number为CComboBox控件，注意所有该控件的sort属性要为false

	local_serial_port.SetComNumber(&m_com_number);				//	设置串口号控件
	local_serial_port.SetBaudRate(&m_baud);						//	设置波特率控件
	local_serial_port.SetParity(&m_parity);						//	设置校验位
	local_serial_port.SetStopBits(&m_stop);						//	设置停止位
	local_serial_port.SetDataBits(8);							//	8位数据位，此处为固定参数

	int re;
	if ((re = local_serial_port.Initiation(&LocalSerialRecvFunc,1024,1024)) == INIT_OK)
	{
		//AfxMessageBox(L"初始化成功");
	} 
	else
	{
	CString str;
	str.Format(L"初始化失败，错误代码:%d",re);
	AfxMessageBox(str);
	}

然后参数检查可以这样使用

	local_serial_port.UpdateWidgetToConfig();				//	更新控件设置到结构缓存
	if (local_serial_port.CheckSetup() == SETUP_OK)
	{
		//	此时为串口参数设置正确
		//MessageBox(L"参数设置正确！");
		//	其他用户代码
		.........
	} 
	else
	{
		MessageBox(L"错误的参数设置！");
	}

对串口的操作
打开		local_serial_port.Open();
发送		local_serial_port.Send(length,pbuff);		//	lenght为要发送的字节数目
													//	pbuff为要发送数据所在的首地址

接收处理函数
void LocalSerialRecvFunc(
int nbytes,					//	进入缓冲区的字节数
unsigned char* dat			//	数据的首地址
)
{
	//	在此添加接收用户代码
	//	nbytes为本次收到的字节数目
	//	dat为收到数据所在的首地址
}
关闭		local_serial_port.Close();		
*********************************************************************/

//-----------------------------------------------------------------------
//	WinCE6.0支持
#include <CommCtrl.h>

#ifndef INVALID_LINK_INDEX
#define INVALID_LINK_INDEX  (-1)
#endif

//==================================================================================================
//	宏定义

//	初始换返回相关
#define INIT_OK							0							//	初始化OK
#define INIT_MISSING_MAIN_CONFIG		1							//	缺少主要配置,或者配置不正确
#define INIT_RECVFUNC_ERROR				2							//	接收处理函数指针非法
#define INIT_BUFFSIZE_ERROR				3							//	缓冲区大小设置错误
#define INIT_MALLOC_ERROR				4							//	申请内存错误
#define INIT_THREADCREAT_FAIL			5							//	线程创建失败

#define MIN_BUFFER_SIZE					128							//	最小缓冲区大小为128个字节

//	打开串口的返回值
#define OPEN_OK							0							//	打开成功
#define OPEN_NO_INIT					1							//	类没有初始化
#define OPEN_YET						2							//	串口已经打开
#define OPEN_COM_NUMBER_ERROR			3							//	串口号错误
#define OPEN_FAIL						4							//	打开失败
#define OPEN_COM_DCB_ERROR				5							//	串口参数设置错误

//	发送数据的返回值，当返回大于0的时候，为已发出的字节数目，当小于等于0时，定义如下
#define SEND_OK							0							//	发送成功
#define SEND_COM_HANDLE_ERROR			1							//	串口句柄错误,或者串口没有打开
#define SEND_OVER_BUFFER_SIZE			2							//	实际要发送的内容超过缓冲区的大小
#define SEND_FAIL						3							//	发送失败，可以调用GetLastError函数获取详细错误信息
#define SEND_BUSY						4							//	发送忙

//	关闭串口的返回值
#define	CLOSE_OK						0							//	关闭成功
#define CLOSE_YET						1							//	串口已经关闭

//	检查串口号合法性的返回值
#define CHECK_NCOM_STR_OK				0							//	检测通过
#define CHECK_NCOM_STR_FAIL				-1							//	不合法

//	串口配置检查函数返回值
#define CHECK_DCB_OK					0							//	检测通过
#define CHECK_DCB_BAUD_ERROR			1							//	非法波特率
#define	CHECK_DCB_DATABITS_ERROR		2							//	非法数据位
#define CHECK_DCB_PARITY_ERROR			3							//	非法校验位
#define	CHECK_DCB_STOPBITS_ERROR		4							//	非法停止位
#define CHECK_DCB_NULL_POINT			5							//	空的DCB结构指针

//	参数可用值的定义
#define CONFIG_NULL						0							//	参数没有准备好
#define CONFIG_OK						1							//	参数合法

//	线程控制代码定义
#define THREAD_STOP						0							//	线程停止
#define THREAD_RUN						1							//	线程运行

//	串口搜索定义
#define MIN_SERIALPORT_NUM				1							//	最小串口号
#define	MAX_SERIALPORT_NUM				100							//	最大串口号

//	相关设置函数的返回值,及参数检查函数的返回值
#define SETUP_OK						0							//	设置成功
#define SETUP_FAIL						1							//	设置失败

//	线程返回代码
#define THREAD_EXIT						0							//	线程正常退出
#define THREAD_COM_STR_ERROR			1							//	检查到串口字符串错误
#define THREAD_COM_DCB_ERROR			2							//	检查到串口设置结构体错误
#define THREAD_UNKNOW_ERROR				3							//	发送未知错误
#define THREAD_SEND_ERROR				4							//	发送错误
#define THREAD_RECV_ERROR				5							//	接收错误
#define THREAD_OPEN_FAIL				6							//	打开串口失败
#define THREAD_CREATE_FAIL				7							//	线程的创建和传递参数错误

//	发送判忙函数返回定义
//#define SEND_BUSY													//	忙，重定义
#define SEND_READY						0							//	发送准备好

//==================================================================================================
//	工作开关、选项
//#define SERIALPORT_DEBUG_LOG										//	调试日志开关选项

//==================================================================================================
//	全局线程函数定义
//	该函数为线程函数，由于创建线程的时候会独立为每个线程分配内存
//	并且为了配合CreateThread函数的使用，所以这里定义为类外部的函数
DWORD WINAPI CSerialPort_ThreadFunction(LPVOID lpParameter);

//==================================================================================================
//	类定义
class CSerialPort
{
//	公有的
public:
	//	构造函数
	CSerialPort();

	//	析构函数
	~CSerialPort();

	//-----------------------------------------------------------------------------------------
	//	初始化
	int Initiation(
		void (*pFunc)(int nbytes,unsigned char* dat),				//	接收到数据的处理函数指针
		int buff_size,												//	缓冲区尺寸
		int send_buff_size											//	发送缓冲区尺寸
		);

	//-----------------------------------------------------------------------------------------
	//	释放函数
	void Release(void);

	//-----------------------------------------------------------------------------------------
	//	直接设置串口号
	int SetComNumber(
		CString ncom_str											//	它的内容为串口的打开字符串，例如"\\\\.\\COM2"
		);			
	//	通过控件进行设置串口号，支持搜索、选定等
	int SetComNumber(
		CComboBox* pcom												//	指定了CComboBox控件，用于搜索和选择串口号
		);

	//-----------------------------------------------------------------------------------------
	//	直接设置波特率
	int SetBaudRate(
		int baud													//	直接指定波特率，像CBR_115200这样的定义，定义在WINBASE.H文件中
		);
	//	通过控件设置波特率
	int SetBaudRate(
		CComboBox* pbaud											//	指定了CComboBox控件，用于设置波特率
		);

	//-----------------------------------------------------------------------------------------
	//	设置校验位
	int SetParity(
		int parity													//	直接指定校验位，像NOPARITY这样的定义，定义在WINBASE.H文件中
		);
	//	通过控件设置校验位
	int SetParity(
		CComboBox* pparity											//	指定了CComboBox控件，用于设置校验位
		);

	//-----------------------------------------------------------------------------------------
	//	设置停止位
	int SetStopBits(
		int stopbits												//	直接指定停止位，像ONESTOPBIT这样的定义，定义在WINBASE.H文件中
		);
	//	通过控件设置停止位
	int SetStopBits(
		CComboBox* pstopbits										//	指定了CComboBox控件，用于设置停止位
		);

	//-----------------------------------------------------------------------------------------
	//	设置数据位
	int SetDataBits(
		int databits												//	直接指定数据位，取值范围一般在6~8
		);
	//	通过控件设置数据位
	int SetDataBits(
		CComboBox* pdatabits										//	指定了CComboBox控件，用于设置数据位
		);
	
	//-----------------------------------------------------------------------------------------
	//	采用DCB结构体进行设置
	int LoadConfig(
		DCB* pdcb													//	要进行设置的DCB结构体指针
		);

	//----------------------------------------------------------------------------------------
	//	更新控件上面的参数到配置
	//	返回内容为 SETUP_OK  或者 SETUP_FAIL
	int UpdateWidgetToConfig(void);

	//-----------------------------------------------------------------------------------------
	//	搜索串口
	//	这里采用暴力搜索，从最小值依次打开到最大值
	//	该函数的返回值为搜索到的串口数目
	int SearchSerialPort(void);

	//-----------------------------------------------------------------------------------------
	//	打开串口
	int Open(void);

	//-----------------------------------------------------------------------------------------
	//	关闭串口
	void Close(void);

	//-----------------------------------------------------------------------------------------
	//	发送数据
	int Send(
		int length,													//	要发送的数据的长度
		unsigned char* dat											//	数据所在的首地址
		);
	
	//-----------------------------------------------------------------------------------------
	//	参数检查测试函数,检查串口号设置，及DCB结构
	int CheckSetup(void);

	//-----------------------------------------------------------------------------------------
	//	串口线程函数
	DWORD ComThreadProc(LPVOID lpParameter);

	//-----------------------------------------------------------------------------------------
	CString log_str;					//	日志字符串
	
	//-----------------------------------------------------------------------------------------
	int SendBusy(void);					//	发送忙判断函数

//	私有的
private:
	//	缓冲区首地址指针
	unsigned char* pSendData_Buffer;
	unsigned char* pRecvData_Buffer;

	//	当串口有数据接收到的时候，调用这个函数指针所指向的函数
	void (*pSerialRecv_Func)(
		int nbytes,					//	进入缓冲区的字节数
		unsigned char* dat			//	数据的首地址
		);
		
	//-----------------------------------------------------------------------------------------
	//	一些控件类的变量
	//	默认情况下这些参数均为NULL,只有当指定了内容才会对相应的控件进行操作
	CComboBox* pComboBox_ComNumber;					//	串口号
	CComboBox* pComboBox_BaudRate;					//	波特率
	CComboBox* pComboBox_Parity;					//	校验位
	CComboBox* pComboBox_StopBits;					//	停止位
	CComboBox* pComboBox_DataBits;					//	数据位

	//-----------------------------------------------------------------------------------------
	//	直接控制串口设置的参数
	HANDLE com_handle;								//	打开后的串口句柄		这个参数将直接反应串口的状态，当为NULL时，串口为关闭状态，否则为打开
													//	该参数在线程里面为读写属性，线程外为只读，初始化的时候为读写
	CString com_number_str;							//	串口号字符串
													//	该参数在线程里面为读属性，线程外为读写属性
	DCB	com_config;									//	串口参数配置结构体
													//	该参数在线程里面为读属性，线程外为读写属性
	HANDLE hThreadHandle;							//	线程句柄
	int ThreadControlCode;							//	线程控制代码
													//	THREAD_STOP为线程关闭，THREAD_RUN为线程运行
	DWORD ThreadErrorCode;							//	定义错误代码
	DWORD RecvBytesNum;								//	每次接收到的字节数
	DWORD SendBytesNum;								//	每次发送的字节数


	//-----------------------------------------------------------------------------------------
	//	其他功能子函数
	//	检查串口号字符串的合法性
	int CheckComNumberString(CString str);

	//	检查串口设置
	int CheckComDCB(DCB* pdcb);

//	保密的
protected:

};

#endif		//	__com_h__

