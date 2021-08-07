
/*---------------------------------------------------------------------------
2016.07.15
	����ͨ��Դ�ļ�

��������		������
���̴�������	2016��7��15��
�����������	2016��7��24��
��ƣ�		rainhenry
���룺		rainhenry
����/У�ԣ�	rainhenry
���Ի�����	VS2010
����ƽ̨��	Windows XP SP3 
����汾��	REV 2.0

˵����	��������Ա��κ���ʹ�ú�ת�أ���ת��/ʹ�ñ�����ʱ���뱣����ע�͡�
����������ο���ѧϰ������������ҵ��;

----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "com.h"

//-----------------------------------------------------------------------------------------------------------------------------
//	CSerialPort����غ���
//	���캯��
CSerialPort::CSerialPort()
{
	//	��ʼ�����в���
	this->ThreadControlCode = THREAD_STOP;									//	�ر��߳�
	memset((void*)(&(this->com_config)),0,sizeof(this->com_config));		//	�����ýṹ�����
	this->com_handle = NULL;												//	���ھ��Ϊ��
	this->hThreadHandle = NULL;												//	�߳̾��Ϊ��
	this->com_number_str = "";												//	�����ַ���Ϊ��

	this->pComboBox_BaudRate = NULL;										//	�����ʿؼ�
	this->pComboBox_ComNumber = NULL;										//	���ںſؼ�
	this->pComboBox_DataBits = NULL;										//	����λ
	this->pComboBox_Parity = NULL;											//	����λ
	this->pComboBox_StopBits = NULL;										//	ֹͣλ

	this->pRecvData_Buffer = NULL;											//	���ջ������׵�ַ
	this->pSendData_Buffer = NULL;											//	���ͻ������׵�ַ

	this->ThreadControlCode = NULL;											//	��մ������
	this->RecvBytesNum = 0;													//	��ս����ֽ���
	this->SendBytesNum = 0;													//	��շ����ֽ���

	this->log_str = L"";													//	�����־
}

//	��������
CSerialPort::~CSerialPort()
{
	//	�����ͷź���
	this->Release();
}

//	��ʼ��
int CSerialPort::Initiation(
	void (*pFunc)(int nbytes,unsigned char* dat),				//	���յ����ݵĴ�����ָ��
	int recv_buff_size,											//	���ջ������ߴ�
	int send_buff_size											//	���ͻ������ߴ�
	)
{
	//----------------------------------------------------------------------
	//	����ָ���Ŀؼ�ָ���ʼ���ؼ�����
	//	���ںſؼ�
	if ((this->pComboBox_ComNumber != NULL)&&(this->pComboBox_ComNumber != INVALID_HANDLE_VALUE))		//	���ؼ�ָ���Ƿ�Ϸ�
	{
		//	��⴮���б��Ƿ���
		if (this->pComboBox_ComNumber->GetCount() == 0)							//	û������
		{
			this->pComboBox_ComNumber->ResetContent();							//	�����Ͽ�����
			this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
			this->pComboBox_ComNumber->AddString(L"���������˿ڡ���");			//	���һ������
			this->pComboBox_ComNumber->SetCurSel(0);							//	ѡ���0��
			this->pComboBox_ComNumber->EnableWindow(FALSE);						//	�øÿؼ�������Ч
		} 
		else																	//	�Ѿ��д�����
		{
		}
	}

	//	�����ʿؼ�
	if ((this->pComboBox_BaudRate != NULL)&&(this->pComboBox_BaudRate != INVALID_HANDLE_VALUE))		//	���ؼ�ָ���Ƿ�Ϸ�
	{
		this->pComboBox_BaudRate->ResetContent();							//	�����Ͽ�����
		this->pComboBox_BaudRate->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
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
		this->pComboBox_BaudRate->EnableWindow(TRUE);						//	�øÿؼ�������Ч
	}

	//	У��λ�ؼ���ʼ��
	if ((this->pComboBox_Parity != NULL)&&(this->pComboBox_Parity != INVALID_HANDLE_VALUE))		//	���ؼ�ָ���Ƿ�Ϸ�
	{
		this->pComboBox_Parity->ResetContent();							//	�����Ͽ�����
		this->pComboBox_Parity->ModifyStyle(CBS_SORT,0);				//	����Ϊ��������ʽ
		this->pComboBox_Parity->AddString(L"��У��");          this->pComboBox_Parity->SetItemData(0,(unsigned long)NOPARITY);
		this->pComboBox_Parity->AddString(L"��У��");          this->pComboBox_Parity->SetItemData(1,(unsigned long)ODDPARITY);
		this->pComboBox_Parity->AddString(L"żУ��");          this->pComboBox_Parity->SetItemData(2,(unsigned long)EVENPARITY);
		this->pComboBox_Parity->AddString(L"��־У��");        this->pComboBox_Parity->SetItemData(3,(unsigned long)MARKPARITY);
		this->pComboBox_Parity->AddString(L"�ո�У��");        this->pComboBox_Parity->SetItemData(4,(unsigned long)SPACEPARITY);
		this->pComboBox_Parity->SetCurSel(0);
		this->pComboBox_Parity->EnableWindow(TRUE);						//	�øÿؼ�������Ч
	}

	//	����λ�ؼ���ʼ��
	if ((this->pComboBox_DataBits != NULL)&&(this->pComboBox_DataBits != INVALID_HANDLE_VALUE))		//	���ؼ�ָ���Ƿ�Ϸ�
	{
		this->pComboBox_DataBits->ResetContent();							//	�����Ͽ�����
		this->pComboBox_DataBits->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
		this->pComboBox_DataBits->AddString(L"6");            this->pComboBox_DataBits->SetItemData(0,(unsigned long)6);
		this->pComboBox_DataBits->AddString(L"7");            this->pComboBox_DataBits->SetItemData(1,(unsigned long)7);
		this->pComboBox_DataBits->AddString(L"8");            this->pComboBox_DataBits->SetItemData(2,(unsigned long)8);
		this->pComboBox_DataBits->SetCurSel(2);
		this->pComboBox_DataBits->EnableWindow(TRUE);						//	�øÿؼ�������Ч
	}

	//	ֹͣλ�ؼ���ʼ��
	if ((this->pComboBox_StopBits != NULL)&&(this->pComboBox_StopBits != INVALID_HANDLE_VALUE))		//	���ؼ�ָ���Ƿ�Ϸ�
	{
		this->pComboBox_StopBits->ResetContent();							//	�����Ͽ�����
		this->pComboBox_StopBits->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
		this->pComboBox_StopBits->AddString(L"1");            this->pComboBox_StopBits->SetItemData(0,(unsigned long)ONESTOPBIT);
		this->pComboBox_StopBits->AddString(L"2");            this->pComboBox_StopBits->SetItemData(1,(unsigned long)TWOSTOPBITS);
		this->pComboBox_StopBits->SetCurSel(0);
		this->pComboBox_StopBits->EnableWindow(TRUE);						//	�øÿؼ�������Ч
	}
	
	//	�����պ�����ָ��
	if ((pFunc == NULL)||(pFunc == INVALID_HANDLE_VALUE))
	{
		return INIT_RECVFUNC_ERROR;
	} 
	else
	{
		this->pSerialRecv_Func = pFunc;
	}

	//	��黺�������ò���
	//	����Ϊ�˸��õĽ��պͷ������ݣ��涨����С�Ļ������ߴ�
	if ((recv_buff_size >= MIN_BUFFER_SIZE) && (send_buff_size >= MIN_BUFFER_SIZE))
	{
		//	�����ջ������Ƿ�Ϊ��
		//	�˴�����Ҫ��ֹ��û�е����ͷŵ�ʱ����һ�ε��ó�ʼ�������������ڴ�й©����
		if ((this->pRecvData_Buffer != NULL)&&(this->pRecvData_Buffer != INVALID_HANDLE_VALUE))
		{
			delete this->pRecvData_Buffer;			//	�ͷŵ��ɵ��ڴ����
		} 
		
		//	Ȼ����з�����ջ��������ڴ�
		this->pRecvData_Buffer = new unsigned char[recv_buff_size];
		if ((this->pRecvData_Buffer == NULL)||(this->pRecvData_Buffer == INVALID_HANDLE_VALUE))
		{
			//	�ڴ����ʧ��
			return INIT_MALLOC_ERROR;
		} 
		else
		{
		}

		//	��鷢�ͻ������Ƿ�Ϊ��
		//	�˴�����Ҫ��ֹ��û�е����ͷŵ�ʱ����һ�ε��ó�ʼ�������������ڴ�й©����
		if ((this->pSendData_Buffer != NULL)&&(this->pSendData_Buffer != INVALID_HANDLE_VALUE))
		{
			delete this->pSendData_Buffer;			//	�ͷŵ��ɵ��ڴ����
		} 

		//	���䷢�ͻ������ڴ�
		this->pSendData_Buffer = new unsigned char[send_buff_size];
		if ((this->pSendData_Buffer == NULL)||(this->pSendData_Buffer == INVALID_HANDLE_VALUE))
		{
			//	�����ͷŵ��ղ�����ɹ����ڴ�
			delete this->pRecvData_Buffer;

			//	Ȼ�󷵻ش������
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
		
	//	��ʱ���г�ʼ���������Ѿ���ȷ���
	return INIT_OK;
}


//-----------------------------------------------------------------------------------------
//	�ͷź���
void CSerialPort::Release(void)
{
	this->ThreadControlCode = THREAD_STOP;									//	�ر��߳�
	Sleep(500);																//	�ȴ��߳��Զ�����

	//	��λ���в���
	memset((void*)(&(this->com_config)),0,sizeof(this->com_config));		//	�����ýṹ�����

	//	���ھ��
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->hThreadHandle);
	} 
	this->com_handle = NULL;												//	���ھ��Ϊ��

	//	�߳̾��
	if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
	{
		//	�����߳�״̬��ȡ�������
		DWORD status = NULL;

		//	���Ի�ȡ�̵߳Ĺ���״̬
		if (GetExitCodeProcess(this->hThreadHandle, &status) != 0)
		{
			//	���̻߳�����
			if (status == STILL_ACTIVE)
			{
				//	ǿ�ƹر��߳�
				TerminateThread(this->hThreadHandle, status);
				//	�ͷž��
				CloseHandle(this->hThreadHandle);
			} 
			//	�߳��Ѿ�������
			else
			{
				//	�ͷž��
				CloseHandle(this->hThreadHandle);
			}
		} 
		//	��ȡ״̬ʧ��
		else
		{
			//	ǿ�ƹر��߳�
			TerminateThread(this->hThreadHandle, status);
			//	�ͷž��
			CloseHandle(this->hThreadHandle);
		}
	}
	this->hThreadHandle = NULL;												//	�߳̾��Ϊ��

	this->com_number_str = "";												//	�����ַ���Ϊ��

	this->pComboBox_BaudRate = NULL;										//	�����ʿؼ�
	this->pComboBox_ComNumber = NULL;										//	���ںſؼ�
	this->pComboBox_DataBits = NULL;										//	����λ
	this->pComboBox_Parity = NULL;											//	����λ
	this->pComboBox_StopBits = NULL;										//	ֹͣλ

	//	�ͷŽ��ջ������ڴ�
	if ((this->pRecvData_Buffer != NULL)&&(this->pRecvData_Buffer != INVALID_HANDLE_VALUE))
	{
		delete this->pRecvData_Buffer;
	}
	this->pRecvData_Buffer = NULL;											//	���ջ������׵�ַ

	//	�ͷŷ��ͻ������ڴ�
	if ((this->pSendData_Buffer != NULL)&&(this->pSendData_Buffer != INVALID_HANDLE_VALUE))
	{
		delete this->pSendData_Buffer;
	}
	this->pSendData_Buffer = NULL;											//	���ͻ������׵�ַ

	this->ThreadControlCode = NULL;											//	��մ������
	this->RecvBytesNum = 0;													//	��ս����ֽ���
	this->SendBytesNum = 0;													//	��շ����ֽ���
}

//-----------------------------------------------------------------------------------------
//	ֱ�����ô��ں�
int CSerialPort::SetComNumber(
	CString ncom_str											//	��������Ϊ���ڵĴ��ַ���������"\\\\.\\COM2"
	)
{
	//	������
	if (CHECK_NCOM_STR_OK == this->CheckComNumberString(ncom_str))
	{
		this->com_number_str = ncom_str;
		return SETUP_OK;
	} 
	else
	{
		//	����ʧ��
		return SETUP_FAIL;
	}
	
}

//	ͨ���ؼ��������ô��ںţ�֧��������ѡ����
int CSerialPort::SetComNumber(
	CComboBox* pcom												//	ָ����CComboBox�ؼ�������������ѡ�񴮿ں�
	)
{
	//	�������
	if ((pcom != NULL)&&(pcom != INVALID_HANDLE_VALUE))
	{
		//	���óɹ�
		this->pComboBox_ComNumber = pcom;
		return SETUP_OK;
	} 
	else
	{
		//	����ʧ��
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	ֱ�����ò�����
int CSerialPort::SetBaudRate(
	int baud													//	ֱ��ָ�������ʣ���CBR_115200�����Ķ��壬������WINBASE.H�ļ���
	)
{
	//	��鲨����
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
		//	���óɹ�
		this->com_config.BaudRate = baud;
		return SETUP_OK;
	} 
	else
	{
		//	������ȡֵ����
		return SETUP_FAIL;
	}
}

//	ͨ���ؼ����ò�����
int CSerialPort::SetBaudRate(
	CComboBox* pbaud											//	ָ����CComboBox�ؼ����������ò�����
	)
{
	//	������
	if ((pbaud != NULL)&&(pbaud != INVALID_HANDLE_VALUE))
	{
		//	����OK
		this->pComboBox_BaudRate = pbaud;
		return SETUP_OK;
	} 
	else
	{
		//	����ʧ��
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	����У��λ
int CSerialPort::SetParity(
	int parity													//	ֱ��ָ��У��λ����NOPARITY�����Ķ��壬������WINBASE.H�ļ���
	)
{
	//	���У��λ����
	if ((parity == NOPARITY) || \
		(parity == ODDPARITY) || \
		(parity == EVENPARITY) || \
		(parity == MARKPARITY) || \
		(parity == SPACEPARITY) \
		)
	{
		//	���óɹ�
		if (parity == NOPARITY)			//	��У��
		{
			this->com_config.fParity = FALSE;
			this->com_config.Parity = NOPARITY;
		} 
		else							//	��У��
		{
			this->com_config.fParity = TRUE;
			this->com_config.Parity = parity;
		}

		return SETUP_OK;
	} 
	else
	{
		//	У��λ���ô���
		return SETUP_FAIL;
	}
}

//	ͨ���ؼ�����У��λ
int CSerialPort::SetParity(
	CComboBox* pparity											//	ָ����CComboBox�ؼ�����������У��λ
	)
{
	//	�������
	if ((pparity != NULL)&&(pparity != INVALID_HANDLE_VALUE))
	{
		//	���óɹ�
		this->pComboBox_Parity = pparity;
		return SETUP_OK;
	} 
	else
	{
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	����ֹͣλ
int CSerialPort::SetStopBits(
	int stopbits												//	ֱ��ָ��ֹͣλ����ONESTOPBIT�����Ķ��壬������WINBASE.H�ļ���
	)
{
	//	���ֹͣλ
	if ((stopbits == ONESTOPBIT) || \
		(stopbits == TWOSTOPBITS) \
		)
	{
		//	���óɹ�
		this->com_config.StopBits = stopbits;
		return SETUP_OK;
	} 
	else
	{
		//	���ֹͣλ����
		return SETUP_FAIL;
	}
}

//	ͨ���ؼ�����ֹͣλ
int CSerialPort::SetStopBits(
	CComboBox* pstopbits										//	ָ����CComboBox�ؼ�����������ֹͣλ
	)
{
	//	������
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
//	��������λ
int CSerialPort::SetDataBits(
	int databits												//	ֱ��ָ������λ��ȡֵ��Χһ����6~8
	)
{
	//	�������λ
	if ((databits == 6) || \
		(databits == 7) || \
		(databits == 8) \
		)
	{
		//	���óɹ�
		this->com_config.ByteSize = databits;
		return SETUP_OK;
	} 
	else
	{
		//	У������λ����
		return SETUP_FAIL;
	}
}

//	ͨ���ؼ���������λ
int CSerialPort::SetDataBits(
	CComboBox* pdatabits										//	ָ����CComboBox�ؼ���������������λ
	)
{
	//	������
	if ((pdatabits != NULL)&&(pdatabits != INVALID_HANDLE_VALUE))
	{
		//	���óɹ�
		this->pComboBox_DataBits = pdatabits;
		return SETUP_OK;
	} 
	else
	{
		//	����ʧ��
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	����DCB�ṹ���������
int CSerialPort::LoadConfig(
	DCB* pdcb													//	Ҫ�������õ�DCB�ṹ��ָ��
	)
{
	//	�������
	if ((pdcb != NULL)&&(pdcb != INVALID_HANDLE_VALUE))
	{
		//	����������
		if (this->CheckComDCB(pdcb) == CHECK_DCB_OK)
		{
			//	���в������ö�����
			this->com_config = *pdcb;			//	��������
			return SETUP_OK;
		} 
		else
		{
			//	�зǷ�����
			return SETUP_FAIL;
		}
	} 
	else
	{
		//	����ʧ��
		return SETUP_FAIL;
	}
}

//-----------------------------------------------------------------------------------------
//	��������
//	������ñ�������������Сֵ���δ򿪵����ֵ
//	�ú����ķ���ֵΪ�������Ĵ�����Ŀ
//	ʹ�øú�����Ŀ����Ϊ���ÿؼ�֧���������ܣ���δ����ؼ���ʱ�򣬾Ͳ���ִ�иú�������Ҫ����
int CSerialPort::SearchSerialPort(void)
{
	//	��⵱ǰ�����Ƿ��д򿪵Ĵ���,��ֹ��Դй¶
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->com_handle);				//	�رյ�ǰ����
		this->com_handle = NULL;
	}

	//	���ںſؼ�
	if ((this->pComboBox_ComNumber != NULL)&&(this->pComboBox_ComNumber != INVALID_HANDLE_VALUE))		//	���ؼ�ָ���Ƿ�Ϸ�
	{
		//	��տؼ�״̬
		this->pComboBox_ComNumber->ResetContent();							//	�����Ͽ�����
		this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
		this->pComboBox_ComNumber->EnableWindow(FALSE);						//	�øÿؼ�������Ч

		//	������������
		int i;																//	����ѭ������
		CString open_str;													//	������ַ�������
		HANDLE tmp_handle;													//	��ʱ�򿪾��
		for (i=MIN_SERIALPORT_NUM;i<=MAX_SERIALPORT_NUM;i++)
		{
			tmp_handle = NULL;												//	��վ��
			open_str = "";													//	��ջ���
			open_str.Format(L"\\\\.\\COM%d",i);								//	���ɴ��ַ���

			//	���Դ򿪸ô���
			tmp_handle = CreateFile(
				open_str,
				GENERIC_READ|GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
				);

			//	�����Ƿ�ɹ�
			if ((tmp_handle == NULL)||(tmp_handle == INVALID_HANDLE_VALUE))			//	����ȷ�ķ��ؾ��
			{
			} 
			//	�򿪳ɹ�
			else
			{
				//	��������ַ���
				open_str = "";													//	��ջ���
				open_str.Format(L"COM%d",i);									//	�����ַ���

				//	��Ӵ����ַ������ؼ��������ô��ں�����
				this->pComboBox_ComNumber->AddString(open_str);
				this->pComboBox_ComNumber->SetItemData(this->pComboBox_ComNumber->GetCount()-1,i);

				//	�رմ���
				CloseHandle(tmp_handle);
			}
		}

		//	ͳ���Ѿ��������Ĵ���
		int count = this->pComboBox_ComNumber->GetCount();
		if (count == 0)															//	�ҵ�������Ϊ0
		{
			this->pComboBox_ComNumber->ResetContent();							//	�����Ͽ�����
			this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
			this->pComboBox_ComNumber->AddString(L"û���ҵ����õĴ��ڣ�");		//	���һ������
			this->pComboBox_ComNumber->SetCurSel(0);							//	ѡ���0��
			this->pComboBox_ComNumber->EnableWindow(FALSE);						//	�øÿؼ�������Ч

			return NULL;
		} 
		else
		{
			this->pComboBox_ComNumber->ModifyStyle(CBS_SORT,0);					//	����Ϊ��������ʽ
			this->pComboBox_ComNumber->SetCurSel(0);							//	ѡ���0��
			this->pComboBox_ComNumber->EnableWindow(TRUE);						//	�øÿؼ�������Ч

			return count;
		}
	}
	else
	{
		return INVALID_LINK_INDEX;
	}

}


//-----------------------------------------------------------------------------------------
//	�����̺߳���
DWORD CSerialPort::ComThreadProc(LPVOID lpParameter)
{
	//	Ӧ����������
	//this->UpdateWidgetToConfig();	//	����ʵ�鷢�֣����߳��д���ؼ��ᵼ���߳��쳣�˳�

	//AfxMessageBox(L"�̼߳�鴮���ַ���");

	//	�����ַ����������
	if (this->CheckComNumberString(this->com_number_str) != CHECK_NCOM_STR_OK)
	{
		return THREAD_COM_STR_ERROR;
	}

	//	���ڲ������ü��
	if (this->CheckComDCB(&(this->com_config)) != CHECK_DCB_OK)
	{
		return THREAD_COM_DCB_ERROR;
	}

	//	�ж��Ƿ����Ѿ��򿪵Ĵ���
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->com_handle);		//	�ر��Ѿ��򿪵ľ��
		this->com_handle = NULL;
	}

	//	�򿪴���
	this->com_handle = CreateFile(
		this->com_number_str,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//	�ж��Ƿ�򿪳ɹ�
	if ((this->com_handle == NULL)||(this->com_handle == INVALID_HANDLE_VALUE))
	{
		return THREAD_OPEN_FAIL;
	}

	//	�������
	DCB tmp_dcb;
	GetCommState(this->com_handle,&tmp_dcb);
	tmp_dcb.BaudRate=this->com_config.BaudRate;
	tmp_dcb.fBinary=TRUE;
	tmp_dcb.fParity=this->com_config.fParity;
	tmp_dcb.ByteSize=this->com_config.ByteSize;
	tmp_dcb.Parity=this->com_config.Parity;
	tmp_dcb.StopBits=this->com_config.StopBits;
	SetCommState(this->com_handle,&tmp_dcb);	//	���ò���
	
	//	������ѭ��
	COMSTAT ComState;							//	���崮�п�״̬�ṹ
	this->ThreadControlCode = THREAD_RUN;		//	����
	while(this->ThreadControlCode == THREAD_RUN)
	{
		//	�ж��Ƿ����δ֪����
		if (
		ClearCommError(
			this->com_handle,
			&(this->ThreadErrorCode),
			&ComState
			) != 0)
		//------------------------------------------
		//	��ʱ������ȷ
		{
			//----------------------------------------------------------
			//	���մ������
			if (ComState.cbInQue > 0)			//	���յ��ֽ�������0
			{
				if (0 != ReadFile(
					this->com_handle,			//	���ھ��
					this->pRecvData_Buffer,		//	������
					ComState.cbInQue,			//	Ҫ���յ��ֽ�
					&(this->RecvBytesNum),		//	ʵ�ʽ��յ��ֽ�
					NULL))
				{
					if (ComState.cbInQue == this->RecvBytesNum)			//	���ݽ�����ȷ
					{
						//	�����û����պ���
						this->pSerialRecv_Func(this->RecvBytesNum,this->pRecvData_Buffer);
					} 
					else													//	�����쳣
					{
						CloseHandle(this->com_handle);				//	�رմ���
						this->com_handle = NULL;
						this->ThreadControlCode = THREAD_STOP;		//	ֹͣ
						return THREAD_RECV_ERROR;					//	�������մ���
					}
				} 
				//	�����ȡ
				else
				{
					CloseHandle(this->com_handle);				//	�رմ���
					this->com_handle = NULL;
					this->ThreadControlCode = THREAD_STOP;		//	ֹͣ
					return THREAD_RECV_ERROR;					//	�������մ���
				}
			}
			//----------------------------------------------------------
			//	���ʹ������
			else
			{
				//	����ʱ��Ҫ���͵�����
				if (this->SendBytesNum > 0)
				{
					//	���ͻ�����Ϊ�գ���ʱ������������
					if (ComState.cbOutQue==0)
					{
						//AfxMessageBox(L"���Է���");
						//	���Է���
						DWORD SendBytes;			//	����ʵ�ʷ��͵��ֽ���Ŀ
						if (0 != WriteFile(
							this->com_handle,
							this->pSendData_Buffer,
							this->SendBytesNum,
							&SendBytes,
							NULL
							))
						//	���Ͳ����ɹ�
						{
							//	�����۷��͵��ֽ���Ŀ��ʵ�ʷ��͵��ֽ���Ŀ��ͬ
							if (SendBytes == this->SendBytesNum)
							{
								//	��ʱ��������ȷ��
								this->SendBytesNum = 0;						//	���Ҫ���͵��ֽ���Ŀ
							} 
							else
							{
								//	���ʹ���
								CloseHandle(this->com_handle);				//	�رմ���
								this->com_handle = NULL;
								this->ThreadControlCode = THREAD_STOP;		//	ֹͣ
								return THREAD_SEND_ERROR;					//	�������ʹ���
							}
						} 
						//	����ʧ��
						else
						{
							CloseHandle(this->com_handle);				//	�رմ���
							this->com_handle = NULL;
							this->ThreadControlCode = THREAD_STOP;		//	ֹͣ
							return THREAD_SEND_ERROR;					//	�������ʹ���
						}
					}
					//	���ͻ����������ݣ����ڲ��õ���ͬ����ʽ�����Ĵ��ڣ����Դ�ʱӦ��ΪӲ��������
					else
					{
						CloseHandle(this->com_handle);				//	�رմ���
						this->com_handle = NULL;
						this->ThreadControlCode = THREAD_STOP;		//	ֹͣ
						return THREAD_UNKNOW_ERROR;					//	����δ֪����
					}
				}
				//	û��Ҫ���͵����ݵ�ʱ�������Ϣ����ֹ�߳�ռ��CPU
				else
				{
					Sleep(10);
				}
			}
		}
		//------------------------------------------
		//	��ʱ���ִ���
		else
		{
			CloseHandle(this->com_handle);				//	�رմ���
			this->com_handle = NULL;
			this->ThreadControlCode = THREAD_STOP;		//	ֹͣ
			return THREAD_UNKNOW_ERROR;					//	����δ֪����
		}
	}

	//	�رմ���
	CloseHandle(this->com_handle);
	this->com_handle = NULL;

	//	�߳���ȷ�˳�
	return THREAD_EXIT;
}

//-----------------------------------------------------------------------------------------
//	�򿪴���
int CSerialPort::Open(void)
{
	//	Ӧ����������
	this->UpdateWidgetToConfig();

	//	�����ַ����������
	if (this->CheckComNumberString(this->com_number_str) != CHECK_NCOM_STR_OK)
	{
		return OPEN_COM_NUMBER_ERROR;
	}

	//	���ڲ������ü��
	if (this->CheckComDCB(&(this->com_config)) != CHECK_DCB_OK)
	{
		return OPEN_COM_DCB_ERROR;
	}

	//	�ж��Ƿ����Ѿ��򿪵Ĵ��� ���� �߳��Ѿ�����
	if (((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE)) || \
		((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE)))
	{
		this->Close();					//	���ùرպ����ͷ���Դ
	}

	//	�򿪴���
	this->com_handle = CreateFile(
		this->com_number_str,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//	�ж��Ƿ�򿪳ɹ�
	if ((this->com_handle == NULL)||(this->com_handle == INVALID_HANDLE_VALUE))
	{
		return OPEN_FAIL;
	}
	else
	{
		//	�رոô���
		CloseHandle(this->com_handle);
		this->com_handle = NULL;

		//	����߳��Ƿ��Ѿ�����
		if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))		//	��ʱ��ʾ�Ѿ����̹߳�����
		{
			this->ThreadControlCode = THREAD_STOP;					//	���͹ر�����
			Sleep(200);												//	�ȴ���Ȼ�ر�

			//	�߳̾�����ͷŴ���
			if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
			{
				//	�����߳�״̬��ȡ�������
				DWORD status = NULL;

				//	���Ի�ȡ�̵߳Ĺ���״̬
				if (GetExitCodeProcess(this->hThreadHandle, &status) != 0)
				{
					//	���̻߳�����
					if (status == STILL_ACTIVE)
					{
						//	ǿ�ƹر��߳�
						TerminateThread(this->hThreadHandle, status);
						//	�ͷž��
						CloseHandle(this->hThreadHandle);
					} 
					//	�߳��Ѿ�������
					else
					{
						//	�ͷž��
						CloseHandle(this->hThreadHandle);
					}
				} 
				//	��ȡ״̬ʧ��
				else
				{
					//	ǿ�ƹر��߳�
					TerminateThread(this->hThreadHandle, status);
					//	�ͷž��
					CloseHandle(this->hThreadHandle);
				}
			}
			this->hThreadHandle = NULL;												//	�߳̾��Ϊ��
		}

#ifdef SERIALPORT_DEBUG_LOG
		this->log_str += L"----���������߳�\r\n";
#endif

		//	�����߳�
		this->ThreadControlCode = THREAD_RUN;		//	�趨Ϊ����
		DWORD ThreadID;						//	�����߳�ID
		this->hThreadHandle = CreateThread(
			NULL,
			0,
			CSerialPort_ThreadFunction,
			this,                   //���ݸ��̴߳������Ĳ���
			0,
			&ThreadID
			);

		//	�򿪳ɹ�
		return OPEN_OK;
	}
}

//-----------------------------------------------------------------------------------------
//	�رմ���
void CSerialPort::Close(void)
{
	this->ThreadControlCode = THREAD_STOP;					//	���͹ر�����
	Sleep(200);												//	�ȴ���Ȼ�ر�

	//	�߳̾�����ͷŴ���
	if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
	{
		//	�����߳�״̬��ȡ�������
		DWORD status = NULL;

		//	���Ի�ȡ�̵߳Ĺ���״̬
		if (GetExitCodeProcess(this->hThreadHandle, &status) != 0)
		{
			//	���̻߳�����
			if (status == STILL_ACTIVE)
			{
				//	ǿ�ƹر��߳�
				TerminateThread(this->hThreadHandle, status);
				//	�ͷž��
				CloseHandle(this->hThreadHandle);
			} 
			//	�߳��Ѿ�������
			else
			{
				//	�ͷž��
				CloseHandle(this->hThreadHandle);
			}
		} 
		//	��ȡ״̬ʧ��
		else
		{
			//	ǿ�ƹر��߳�
			TerminateThread(this->hThreadHandle, status);
			//	�ͷž��
			CloseHandle(this->hThreadHandle);
		}
	}
	this->hThreadHandle = NULL;												//	�߳̾��Ϊ��

	//	�ж��Ƿ����Ѿ��򿪵Ĵ���
	if ((this->com_handle != NULL)&&(this->com_handle != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->com_handle);		//	�ر��Ѿ��򿪵ľ��
	}
	this->com_handle = NULL;
}

//-----------------------------------------------------------------------------------------
//	��������
int CSerialPort::Send(
	int length,													//	Ҫ���͵����ݵĳ���
	unsigned char* dat											//	�������ڵ��׵�ַ
	)
{
#ifdef SERIALPORT_DEBUG_LOG
	this->log_str += L"----���뷢��\r\n";
#endif

	//	���߳��ڹ���
	if ((this->hThreadHandle != NULL)&&(this->hThreadHandle != INVALID_HANDLE_VALUE))
	{
		//	����ǰ���Ͳ�æ
		if (this->SendBytesNum == 0)
		{
			memcpy(this->pSendData_Buffer,dat,length);			//	����Ҫ���͵����ݵ����ͻ���
			this->SendBytesNum = length;						//	����Ҫ���͵��ֽ���Ŀ

			return SEND_OK;										//	���ͳɹ�
		} 
		//	�����ͻ�δ���
		else
		{
			return SEND_BUSY;						//	����æ
		}
	}
	//	�����̻߳�δ����
	else
	{
		return SEND_COM_HANDLE_ERROR;
	}
}


//-----------------------------------------------------------------------------------------
//	���������Ӻ���
//	��鴮�ں��ַ����ĺϷ���
int CSerialPort::CheckComNumberString(CString str)
{

	//	wince�´��ں����⴦��
	return CHECK_NCOM_STR_OK;

	////	�����ַ����м����
	//char tmp_str[100];

	//memset(tmp_str,0,sizeof(tmp_str));							//	�����ʱ�ַ�������
	//str.MakeUpper();											//	������Сд��ĸת��Ϊ��д
	//sprintf(tmp_str,"%S",str);									//	��ʽ�����ַ�����

	////	��������ѡ��ͬ�ļ�鷽ʽ
	//if (tmp_str[0] == 'C')
	//{
	//	//	����ǰ����ַ���ΪCOM�Ļ�
	//	if ( (tmp_str[1] == 'O') && (tmp_str[2] == 'M') && (strlen(tmp_str) > 2))
	//	{
	//		//	ѭ����������Ƿ�������
	//		size_t i;
	//		for (i=3;i<strlen(tmp_str);i++)
	//		{
	//			if (('0' <= tmp_str[i])&&(tmp_str[i] <= '9'))
	//			{
	//			}
	//			else
	//			{
	//				//	���ַǷ��ַ�
	//				return CHECK_NCOM_STR_FAIL;
	//			}
	//		}

	//		//	��ʱΪ��ȷ��ʽ
	//		return CHECK_NCOM_STR_OK;
	//	}
	//	else
	//	{
	//		//	����
	//		return CHECK_NCOM_STR_FAIL;
	//	}
	//} 
	//else if(tmp_str[0] == '\\')
	//{
	//	//	����ǰ����ַ���Ϊ\\\\.\\COM�Ļ�
	//	if ( (tmp_str[1] == '\\') && (tmp_str[2] == '.') && (tmp_str[3] == '\\') && (tmp_str[4] == 'C') && (tmp_str[5] == 'O') && (tmp_str[6] == 'M') && (strlen(tmp_str) > 6))
	//	{
	//		//	ѭ����������Ƿ�������
	//		size_t i;
	//		for (i=7;i<strlen(tmp_str);i++)
	//		{
	//			if (('0' <= tmp_str[i])&&(tmp_str[i] <= '9'))
	//			{
	//			}
	//			else
	//			{
	//				//	���ַǷ��ַ�
	//				return CHECK_NCOM_STR_FAIL;
	//			}
	//		}

	//		//	��ʱΪ��ȷ��ʽ
	//		return CHECK_NCOM_STR_OK;
	//	}
	//	else
	//	{
	//		//	����
	//		return CHECK_NCOM_STR_FAIL;
	//	}
	//}
	//else
	//{
	//	//	�Ƿ���ʽ
	//	return CHECK_NCOM_STR_FAIL;
	//}

	////	���ؽ��
	//return CHECK_NCOM_STR_FAIL;
}


//	��鴮������
int CSerialPort::CheckComDCB(DCB* pdcb)
{
	//	��������ַ�ĺϷ���
	if ((pdcb == NULL)||(pdcb == INVALID_HANDLE_VALUE))
	{
		//	��ָ��
		return CHECK_DCB_NULL_POINT;
	} 
	else
	{
	}

	//	��鲨����
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
		//	������ȡֵ����
		return CHECK_DCB_BAUD_ERROR;
	}

	//	���У��λ����
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
		//	У��λ���ô���
		return CHECK_DCB_PARITY_ERROR;
	}

	//	�������λ
	if ((pdcb->ByteSize == 6) || \
		(pdcb->ByteSize == 7) || \
		(pdcb->ByteSize == 8) \
		)
	{
	} 
	else
	{
		//	У������λ����
		return CHECK_DCB_DATABITS_ERROR;
	}

	//	���ֹͣλ
	if ((pdcb->StopBits == ONESTOPBIT) || \
		(pdcb->StopBits == TWOSTOPBITS) \
		)
	{
	} 
	else
	{
		//	���ֹͣλ����
		return CHECK_DCB_STOPBITS_ERROR;
	}

	//	ͨ�����Լ����Ŀ
	return CHECK_DCB_OK;
}


//	���¿ؼ��ϵ����ݵ�������
int CSerialPort::UpdateWidgetToConfig(void)
{
	//	���õĴ����ۻ��������ж��Ƿ��и���
	int setup_count = 0;

	//	�ַ�������
	char tmp_str[20];

	//	���ô��ں��ַ���
	if ((this->pComboBox_ComNumber != NULL)&&(this->pComboBox_ComNumber != INVALID_HANDLE_VALUE))		//	�Ϸ��Ŀؼ�ָ��
	{
		if (this->pComboBox_ComNumber->GetCount() > 0)
		{
			int val = this->pComboBox_ComNumber->GetItemData(this->pComboBox_ComNumber->GetCurSel());	//	��ȡ��ǰ������ֵ
			if ((MIN_SERIALPORT_NUM <= val)&&(val <= MAX_SERIALPORT_NUM))								//	�õ��Ĵ�����ֵ�ںϷ��ķ�Χ��
			{
				memset(tmp_str,0,sizeof(tmp_str));															//	��ջ���
				sprintf(tmp_str,
					"\\\\.\\COM%d",
					val);		//	��ʽ��������
				this->com_number_str.Format(L"%S",tmp_str);													//	����������ݸ�ʽ�������ںŲ����ַ���
				setup_count++;																				//	�ɹ�������һ������
			}
		}
	}

	//	���ò�����
	if ((this->pComboBox_BaudRate != NULL)&&(this->pComboBox_BaudRate != INVALID_HANDLE_VALUE))		//	�Ϸ��Ŀؼ�ָ��
	{
		this->SetBaudRate(this->pComboBox_BaudRate->GetItemData(this->pComboBox_BaudRate->GetCurSel()));	//	���ò�����
		setup_count++;																						//	�ɹ�������һ������
	} 

	//	����У��λ
	if ((this->pComboBox_Parity != NULL)&&(this->pComboBox_Parity != INVALID_HANDLE_VALUE))			//	�Ϸ��Ŀؼ�ָ��
	{
		this->SetParity(this->pComboBox_Parity->GetItemData(this->pComboBox_Parity->GetCurSel()));			//	����У��λ
		setup_count++;																						//	�ɹ�������һ������
	}

	//	��������λ
	if ((this->pComboBox_DataBits != NULL)&&(this->pComboBox_DataBits != INVALID_HANDLE_VALUE))		//	�Ϸ��Ŀؼ�ָ��
	{
		this->SetDataBits(this->pComboBox_DataBits->GetItemData(this->pComboBox_DataBits->GetCurSel()));	//	��������λ
		setup_count++;																						//	�ɹ�������һ������
	}

	//	���һ�������˶��ٲ���
	if (setup_count > 0)
	{
		//	�����˲���
		return SETUP_OK;
	} 
	else
	{
		//	û�����ò���
		return SETUP_FAIL;
	}
}


//-----------------------------------------------------------------------------------------
//	���������Ժ���,��鴮�ں����ã���DCB�ṹ
int CSerialPort::CheckSetup(void)
{
	//----------------------------------------------------------------------
	//	�����Ҫ���ڲ���������
	//	���ںŵ�����
	if (CHECK_NCOM_STR_OK == this->CheckComNumberString(this->com_number_str))
	{
		//	������Ϣ��ȷ
	} 
	else
	{
		return SETUP_FAIL;
	}

	//	�������õļ��									
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
int CSerialPort::SendBusy(void)					//	����æ�жϺ���
{
	if (this->SendBytesNum > 0)
	{
		return SEND_BUSY;					//	����æ
	} 
	else
	{
		return SEND_READY;					//	����׼����
	}
}

//////////////////////////////////////////////////////////////////////////
//	�ú���Ϊ�̺߳��������ڴ����̵߳�ʱ������Ϊÿ���̷߳����ڴ�
//	����Ϊ�����CreateThread������ʹ�ã��������ﶨ��Ϊ���ⲿ�ĺ���
DWORD WINAPI CSerialPort_ThreadFunction(LPVOID lpParameter)
{
	////	������ָ��
	CSerialPort* pcsp;

	//	�������Ϸ�
	if ((lpParameter == NULL)||(lpParameter == INVALID_HANDLE_VALUE))
	{
		return THREAD_CREATE_FAIL;
	}

	//	ǿ��ת��
	pcsp = (CSerialPort*)lpParameter;

	//AfxMessageBox(L"�̼߳�鴮���ַ���");

#ifdef SERIALPORT_DEBUG_LOG
	pcsp->log_str += L"----�߳��Ѿ�����\r\n";
#endif

	return pcsp->ComThreadProc(NULL);		//	�����߳�ʵ��
}