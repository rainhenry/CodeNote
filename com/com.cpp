
/*---------------------------------------------------------------------------
2016.07.15
	串口通信源文件

工程名称		串口类
工程创建日期	2016年7月15日
工程完结日期	2016年7月24日
设计：		rainhenry
代码：		rainhenry
调试/校对：	rainhenry
测试环境：	VS2010
测试平台：	Windows XP SP3 
代码版本：	REV 2.0

说明：	本代码可以被任何人使用和转载，但转载/使用本代码时，请保留本注释。
本代码仅供参考和学习，切勿用于商业用途

----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "com.h"

//-----------------------------------------------------------------------------------------------------------------------------
//	CSerialPort类相关函数
//	构造函数
CSerialPort::CSerialPort()
{
	//	初始化所有参数
	this->ThreadControlCode = THREAD_STOP;									//	关闭线程
	memset((void*)(&(this->com_config)),0,sizeof(this->com_config));		//	将配置结构体清空
	this->com_handle = NULL;												//	串口句柄为空
	this->hThreadHandle = NULL;												//	线程句柄为空
	this->com_number_str = "";												//	串口字符串为空

	this->pComboBox_BaudRate = NULL;										//	波特率控件
	this->pComboBox_ComNumber = NULL;										//	串口号控件
	this->pComboBox_DataBits = NULL;										//	数据位
	this->pComboBox_Parity = NULL;											//	检验位
	this->pComboBox_StopBits = NULL;										//	停止位

	this->pRecvData_Buffer = NULL;											//	接收缓冲区首地址
	this->pSendData_Buffer = NULL;											//	发送缓冲区首地址

	this->ThreadControlCode = NULL;											//	清空错误代码
	this->RecvBytesNum = 0;													//	清空接收字节数
	this->SendBytesNum = 0;													//	清空发送字节数

	this->log_str = L"";													//	清空日志
}

//	析构函数
CSerialPort::~CSerialPort()
{
	//	调用释放函数
	this->Release();
}

//	初始化
int CSerialPort::Initiation(
	void (*pFunc)(int nbytes,unsigned char* dat),				//	接收到数据的处理函数指针
	int recv_buff_size,											//	接收缓冲区尺寸
	int send_buff_size											//	发送缓冲区尺寸
	)
{
	//----------------------------------------------------------------------
	//	根据指定的控件指针初始化控件设置
	//	串口号控件
	if ((this->pComboBox_ComNumber != NULL)&&(this->pComboBox_ComNumber != INVALID_HANDLE_VALUE))		//	检测控件指针是否合法
	{
		//	检测串口列表是否有
		if (this->pComboBox_ComNumber->GetCount() == 0)							//	没有数据
		{
			this->pComboBox_ComNumber->ResetContent();							//	清空组合框内容
			this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
			this->pComboBox_ComNumber->AddString(L"请先搜索端口……");			//	添加一个内容
			this->pComboBox_ComNumber->SetCurSel(0);							//	选择第0个
			this->pComboBox_ComNumber->EnableWindow(FALSE);						//	让该控件操作无效
		} 
		else																	//	已经有串口了
		{
		}
	}

	//	波特率控件
	if ((this->pComboBox_BaudRate != NULL)&&(this->pComboBox_BaudRate != INVALID_HANDLE_VALUE))		//	检测控件指针是否合法
	{
		this->pComboBox_BaudRate->ResetContent();							//	清空组合框内容
		this->pComboBox_BaudRate->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
		this->pComboBox_BaudRate->AddString(L"110");           this->pComboBox_BaudRate->SetItemData(0,CBR_110);
		this->pComboBox_BaudRate->AddString(L"300");           this->pComboBox_BaudRate->SetItemData(1,CBR_300);
		this->pComboBox_BaudRate->AddString(L"600");           this->pComboBox_BaudRate->SetItemData(2,CBR_600);
		this->pComboBox_BaudRate->AddString(L"1200");          this->pComboBox_BaudRate->SetItemData(3,CBR_1200);
		this->pComboBox_BaudRate->AddString(L"2400");          this->pComboBox_BaudRate->SetItemData(4,CBR_2400);
		this->pComboBox_BaudRate->AddString(L"4800");          this->pComboBox_BaudRate->SetItemData(5,CBR_4800);
		this->pComboBox_BaudRate->AddString(L"9600");          this->pComboBox_BaudRate->SetItemData(6,CBR_9600);
		this->pComboBox_BaudRate->AddString(L"14400");         this->pComboBox_BaudRate->SetItemData(7,CBR_14400);
		this->pComboBox_BaudRate->AddString(L"19200");         this->pComboBox_BaudRate->SetItemData(8,CBR_19200);
		this->pComboBox_BaudRate->AddString(L"38400");         this->pComboBox_BaudRate->SetItemData(9,CBR_38400);
		this->pComboBox_BaudRate->AddString(L"56000");         this->pComboBox_BaudRate->SetItemData(10,CBR_56000);
		this->pComboBox_BaudRate->AddString(L"57600");         this->pComboBox_BaudRate->SetItemData(11,CBR_57600);
		this->pComboBox_BaudRate->AddString(L"115200");        this->pComboBox_BaudRate->SetItemData(12,CBR_115200);
		this->pComboBox_BaudRate->AddString(L"128000");        this->pComboBox_BaudRate->SetItemData(13,CBR_128000);
		this->pComboBox_BaudRate->AddString(L"256000");        this->pComboBox_BaudRate->SetItemData(14,CBR_256000);
		this->pComboBox_BaudRate->SetCurSel(12);
		this->pComboBox_BaudRate->EnableWindow(TRUE);						//	让该控件操作有效
	}

	//	校验位控件初始化
	if ((this->pComboBox_Parity != NULL)&&(this->pComboBox_Parity != INVALID_HANDLE_VALUE))		//	检测控件指针是否合法
	{
		this->pComboBox_Parity->ResetContent();							//	清空组合框内容
		this->pComboBox_Parity->ModifyStyle(CBS_SORT,0);				//	设置为非排序样式
		this->pComboBox_Parity->AddString(L"无校验");          this->pComboBox_Parity->SetItemData(0,(unsigned long)NOPARITY);
		this->pComboBox_Parity->AddString(L"奇校验");          this->pComboBox_Parity->SetItemData(1,(unsigned long)ODDPARITY);
		this->pComboBox_Parity->AddString(L"偶校验");          this->pComboBox_Parity->SetItemData(2,(unsigned long)EVENPARITY);
		this->pComboBox_Parity->AddString(L"标志校验");        this->pComboBox_Parity->SetItemData(3,(unsigned long)MARKPARITY);
		this->pComboBox_Parity->AddString(L"空格校验");        this->pComboBox_Parity->SetItemData(4,(unsigned long)SPACEPARITY);
		this->pComboBox_Parity->SetCurSel(0);
		this->pComboBox_Parity->EnableWindow(TRUE);						//	让该控件操作有效
	}

	//	数据位控件初始化
	if ((this->pComboBox_DataBits != NULL)&&(this->pComboBox_DataBits != INVALID_HANDLE_VALUE))		//	检测控件指针是否合法
	{
		this->pComboBox_DataBits->ResetContent();							//	清空组合框内容
		this->pComboBox_DataBits->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
		this->pComboBox_DataBits->AddString(L"6");            this->pComboBox_DataBits->SetItemData(0,(unsigned long)6);
		this->pComboBox_DataBits->AddString(L"7");            this->pComboBox_DataBits->SetItemData(1,(unsigned long)7);
		this->pComboBox_DataBits->AddString(L"8");            this->pComboBox_DataBits->SetItemData(2,(unsigned long)8);
		this->pComboBox_DataBits->SetCurSel(2);
		this->pComboBox_DataBits->EnableWindow(TRUE);						//	让该控件操作有效
	}

	//	停止位控件初始化
	if ((this->pComboBox_StopBits != NULL)&&(this->pComboBox_StopBits != INVALID_HANDLE_VALUE))		//	检测控件指针是否合法
	{
		this->pComboBox_StopBits->ResetContent();							//	清空组合框内容
		this->pComboBox_StopBits->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
		this->pComboBox_StopBits->AddString(L"1");            this->pComboBox_StopBits->SetItemData(0,(unsigned long)ONESTOPBIT);
		this->pComboBox_StopBits->AddString(L"2");            this->pComboBox_StopBits->SetItemData(1,(unsigned long)TWOSTOPBITS);
		this->pComboBox_StopBits->SetCurSel(0);
		this->pComboBox_StopBits->EnableWindow(TRUE);						//	让该控件操作有效
	}
	
	//	检查接收函数的指针
	if ((pFunc == NULL)||(pFunc == INVALID_HANDLE_VALUE))
	{
		return INIT_RECVFUNC_ERROR;
	} 
	else
	{
		this->pSerialRecv_Func = pFunc;
	}

	//	检查缓冲区设置参数
	//	这里为了更好的接收和发送数据，规定了最小的缓冲区尺寸
	if ((recv_buff_size >= MIN_BUFFER_SIZE) && (send_buff_size >= MIN_BUFFER_SIZE))
	{
		//	检查接收缓冲区是否为空
		//	此代码主要防止再没有调用释放的时候，再一次调用初始化函数，导致内存泄漏现象
		if ((this->pRecvData_Buffer != NULL)&&(this->pRecvData_Buffer != INVALID_HANDLE_VALUE))
		{
			delete this->pRecvData_Buffer;			//	释放掉旧的内存分配
		} 
		
		//	然后进行分配接收缓冲区的内存
		this->pRecvData_Buffer = new unsigned char[recv_buff_size];
		if ((this->pRecvData_Buffer == NULL)||(this->pRecvData_Buffer == INVALID_HANDLE_VALUE))
		{
			//	内存分配失败
			return INIT_MALLOC_ERROR;
		} 
		else
		{
		}

		//	检查发送缓冲区是否为空
		//	此代码主要防止再没有调用释放的时候，再一次调用初始化函数，导致内存泄漏现象
		if ((this->pSendData_Buffer != NULL)&&(this->pSendData_Buffer != INVALID_HANDLE_VALUE))
		{
			delete this->pSendData_Buffer;			//	释放掉旧的内存分配
		} 

		//	分配发送缓冲区内存
		this->pSendData_Buffer = new unsigned char[send_buff_size];
		if ((this->pSendData_Buffer == NULL)||(this->pSendData_Buffer == INVALID_HANDLE_VALUE))
		{
			//	首先释放掉刚才申请成功的内存
			delete this->pRecvData_Buffer;

			//	然后返回错误代码
			return INIT_MALLOC_ERROR;
		} 
		else
		{
		}
	} 
	else
	{
		return INIT_BUFFSIZE_ERROR;
	}
		
	//	此时所有初始化工作都已经正确完成
	return INIT_OK;
}


//-----------------------------------------------------------------------------------------
//	释放函数
void CSerialPort::Release(void)
{
	this->ThreadControlCode = THREAD_STOP;									//	关闭线程
	Sleep(500);																//	等待线程自动结束

	//	归位所有参数
	memset((void*)(&(this->com_config)),0,sizeof(this->com_config));		//	将配置结构体清空

	//	串口句柄
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->hThreadHandle);
	} 
	this->com_handle = NULL;												//	串口句柄为空

	//	线程句柄
	if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
	{
		//	定义线程状态获取结果变量
		DWORD status = NULL;

		//	尝试获取线程的工作状态
		if (GetExitCodeProcess(this->hThreadHandle, &status) != 0)
		{
			//	若线程还活着
			if (status == STILL_ACTIVE)
			{
				//	强制关闭线程
				TerminateThread(this->hThreadHandle, status);
				//	释放句柄
				CloseHandle(this->hThreadHandle);
			} 
			//	线程已经结束了
			else
			{
				//	释放句柄
				CloseHandle(this->hThreadHandle);
			}
		} 
		//	获取状态失败
		else
		{
			//	强制关闭线程
			TerminateThread(this->hThreadHandle, status);
			//	释放句柄
			CloseHandle(this->hThreadHandle);
		}
	}
	this->hThreadHandle = NULL;												//	线程句柄为空

	this->com_number_str = "";												//	串口字符串为空

	this->pComboBox_BaudRate = NULL;										//	波特率控件
	this->pComboBox_ComNumber = NULL;										//	串口号控件
	this->pComboBox_DataBits = NULL;										//	数据位
	this->pComboBox_Parity = NULL;											//	检验位
	this->pComboBox_StopBits = NULL;										//	停止位

	//	释放接收缓冲区内存
	if ((this->pRecvData_Buffer != NULL)&&(this->pRecvData_Buffer != INVALID_HANDLE_VALUE))
	{
		delete this->pRecvData_Buffer;
	}
	this->pRecvData_Buffer = NULL;											//	接收缓冲区首地址

	//	释放发送缓冲区内存
	if ((this->pSendData_Buffer != NULL)&&(this->pSendData_Buffer != INVALID_HANDLE_VALUE))
	{
		delete this->pSendData_Buffer;
	}
	this->pSendData_Buffer = NULL;											//	发送缓冲区首地址

	this->ThreadControlCode = NULL;											//	清空错误代码
	this->RecvBytesNum = 0;													//	清空接收字节数
	this->SendBytesNum = 0;													//	清空发送字节数
}

//-----------------------------------------------------------------------------------------
//	直接设置串口号
int CSerialPort::SetComNumber(
	CString ncom_str											//	它的内容为串口的打开字符串，例如"\\\\.\\COM2"
	)
{
	//	检查参数
	if (CHECK_NCOM_STR_OK == this->CheckComNumberString(ncom_str))
	{
		this->com_number_str = ncom_str;
		return SETUP_OK;
	} 
	else
	{
		//	设置失败
		return SETUP_FAIL;
	}
	
}

//	通过控件进行设置串口号，支持搜索、选定等
int CSerialPort::SetComNumber(
	CComboBox* pcom												//	指定了CComboBox控件，用于搜索和选择串口号
	)
{
	//	参数检查
	if ((pcom != NULL)&&(pcom != INVALID_HANDLE_VALUE))
	{
		//	设置成功
		this->pComboBox_ComNumber = pcom;
		return SETUP_OK;
	} 
	else
	{
		//	设置失败
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	直接设置波特率
int CSerialPort::SetBaudRate(
	int baud													//	直接指定波特率，像CBR_115200这样的定义，定义在WINBASE.H文件中
	)
{
	//	检查波特率
	if ((baud == CBR_110) || \
		(baud == CBR_300) || \
		(baud == CBR_600) || \
		(baud == CBR_1200) || \
		(baud == CBR_2400) || \
		(baud == CBR_4800) || \
		(baud == CBR_9600) || \
		(baud == CBR_14400) || \
		(baud == CBR_19200) || \
		(baud == CBR_38400) || \
		(baud == CBR_56000) || \
		(baud == CBR_57600) || \
		(baud == CBR_115200) || \
		(baud == CBR_128000) || \
		(baud == CBR_256000) \
		)
	{
		//	设置成功
		this->com_config.BaudRate = baud;
		return SETUP_OK;
	} 
	else
	{
		//	波特率取值错误
		return SETUP_FAIL;
	}
}

//	通过控件设置波特率
int CSerialPort::SetBaudRate(
	CComboBox* pbaud											//	指定了CComboBox控件，用于设置波特率
	)
{
	//	检查参数
	if ((pbaud != NULL)&&(pbaud != INVALID_HANDLE_VALUE))
	{
		//	设置OK
		this->pComboBox_BaudRate = pbaud;
		return SETUP_OK;
	} 
	else
	{
		//	设置失败
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	设置校验位
int CSerialPort::SetParity(
	int parity													//	直接指定校验位，像NOPARITY这样的定义，定义在WINBASE.H文件中
	)
{
	//	检测校验位设置
	if ((parity == NOPARITY) || \
		(parity == ODDPARITY) || \
		(parity == EVENPARITY) || \
		(parity == MARKPARITY) || \
		(parity == SPACEPARITY) \
		)
	{
		//	设置成功
		if (parity == NOPARITY)			//	无校验
		{
			this->com_config.fParity = FALSE;
			this->com_config.Parity = NOPARITY;
		} 
		else							//	有校验
		{
			this->com_config.fParity = TRUE;
			this->com_config.Parity = parity;
		}

		return SETUP_OK;
	} 
	else
	{
		//	校验位设置错误
		return SETUP_FAIL;
	}
}

//	通过控件设置校验位
int CSerialPort::SetParity(
	CComboBox* pparity											//	指定了CComboBox控件，用于设置校验位
	)
{
	//	参数检查
	if ((pparity != NULL)&&(pparity != INVALID_HANDLE_VALUE))
	{
		//	设置成功
		this->pComboBox_Parity = pparity;
		return SETUP_OK;
	} 
	else
	{
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	设置停止位
int CSerialPort::SetStopBits(
	int stopbits												//	直接指定停止位，像ONESTOPBIT这样的定义，定义在WINBASE.H文件中
	)
{
	//	检测停止位
	if ((stopbits == ONESTOPBIT) || \
		(stopbits == TWOSTOPBITS) \
		)
	{
		//	设置成功
		this->com_config.StopBits = stopbits;
		return SETUP_OK;
	} 
	else
	{
		//	检查停止位出错
		return SETUP_FAIL;
	}
}

//	通过控件设置停止位
int CSerialPort::SetStopBits(
	CComboBox* pstopbits										//	指定了CComboBox控件，用于设置停止位
	)
{
	//	检查参数
	if ((pstopbits != NULL)&&(pstopbits != INVALID_HANDLE_VALUE))
	{
		this->pComboBox_StopBits = pstopbits;
		return SETUP_OK;
	} 
	else
	{
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	设置数据位
int CSerialPort::SetDataBits(
	int databits												//	直接指定数据位，取值范围一般在6~8
	)
{
	//	检测数据位
	if ((databits == 6) || \
		(databits == 7) || \
		(databits == 8) \
		)
	{
		//	设置成功
		this->com_config.ByteSize = databits;
		return SETUP_OK;
	} 
	else
	{
		//	校验数据位错误
		return SETUP_FAIL;
	}
}

//	通过控件设置数据位
int CSerialPort::SetDataBits(
	CComboBox* pdatabits										//	指定了CComboBox控件，用于设置数据位
	)
{
	//	检查参数
	if ((pdatabits != NULL)&&(pdatabits != INVALID_HANDLE_VALUE))
	{
		//	设置成功
		this->pComboBox_DataBits = pdatabits;
		return SETUP_OK;
	} 
	else
	{
		//	配置失败
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	采用DCB结构体进行设置
int CSerialPort::LoadConfig(
	DCB* pdcb													//	要进行设置的DCB结构体指针
	)
{
	//	参数检查
	if ((pdcb != NULL)&&(pdcb != INVALID_HANDLE_VALUE))
	{
		//	检查具体设置
		if (this->CheckComDCB(pdcb) == CHECK_DCB_OK)
		{
			//	所有参数设置都正常
			this->com_config = *pdcb;			//	复制设置
			return SETUP_OK;
		} 
		else
		{
			//	有非法参数
			return SETUP_FAIL;
		}
	} 
	else
	{
		//	设置失败
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	搜索串口
//	这里采用暴力搜索，从最小值依次打开到最大值
//	该函数的返回值为搜索到的串口数目
//	使用该函数的目的是为了让控件支持搜索功能，当未定义控件的时候，就不会执行该函数的主要部分
int CSerialPort::SearchSerialPort(void)
{
	//	检测当前对象是否有打开的串口,防止资源泄露
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->com_handle);				//	关闭当前串口
		this->com_handle = NULL;
	}

	//	串口号控件
	if ((this->pComboBox_ComNumber != NULL)&&(this->pComboBox_ComNumber != INVALID_HANDLE_VALUE))		//	检测控件指针是否合法
	{
		//	清空控件状态
		this->pComboBox_ComNumber->ResetContent();							//	清空组合框内容
		this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
		this->pComboBox_ComNumber->EnableWindow(FALSE);						//	让该控件操作无效

		//	进入搜索程序
		int i;																//	定义循环变量
		CString open_str;													//	定义打开字符串缓存
		HANDLE tmp_handle;													//	临时打开句柄
		for (i=MIN_SERIALPORT_NUM;i<=MAX_SERIALPORT_NUM;i++)
		{
			tmp_handle = NULL;												//	清空句柄
			open_str = "";													//	清空缓存
			open_str.Format(L"\\\\.\\COM%d",i);								//	生成打开字符串

			//	尝试打开该串口
			tmp_handle = CreateFile(
				open_str,
				GENERIC_READ|GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
				);

			//	检查打开是否成功
			if ((tmp_handle == NULL)||(tmp_handle == INVALID_HANDLE_VALUE))			//	不正确的返回句柄
			{
			} 
			//	打开成功
			else
			{
				//	生成添加字符串
				open_str = "";													//	清空缓存
				open_str.Format(L"COM%d",i);									//	生成字符串

				//	添加串口字符串到控件，并设置串口号数据
				this->pComboBox_ComNumber->AddString(open_str);
				this->pComboBox_ComNumber->SetItemData(this->pComboBox_ComNumber->GetCount()-1,i);

				//	关闭串口
				CloseHandle(tmp_handle);
			}
		}

		//	统计已经搜索到的串口
		int count = this->pComboBox_ComNumber->GetCount();
		if (count == 0)															//	找到的总数为0
		{
			this->pComboBox_ComNumber->ResetContent();							//	清空组合框内容
			this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
			this->pComboBox_ComNumber->AddString(L"没有找到可用的串口！");		//	添加一个内容
			this->pComboBox_ComNumber->SetCurSel(0);							//	选择第0个
			this->pComboBox_ComNumber->EnableWindow(FALSE);						//	让该控件操作无效

			return NULL;
		} 
		else
		{
			this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	设置为非排序样式
			this->pComboBox_ComNumber->SetCurSel(0);							//	选择第0个
			this->pComboBox_ComNumber->EnableWindow(TRUE);						//	让该控件操作有效

			return count;
		}
	}
	else
	{
		return INVALID_LINK_INDEX;
	}

}


//-----------------------------------------------------------------------------------------
//	串口线程函数
DWORD CSerialPort::ComThreadProc(LPVOID lpParameter)
{
	//	应用最新配置
	//this->UpdateWidgetToConfig();	//	经过实验发现，在线程中处理控件会导致线程异常退出

	//AfxMessageBox(L"线程检查串口字符串");

	//	串口字符串参数检查
	if (this->CheckComNumberString(this->com_number_str) != CHECK_NCOM_STR_OK)
	{
		return THREAD_COM_STR_ERROR;
	}

	//	串口参数设置检查
	if (this->CheckComDCB(&(this->com_config)) != CHECK_DCB_OK)
	{
		return THREAD_COM_DCB_ERROR;
	}

	//	判断是否有已经打开的串口
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->com_handle);		//	关闭已经打开的句柄
		this->com_handle = NULL;
	}

	//	打开串口
	this->com_handle = CreateFile(
		this->com_number_str,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//	判断是否打开成功
	if ((this->com_handle == NULL)||(this->com_handle == INVALID_HANDLE_VALUE))
	{
		return THREAD_OPEN_FAIL;
	}

	//	载入参数
	DCB tmp_dcb;
	GetCommState(this->com_handle,&tmp_dcb);
	tmp_dcb.BaudRate=this->com_config.BaudRate;
	tmp_dcb.fBinary=TRUE;
	tmp_dcb.fParity=this->com_config.fParity;
	tmp_dcb.ByteSize=this->com_config.ByteSize;
	tmp_dcb.Parity=this->com_config.Parity;
	tmp_dcb.StopBits=this->com_config.StopBits;
	SetCommState(this->com_handle,&tmp_dcb);	//	设置参数
	
	//	启动主循环
	COMSTAT ComState;							//	定义串行口状态结构
	this->ThreadControlCode = THREAD_RUN;		//	启动
	while(this->ThreadControlCode == THREAD_RUN)
	{
		//	判断是否出现未知错误
		if (
		ClearCommError(
			this->com_handle,
			&(this->ThreadErrorCode),
			&ComState
			) != 0)
		//------------------------------------------
		//	此时接收正确
		{
			//----------------------------------------------------------
			//	接收处理程序
			if (ComState.cbInQue > 0)			//	接收的字节数大于0
			{
				if (0 != ReadFile(
					this->com_handle,			//	串口句柄
					this->pRecvData_Buffer,		//	缓冲区
					ComState.cbInQue,			//	要接收的字节
					&(this->RecvBytesNum),		//	实际接收的字节
					NULL))
				{
					if (ComState.cbInQue == this->RecvBytesNum)			//	数据接收正确
					{
						//	调用用户接收函数
						this->pSerialRecv_Func(this->RecvBytesNum,this->pRecvData_Buffer);
					} 
					else													//	接收异常
					{
						CloseHandle(this->com_handle);				//	关闭串口
						this->com_handle = NULL;
						this->ThreadControlCode = THREAD_STOP;		//	停止
						return THREAD_RECV_ERROR;					//	发生接收错误
					}
				} 
				//	错误读取
				else
				{
					CloseHandle(this->com_handle);				//	关闭串口
					this->com_handle = NULL;
					this->ThreadControlCode = THREAD_STOP;		//	停止
					return THREAD_RECV_ERROR;					//	发生接收错误
				}
			}
			//----------------------------------------------------------
			//	发送处理程序
			else
			{
				//	若此时有要发送的数据
				if (this->SendBytesNum > 0)
				{
					//	发送缓冲区为空，此时可以正常发送
					if (ComState.cbOutQue==0)
					{
						//AfxMessageBox(L"尝试发送");
						//	尝试发送
						DWORD SendBytes;			//	定义实际发送的字节数目
						if (0 != WriteFile(
							this->com_handle,
							this->pSendData_Buffer,
							this->SendBytesNum,
							&SendBytes,
							NULL
							))
						//	发送操作成功
						{
							//	若理论发送的字节数目和实际发送的字节数目相同
							if (SendBytes == this->SendBytesNum)
							{
								//	此时发送是正确的
								this->SendBytesNum = 0;						//	清空要发送的字节数目
							} 
							else
							{
								//	发送错误
								CloseHandle(this->com_handle);				//	关闭串口
								this->com_handle = NULL;
								this->ThreadControlCode = THREAD_STOP;		//	停止
								return THREAD_SEND_ERROR;					//	发生发送错误
							}
						} 
						//	发送失败
						else
						{
							CloseHandle(this->com_handle);				//	关闭串口
							this->com_handle = NULL;
							this->ThreadControlCode = THREAD_STOP;		//	停止
							return THREAD_SEND_ERROR;					//	发生发送错误
						}
					}
					//	发送缓冲区有内容，由于采用的是同步方式操作的串口，所以此时应该为硬件错误导致
					else
					{
						CloseHandle(this->com_handle);				//	关闭串口
						this->com_handle = NULL;
						this->ThreadControlCode = THREAD_STOP;		//	停止
						return THREAD_UNKNOW_ERROR;					//	发生未知错误
					}
				}
				//	没有要发送的数据的时候进行休息，防止线程占用CPU
				else
				{
					Sleep(10);
				}
			}
		}
		//------------------------------------------
		//	此时出现错误
		else
		{
			CloseHandle(this->com_handle);				//	关闭串口
			this->com_handle = NULL;
			this->ThreadControlCode = THREAD_STOP;		//	停止
			return THREAD_UNKNOW_ERROR;					//	发生未知错误
		}
	}

	//	关闭串口
	CloseHandle(this->com_handle);
	this->com_handle = NULL;

	//	线程正确退出
	return THREAD_EXIT;
}

//-----------------------------------------------------------------------------------------
//	打开串口
int CSerialPort::Open(void)
{
	//	应用最新配置
	this->UpdateWidgetToConfig();

	//	串口字符串参数检查
	if (this->CheckComNumberString(this->com_number_str) != CHECK_NCOM_STR_OK)
	{
		return OPEN_COM_NUMBER_ERROR;
	}

	//	串口参数设置检查
	if (this->CheckComDCB(&(this->com_config)) != CHECK_DCB_OK)
	{
		return OPEN_COM_DCB_ERROR;
	}

	//	判断是否有已经打开的串口 或者 线程已经启动
	if (((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE)) || \
		((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE)))
	{
		this->Close();					//	调用关闭函数释放资源
	}

	//	打开串口
	this->com_handle = CreateFile(
		this->com_number_str,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//	判断是否打开成功
	if ((this->com_handle == NULL)||(this->com_handle == INVALID_HANDLE_VALUE))
	{
		return OPEN_FAIL;
	}
	else
	{
		//	关闭该串口
		CloseHandle(this->com_handle);
		this->com_handle = NULL;

		//	检查线程是否已经启动
		if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))		//	此时表示已经有线程工作了
		{
			this->ThreadControlCode = THREAD_STOP;					//	发送关闭命令
			Sleep(200);												//	等待自然关闭

			//	线程句柄的释放处理
			if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
			{
				//	定义线程状态获取结果变量
				DWORD status = NULL;

				//	尝试获取线程的工作状态
				if (GetExitCodeProcess(this->hThreadHandle, &status) != 0)
				{
					//	若线程还活着
					if (status == STILL_ACTIVE)
					{
						//	强制关闭线程
						TerminateThread(this->hThreadHandle, status);
						//	释放句柄
						CloseHandle(this->hThreadHandle);
					} 
					//	线程已经结束了
					else
					{
						//	释放句柄
						CloseHandle(this->hThreadHandle);
					}
				} 
				//	获取状态失败
				else
				{
					//	强制关闭线程
					TerminateThread(this->hThreadHandle, status);
					//	释放句柄
					CloseHandle(this->hThreadHandle);
				}
			}
			this->hThreadHandle = NULL;												//	线程句柄为空
		}

#ifdef SERIALPORT_DEBUG_LOG
		this->log_str += L"----尝试启动线程\r\n";
#endif

		//	启动线程
		this->ThreadControlCode = THREAD_RUN;		//	设定为启动
		DWORD ThreadID;						//	定义线程ID
		this->hThreadHandle = CreateThread(
			NULL,
			0,
			CSerialPort_ThreadFunction,
			this,                   //传递给线程处理函数的参数
			0,
			&ThreadID
			);

		//	打开成功
		return OPEN_OK;
	}
}

//-----------------------------------------------------------------------------------------
//	关闭串口
void CSerialPort::Close(void)
{
	this->ThreadControlCode = THREAD_STOP;					//	发送关闭命令
	Sleep(200);												//	等待自然关闭

	//	线程句柄的释放处理
	if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
	{
		//	定义线程状态获取结果变量
		DWORD status = NULL;

		//	尝试获取线程的工作状态
		if (GetExitCodeProcess(this->hThreadHandle, &status) != 0)
		{
			//	若线程还活着
			if (status == STILL_ACTIVE)
			{
				//	强制关闭线程
				TerminateThread(this->hThreadHandle, status);
				//	释放句柄
				CloseHandle(this->hThreadHandle);
			} 
			//	线程已经结束了
			else
			{
				//	释放句柄
				CloseHandle(this->hThreadHandle);
			}
		} 
		//	获取状态失败
		else
		{
			//	强制关闭线程
			TerminateThread(this->hThreadHandle, status);
			//	释放句柄
			CloseHandle(this->hThreadHandle);
		}
	}
	this->hThreadHandle = NULL;												//	线程句柄为空

	//	判断是否有已经打开的串口
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->com_handle);		//	关闭已经打开的句柄
	}
	this->com_handle = NULL;
}

//-----------------------------------------------------------------------------------------
//	发送数据
int CSerialPort::Send(
	int length,													//	要发送的数据的长度
	unsigned char* dat											//	数据所在的首地址
	)
{
#ifdef SERIALPORT_DEBUG_LOG
	this->log_str += L"----申请发送\r\n";
#endif

	//	若线程在工作
	if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
	{
		//	若当前发送不忙
		if (this->SendBytesNum == 0)
		{
			memcpy(this->pSendData_Buffer,dat,length);			//	复制要发送的内容到发送缓冲
			this->SendBytesNum = length;						//	设置要发送的字节数目

			return SEND_OK;										//	发送成功
		} 
		//	否则发送还未完成
		else
		{
			return SEND_BUSY;						//	发送忙
		}
	}
	//	否则线程还未启动
	else
	{
		return SEND_COM_HANDLE_ERROR;
	}
}


//-----------------------------------------------------------------------------------------
//	其他功能子函数
//	检查串口号字符串的合法性
int CSerialPort::CheckComNumberString(CString str)
{

	//	wince下串口号特殊处理
	return CHECK_NCOM_STR_OK;

	////	定义字符串中间变量
	//char tmp_str[100];

	//memset(tmp_str,0,sizeof(tmp_str));							//	清空临时字符串缓存
	//str.MakeUpper();											//	将所有小写字母转换为大写
	//sprintf(tmp_str,"%S",str);									//	格式化到字符缓存

	////	根据内容选择不同的检查方式
	//if (tmp_str[0] == 'C')
	//{
	//	//	并且前面的字符串为COM的话
	//	if ( (tmp_str[1] == 'O') && (tmp_str[2] == 'M') && (strlen(tmp_str) > 2))
	//	{
	//		//	循环检查后面的是否都是数字
	//		size_t i;
	//		for (i=3;i<strlen(tmp_str);i++)
	//		{
	//			if (('0' <= tmp_str[i])&&(tmp_str[i] <= '9'))
	//			{
	//			}
	//			else
	//			{
	//				//	发现非法字符
	//				return CHECK_NCOM_STR_FAIL;
	//			}
	//		}

	//		//	此时为正确格式
	//		return CHECK_NCOM_STR_OK;
	//	}
	//	else
	//	{
	//		//	错误
	//		return CHECK_NCOM_STR_FAIL;
	//	}
	//} 
	//else if(tmp_str[0] == '\\')
	//{
	//	//	并且前面的字符串为\\\\.\\COM的话
	//	if ( (tmp_str[1] == '\\') && (tmp_str[2] == '.') && (tmp_str[3] == '\\') && (tmp_str[4] == 'C') && (tmp_str[5] == 'O') && (tmp_str[6] == 'M') && (strlen(tmp_str) > 6))
	//	{
	//		//	循环检查后面的是否都是数字
	//		size_t i;
	//		for (i=7;i<strlen(tmp_str);i++)
	//		{
	//			if (('0' <= tmp_str[i])&&(tmp_str[i] <= '9'))
	//			{
	//			}
	//			else
	//			{
	//				//	发现非法字符
	//				return CHECK_NCOM_STR_FAIL;
	//			}
	//		}

	//		//	此时为正确格式
	//		return CHECK_NCOM_STR_OK;
	//	}
	//	else
	//	{
	//		//	错误
	//		return CHECK_NCOM_STR_FAIL;
	//	}
	//}
	//else
	//{
	//	//	非法格式
	//	return CHECK_NCOM_STR_FAIL;
	//}

	////	返回结果
	//return CHECK_NCOM_STR_FAIL;
}


//	检查串口设置
int CSerialPort::CheckComDCB(DCB* pdcb)
{
	//	检查参数地址的合法性
	if ((pdcb == NULL)||(pdcb == INVALID_HANDLE_VALUE))
	{
		//	空指针
		return CHECK_DCB_NULL_POINT;
	} 
	else
	{
	}

	//	检查波特率
	if ((pdcb->BaudRate == CBR_110) || \
		(pdcb->BaudRate == CBR_300) || \
		(pdcb->BaudRate == CBR_600) || \
		(pdcb->BaudRate == CBR_1200) || \
		(pdcb->BaudRate == CBR_2400) || \
		(pdcb->BaudRate == CBR_4800) || \
		(pdcb->BaudRate == CBR_9600) || \
		(pdcb->BaudRate == CBR_14400) || \
		(pdcb->BaudRate == CBR_19200) || \
		(pdcb->BaudRate == CBR_38400) || \
		(pdcb->BaudRate == CBR_56000) || \
		(pdcb->BaudRate == CBR_57600) || \
		(pdcb->BaudRate == CBR_115200) || \
		(pdcb->BaudRate == CBR_128000) || \
		(pdcb->BaudRate == CBR_256000) \
		)
	{
	} 
	else
	{
		//	波特率取值错误
		return CHECK_DCB_BAUD_ERROR;
	}

	//	检测校验位设置
	if ( ((pdcb->fParity == FALSE) && (pdcb->Parity == NOPARITY))|| \
		((pdcb->fParity == TRUE) && (pdcb->Parity == ODDPARITY)) || \
		((pdcb->fParity == TRUE) && (pdcb->Parity == EVENPARITY)) || \
		((pdcb->fParity == TRUE) && (pdcb->Parity == MARKPARITY)) || \
		((pdcb->fParity == TRUE) && (pdcb->Parity == SPACEPARITY)) \
		)
	{
	} 
	else
	{
		//	校验位设置错误
		return CHECK_DCB_PARITY_ERROR;
	}

	//	检测数据位
	if ((pdcb->ByteSize == 6) || \
		(pdcb->ByteSize == 7) || \
		(pdcb->ByteSize == 8) \
		)
	{
	} 
	else
	{
		//	校验数据位错误
		return CHECK_DCB_DATABITS_ERROR;
	}

	//	检测停止位
	if ((pdcb->StopBits == ONESTOPBIT) || \
		(pdcb->StopBits == TWOSTOPBITS) \
		)
	{
	} 
	else
	{
		//	检查停止位出错
		return CHECK_DCB_STOPBITS_ERROR;
	}

	//	通过所以检测项目
	return CHECK_DCB_OK;
}


//	更新控件上的数据到配置中
int CSerialPort::UpdateWidgetToConfig(void)
{
	//	设置的次数累积，用于判断是否有更新
	int setup_count = 0;

	//	字符串缓存
	char tmp_str[20];

	//	设置串口号字符串
	if ((this->pComboBox_ComNumber != NULL)&&(this->pComboBox_ComNumber != INVALID_HANDLE_VALUE))		//	合法的控件指针
	{
		if (this->pComboBox_ComNumber->GetCount() > 0)
		{
			int val = this->pComboBox_ComNumber->GetItemData(this->pComboBox_ComNumber->GetCurSel());	//	获取当前的数据值
			if ((MIN_SERIALPORT_NUM <= val)&&(val <= MAX_SERIALPORT_NUM))								//	得到的串口数值在合法的范围内
			{
				memset(tmp_str,0,sizeof(tmp_str));															//	清空缓存
				sprintf(tmp_str,
					"\\\\.\\COM%d",
					val);		//	格式化到缓存
				this->com_number_str.Format(L"%S",tmp_str);													//	将缓存的数据格式化到串口号参数字符串
				setup_count++;																				//	成功设置了一个参数
			}
		}
	}

	//	设置波特率
	if ((this->pComboBox_BaudRate != NULL)&&(this->pComboBox_BaudRate != INVALID_HANDLE_VALUE))		//	合法的控件指针
	{
		this->SetBaudRate(this->pComboBox_BaudRate->GetItemData(this->pComboBox_BaudRate->GetCurSel()));	//	设置波特率
		setup_count++;																						//	成功设置了一个参数
	} 

	//	设置校验位
	if ((this->pComboBox_Parity != NULL)&&(this->pComboBox_Parity != INVALID_HANDLE_VALUE))			//	合法的控件指针
	{
		this->SetParity(this->pComboBox_Parity->GetItemData(this->pComboBox_Parity->GetCurSel()));			//	设置校验位
		setup_count++;																						//	成功设置了一个参数
	}

	//	设置数据位
	if ((this->pComboBox_DataBits != NULL)&&(this->pComboBox_DataBits != INVALID_HANDLE_VALUE))		//	合法的控件指针
	{
		this->SetDataBits(this->pComboBox_DataBits->GetItemData(this->pComboBox_DataBits->GetCurSel()));	//	设置数据位
		setup_count++;																						//	成功设置了一个参数
	}

	//	检查一共设置了多少参数
	if (setup_count > 0)
	{
		//	设置了参数
		return SETUP_OK;
	} 
	else
	{
		//	没有设置参数
		return SETUP_FAIL;
	}
}


//-----------------------------------------------------------------------------------------
//	参数检查测试函数,检查串口号设置，及DCB结构
int CSerialPort::CheckSetup(void)
{
	//----------------------------------------------------------------------
	//	检查主要串口参数的配置
	//	串口号的配置
	if (CHECK_NCOM_STR_OK == this->CheckComNumberString(this->com_number_str))
	{
		//	配置信息正确
	} 
	else
	{
		return SETUP_FAIL;
	}

	//	串口配置的检查									
	if (CHECK_DCB_OK == CheckComDCB(&(this->com_config)))
	{
	} 
	else
	{
		return SETUP_FAIL;
	}

	return SETUP_OK;
}

//-----------------------------------------------------------------------------------------
int CSerialPort::SendBusy(void)					//	发送忙判断函数
{
	if (this->SendBytesNum > 0)
	{
		return SEND_BUSY;					//	发送忙
	} 
	else
	{
		return SEND_READY;					//	发送准备好
	}
}

//////////////////////////////////////////////////////////////////////////
//	该函数为线程函数，由于创建线程的时候会独立为每个线程分配内存
//	并且为了配合CreateThread函数的使用，所以这里定义为类外部的函数
DWORD WINAPI CSerialPort_ThreadFunction(LPVOID lpParameter)
{
	////	定义类指针
	CSerialPort* pcsp;

	//	检查参数合法
	if ((lpParameter == NULL)||(lpParameter == INVALID_HANDLE_VALUE))
	{
		return THREAD_CREATE_FAIL;
	}

	//	强制转换
	pcsp = (CSerialPort*)lpParameter;

	//AfxMessageBox(L"线程检查串口字符串");

#ifdef SERIALPORT_DEBUG_LOG
	pcsp->log_str += L"----线程已经启动\r\n";
#endif

	return pcsp->ComThreadProc(NULL);		//	运行线程实体
}