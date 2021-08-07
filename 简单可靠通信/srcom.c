
//	程序版本	REV 0.4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plist_c.h"
#include "srcom.h"
#include "main.h"
#include "user.h"
#include "nRF2401.h"
#include "protocol.h"

//=====================================================================================
//	需要用户移植的底层函数

#if defined(SRCOM_USING_SEND) || defined(SRCOM_USING_RECV)
//	打包前回调函数，在将要开始打包一个数据包之前SRCom程序将调用该函数
void SRCom_PACK_BEFORE(void)
{	
	//nRF2401_ConfigTxMode();
	nRF2401_FastConfigTx();
}

//	发送一个字节到硬件
void SRCom_SEND_ONE_BYTE_TO_HARDWARE(unsigned char in)
{
	NRF2401_SendBuff[NRF2401_SendBuff_pos] = in;					//	存入缓冲区
	++NRF2401_SendBuff_pos;											//	统计记录的字节数目
	
	//	检查已经存入的数目，当数目足够
	if(NRF2401_SendBuff_pos >= 32)
	{
		nRF2401_TxData((unsigned char*)NRF2401_SendBuff, 32);		//	发送到硬件
		memset(NRF2401_SendBuff, 0, sizeof(NRF2401_SendBuff));		//	清空缓存
		NRF2401_SendBuff_pos = 0;									//	清空计数器
	}
}

//	打包后回调函数，在完成一个数据包打包以后，SRCom程序将调用该函数
void SRCom_PACK_AFTER(void)
{
	//	检查已经存入的数目，当有数据的时候
	if(NRF2401_SendBuff_pos > 0)
	{
		memset(NRF2401_SendBuff+NRF2401_SendBuff_pos, 0, sizeof(NRF2401_SendBuff) - NRF2401_SendBuff_pos);		//	清空未用空间
		nRF2401_TxData((unsigned char*)NRF2401_SendBuff, 32);		//	发送到硬件
		memset(NRF2401_SendBuff, 0, sizeof(NRF2401_SendBuff));		//	清空缓存
		NRF2401_SendBuff_pos = 0;									//	清空计数器
	}
	
	//nRF2401_ConfigRxMode();
	nRF2401_FastConfigRx();
}
#endif	//	SRCOM_USING_SEND || SRCOM_USING_RECV

#ifdef SRCOM_USING_SEND
//	当发送一个数据，并成功收到回应后，所收到的用户数据的回调函数
void SRCom_SEND_ACK_ONRECV(int id, int len, void* pdat)
{
	//	打印测试信息
	//uart1_printf("OK Recv ACK ID = %d, Length =  %d Bytes\r\n", id, len);
	
	Protocol_Unpack(pdat, len, FROM_HOST);
}

//	当包ID改变时，调用用户存储本次操作的包ID到非易失存储器中，以便下次用户手动载入包ID
void SRCom_ON_PACK_ID_CHANGED(int id)
{
	mFlashUserConf.pack_id = id;										//	配置包ID
	
	//	写入数据
	UpdateUserDataToFlash();	
}
#endif	//	SRCOM_USING_SEND

//	当接收方成功收到一个请求的时候，所收到的用户数据的回调函数
//	由于接收方的回应里面涉及到用户数据，所以该回应的打包发送必须由用户自行在下面函数中调用
//	本函数由SRCom_RecvProcess调用执行
//	这里的retry_flag为重试标志，当该值为0的时候表示这个请求为新的请求，当该值为1的时候，为已经处理过一次了，然后需要请求重新回复而已
//	也就是当retry_flag=1的时候，就已经执行过retry_flag=0时的该程序了
#ifdef SRCOM_USING_RECV
void SRCom_ONRECV_USER_DATA(int id, int len, void* pdat, int retry_flag)
{
	//-----------------------------------------------------------------------------------
	//	第一次收到执行
	if(retry_flag == 0)
	{
		//	回应
		SRCom_RecvPackOneSendAck(
			id,						//	包的ID
			0,						//	用户数据的长度
			0						//	用户数据的首地址
			);
		
		//	打印输出信息
		uart1_printf("Recv SRCom Recv Data OK!! ID = %d, Length = %d\r\n", id, len);

		//	执行完成
		return;
	}

	//-----------------------------------------------------------------------------------
	//	重试回复执行，此时已经执行过第一次
	if(retry_flag == 1)
	{
		//	回应
		SRCom_RecvPackOneSendAck(
			id,						//	包的ID
			0,						//	用户数据的长度
			0						//	用户数据的首地址
			);

		//	执行完成
		return;
	}
}
#endif	//	SRCOM_USING_RECV

//=====================================================================================
//	SRCom内部发送相关
#ifdef SRCOM_USING_SEND

//	发送信息缓冲区定义
char SRCom_SendInfoBuff[SRCOM_SEND_INFO_BUFF_SIZE] = {0};

//	定义发送部分的应答接收用户数据的缓冲区
char SRCom_SendAckRecvBuff[SRCOM_SEND_ACK_RECV_BUFF_SIZE] = {0};

//	当前收到回应的用户数据的长度
int SRCom_SendAckRecvUserDataLen = 0;

//	定义发送信息结构体队列配置
SpListConfig SRCom_SendInfoListConf = 
{
	SRCom_SendInfoBuff,				//	缓冲区首地址
	sizeof(SRCom_SendInfoBuff),		//	缓冲区字节尺寸
	sizeof(SRCom_SSendInfo),		//	每个用户数据类型在缓冲区内的偏移量，即每个数据类型占用的字节数目
	0,								//	入队位置，该位置指偏移量的倍数
	0,								//	出队位置，该位置指偏移量的倍数
	0,								//	数据统计
};

//	包ID变量
int SRCom_CurrentID = 0;

//	打包一个请求数据包
void SRCom_OnePackSend(
	int id,							//	包ID
	int count,						//	该ID的包的实际的发送次数
	int len,						//	用户数据长度
	void* pdat						//	用户数据首地址
	)
{
	//	定义相关变量
	int i = 0;						//	循环变量
	unsigned char* ptmp = 0;		//	临时数据指针
	unsigned char tmp = 0;			//	临时数据
	unsigned char check = 0;		//	检验值
	
	//	检查用户数据相关
	if((len <= 0) || (pdat == 0)) return;
	
	//	开始打包
	SRCom_PACK_BEFORE();
	
	//	打包数据头
	tmp = 0x5A;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = 0x12;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	打包包ID
	tmp = id&0x00FF;			SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (id>>8)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	打包实际发送次数
	tmp = count&0x00FF;				SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (count>>8)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	打包用户长度
	tmp = len&0x00FF;			SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (len>>8)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (len>>16)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (len>>24)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	循环打包用户数据
	ptmp = pdat;
	for(i=0;i<len;++i)
	{
		SRCom_SEND_ONE_BYTE_TO_HARDWARE(ptmp[i]);
		check = check ^ ptmp[i];
	}
	
	//	打包校验
	SRCom_SEND_ONE_BYTE_TO_HARDWARE(check);
	
	//	结束打包
	SRCom_PACK_AFTER();
}

//	设置包ID
void SRCom_SetPackID(int in)
{
	SRCom_CurrentID = in;
}

//	获取包ID，此方法生成的包ID为1基序
int SRCom_GenerateID(void)
{
	if(SRCom_CurrentID >= SRCOM_MAX_ID)
	{
		SRCom_CurrentID = 0;
	}
	else
	{
		++SRCom_CurrentID;
	}
	
	//	调用用户回调函数存储本次变化的包ID
	SRCom_ON_PACK_ID_CHANGED(SRCom_CurrentID);
	
	return SRCom_CurrentID;
}

//	提供给用户调用的发送数据，该函数只是将用户的请求放入发送信息队列中，并不直接操作底层硬件
void SRCom_UserSend(
	void* pdat,							//	用户数据首地址
	int len								//	用户数据长度	
	)
{
	//	定义临时信息结构体
	SRCom_SSendInfo stmp = {0};
	
	//	配置
	stmp.pUserData = pdat;				//	用户数据首地址
	stmp.data_len = len;				//	用户数据长度
	stmp.status = 0;					//	已放入请求，并未执行发送的状态
	stmp.send_count = 0;				//	此时还未进行任何发送
	stmp.pack_id = SRCom_GenerateID();	//	分配包ID
	
	//	检查队列是否已经存满
	if(pListC_GetCount(&SRCom_SendInfoListConf) >= pListC_GetMaxCount(&SRCom_SendInfoListConf))
	{
		//	获取第一个
		SRCom_SSendInfo *ptmp = 0;
		ptmp = (SRCom_SSendInfo*)pListC_GetDataAtN_OutPos(0, &SRCom_SendInfoListConf);			//	获取指针
		if(ptmp == 0) return;				//	如果失败则放弃第一次的发送处理
		
		//	检查第一个是否发送成功，当为用户已经处理完，或者为已经发送成功的时候
		if((ptmp->status == 3)||(ptmp->status == 2))
		{
			//	删除一个
			pListC_DeleteOneAtOutPos(&SRCom_SendInfoListConf);
		}	
		//	信号中断太长了
		else
		{
			return;			//	忽略新插入的数据
		}
	}
	
	//	存入队列
	pListC_InsertOneAtInPos(&stmp, &SRCom_SendInfoListConf);
}

//	由用户调用的发送处理
void SRCom_SendProcess(void)
{
	//	定义相关变量
	static int loop_count = 0;			//	循环次数统计变量
	int i = 0;							//	循环变量
	
	//-------------------------------------------------------------------------
	//	遍历队列，找到第一个未发送的
	for(i=0;i<pListC_GetCount(&SRCom_SendInfoListConf);++i)
	{
		SRCom_SSendInfo *ptmp = 0;
		ptmp = (SRCom_SSendInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_SendInfoListConf);			//	获取指针
		if(ptmp == 0) break;				//	如果失败则放弃第一次的发送处理
		
		//	如果为未发送处理
		if(ptmp->status == 0)
		{
			//	执行发送
			ptmp->send_count = 1;		//	统计发送次数，此时为第一次发送
			
			//	20180605修改：为了兼容keil5的编译BUG，这里需要将结构体内部的变量赋值给局部变量，然后才能调用打包函数，否则会导致硬件错误中断
			int id = ptmp->pack_id;
			int retry_cnt = ptmp->send_count;
			int len = ptmp->data_len;
			void* p = ptmp->pUserData;
			SRCom_OnePackSend(id,retry_cnt,len,p);
			ptmp->status = 1;			//	更新状态
			loop_count = 0;				//	清空循环次数
			break;						//	处理完成，并且只处理未成功的第一个
		}
		//	如果为已经发送的，但是并没有收到回应的
		else if(ptmp->status == 1)
		{
			++loop_count;				//	统计循环次数
			
			//	如果没有超时
			if(loop_count < SRCOM_RESEND_TIMEOUT)	
			{
				break;					//	处理完成，并且只处理未成功的第一个
			}
			//	超时了，进行重发
			else
			{
				//	统计发送次数，如果没有超出范围的时候才累加，超出范围不进行累加
				if(ptmp->send_count < (pListC_GetMaxCount(&SRCom_SendInfoListConf) * 3))	ptmp->send_count++;		

				//	20180605修改：为了兼容keil5的编译BUG，这里需要将结构体内部的变量赋值给局部变量，然后才能调用打包函数，否则会导致硬件错误中断
				int id = ptmp->pack_id;
				int retry_cnt = ptmp->send_count;
				int len = ptmp->data_len;
				void* p = ptmp->pUserData;
				SRCom_OnePackSend(id,retry_cnt,len,p);
				loop_count = 0;				//	清空超时计数器
				break;						//	处理完成，并且只处理未成功的第一个
			}
		}
		//	如果已经收到回应，但是用户还未处理
		else if(ptmp->status == 2)
		{
			//	调用用户处理
			SRCom_SEND_ACK_ONRECV(ptmp->pack_id, SRCom_SendAckRecvUserDataLen, SRCom_SendAckRecvBuff);			
			ptmp->status = 3;				//	修改当前标志
			loop_count = 0;					//	清空超时计数器			
			//break;							//	处理完成，取消该break可以提高处理效率和稳定性，这样处理完用户数据后马上就会处理发送新的包
		}
	}
}

//	设置发送部分指定ID的状态，成功返回1，失败返回0
int SRCom_SetSendStatusByID(int id, int status)
{
	//	定义相关变量
	int i = 0;								//	循环变量
	
	//	遍历所有信息结构体，找到符合ID的
	for(i=0;i<pListC_GetCount(&SRCom_SendInfoListConf);++i)
	{
		SRCom_SSendInfo *ptmp = 0;
		ptmp = (SRCom_SSendInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_SendInfoListConf);			//	获取指针
		if(ptmp == 0) return 0;				//	如果失败则放弃
		
		//	如果ID相同
		if(id == ptmp->pack_id)
		{
			ptmp->status = status;
			return 1;
		}
	}
	
	return 0;
}

//	接收回应
void SRCom_OnRecvAck(unsigned char in)
{
	//	定义相关变量
	static int cp = 0;							//	状态机变量
	int TAG = 0;								//	程序标签
	static int tmp_id = 0;						//	临时ID值
	static int tmp_check = 0;					//	临时校验值
	static int tmp_len = 0;						//	临时用户数据长度
	static int len_count = 0;					//	长度累加器
	
	//	开始状态机
	//------------------------------------------	0	检查数据头第一个
	if((TAG++) == cp)
	{
		if(in == 0x5A) 	
		{
			tmp_check = 0x00 ^ in;
			++cp;
		}
	}
	//------------------------------------------	1	检查数据头第二个
	else if((TAG++) == cp)
	{
		if(in == 0x34)
		{
			tmp_check = tmp_check ^ in;
			++cp;
		}
		else 			
		{
			tmp_check = 0;
			cp = 0;
		}
	}
	//------------------------------------------	2	处理ID的第1个字节
	else if((TAG++) == cp)
	{
		tmp_id = in & 0x00FF;
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	3	处理ID的第2个字节
	else if((TAG++) == cp)
	{
		tmp_id = tmp_id | ((in & 0x00FF) * 0x0100);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	4	处理用户数据长度第1个字节
	else if((TAG++) == cp)
	{
		tmp_len = in & 0x00FF;
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	5	处理用户数据长度第2个字节
	else if((TAG++) == cp)
	{
		tmp_len = tmp_len | ((in & 0x00FF) * 0x0100);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	6	处理用户数据长度第3个字节
	else if((TAG++) == cp)
	{
		tmp_len = tmp_len | ((in & 0x00FF) * 0x010000);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	7	处理用户数据长度第4个字节
	else if((TAG++) == cp)
	{
		tmp_len = tmp_len | ((in & 0x00FF) * 0x1000000);
		tmp_check = tmp_check ^ in;
		//	如果超出缓存大小，则复位状态机
		if(tmp_len > SRCOM_SEND_ACK_RECV_BUFF_SIZE)
		{		
			cp = 0;
			tmp_id = 0;		
			tmp_check = 0;
			tmp_len = 0;
			len_count = 0;
		}
		//	如果存在回应的用户数据，则跳转处理用户数据
		else if(tmp_len != 0) 
		{
			++cp;	
		}
		//	如果不存在用户数据，则直接处理校验部分
		else 
		{
			cp = 9;					
		}
		len_count = 0;					//	清空长度累加器
	}
	//------------------------------------------	8	循环处理用户数据
	else if((TAG++) == cp)
	{
		SRCom_SendAckRecvBuff[len_count] = in;		//	存入缓冲区
		++len_count;								//	统计已经接收的字节
		tmp_check = tmp_check ^ in;					//	计算校验
		
		//	如果已经达到指定的数目
		if(len_count >= tmp_len) ++cp;				//	跳转下一阶段
	}
	//------------------------------------------	9	检查校验
	else if((TAG++) == cp)
	{
		if(in == tmp_check)
		{
			//	设置用户数据长度
			SRCom_SendAckRecvUserDataLen = tmp_len;

			//	设置该ID的包已经送达成功
			SRCom_SetSendStatusByID(tmp_id, 2);			
		}
		
		cp = 0;
		tmp_id = 0;		
		tmp_check = 0;
		tmp_len = 0;
		len_count = 0;
	}
	//------------------------------------------	执行错误
	else
	{
		cp = 0;						//	复位状态机
		tmp_id = 0;		
		tmp_check = 0;
		tmp_len = 0;
		len_count = 0;
	}
}

//	计算当前发送部分的信号质量百分比
int SRCom_GetSendSignalQualityPer(void)
{
	//	定义相关变量
	int i = 0;						//	循环变量
	int send_count = 0;				//	实际发送的总数
	int ok_count = 0;				//	成功发送的次数

	//	遍历发送信号
	for(i=0;i<pListC_GetCount(&SRCom_SendInfoListConf);++i)
	{
		SRCom_SSendInfo *ptmp = 0;
		ptmp = (SRCom_SSendInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_SendInfoListConf);			//	获取指针
		if(ptmp == 0) return 0;				//	如果失败则放弃

		//	统计实际发送总数
		send_count = send_count + ptmp->send_count;		
		
		//	统计成功发送的次数
		if((ptmp->status == 2) || (ptmp->status == 3))
		{
			++ok_count;
		}
	}

	//	计算信号质量百分比
	return (ok_count * 100)/send_count;
}

#endif	//	SRCOM_USING_SEND

//=====================================================================================
//	SRCom内部接收相关
#ifdef SRCOM_USING_RECV

//	定义接收缓冲区
char SRCom_RecvBuff[SRCOM_RECV_BUFF_SIZE] = {0};

//	定义接收信息缓冲区
char SRCom_RecvInfoBuff[SRCOM_RECV_INFO_BUFF_SIZE] = {0};

//	定义接收部分当前收到的用户数据长度
int SRCom_RecvUserDataLen = 0;

//	定义接收信息结构体队列配置
SpListConfig SRCom_RecvInfoListConf = 
{
	SRCom_RecvInfoBuff,				//	缓冲区首地址
	sizeof(SRCom_RecvInfoBuff),		//	缓冲区字节尺寸
	sizeof(SRCom_SRecvInfo),		//	每个用户数据类型在缓冲区内的偏移量，即每个数据类型占用的字节数目
	0,								//	入队位置，该位置指偏移量的倍数
	0,								//	出队位置，该位置指偏移量的倍数
	0,								//	数据统计
};

//	接收部分的应答发送打包
void SRCom_RecvPackOneSendAck(
	int id,							//	数据包的ID
	int len,						//	用户数据的长度
	void* pdat 						//	用户数据的首地址
	)
{
	//	定义相关变量
	int i = 0;						//	循环变量
	unsigned char* ptmp = 0;		//	临时数据指针
	unsigned char tmp = 0;			//	临时数据
	unsigned char check = 0;		//	检验值
	
	//	开始打包
	SRCom_PACK_BEFORE();

	//	发送数据头
	tmp = 0x5A;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = 0x34;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	打包包ID
	tmp = id&0x00FF;			SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (id>>8)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	打包用户长度
	tmp = len&0x00FF;			SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (len>>8)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (len>>16)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	tmp = (len>>24)&0x00FF;		SRCom_SEND_ONE_BYTE_TO_HARDWARE(tmp);		check = check ^ tmp;
	
	//	循环打包用户数据
	ptmp = pdat;
	for(i=0;i<len;++i)
	{
		SRCom_SEND_ONE_BYTE_TO_HARDWARE(ptmp[i]);
		check = check ^ ptmp[i];
	}
	
	//	打包校验
	SRCom_SEND_ONE_BYTE_TO_HARDWARE(check);
	
	//	结束打包
	SRCom_PACK_AFTER();
}

//	设置接收部分指定ID的状态，成功返回1，失败返回0
int SRCom_SetRecvStatusByID(int id, int status)
{
	//	定义相关变量
	int i = 0;								//	循环变量
	
	//	遍历所有信息结构体，找到符合ID的
	for(i=0;i<pListC_GetCount(&SRCom_RecvInfoListConf);++i)
	{
		SRCom_SRecvInfo *ptmp = 0;
		ptmp = (SRCom_SRecvInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_RecvInfoListConf);			//	获取指针
		if(ptmp == 0) return 0;				//	如果失败则放弃
		
		//	如果ID相同
		if(id == ptmp->pack_id)
		{
			ptmp->status = status;
			return 1;
		}
	}
	
	return 0;
}

//	接收处理函数
void SRCom_OnRecv(unsigned char in)
{
	//	定义相关变量
	static int cp = 0;							//	状态机变量
	int TAG = 0;								//	程序标签
	static int tmp_id = 0;						//	临时ID值
	static int tmp_count = 0;					//	临时实际发送次数
	static int tmp_check = 0;					//	临时校验值
	static int tmp_len = 0;						//	临时用户数据长度
	static int len_count = 0;					//	长度累加器
	
	//	开始状态机
	//------------------------------------------	0	检查数据头第一个
	if((TAG++) == cp)
	{
		if(in == 0x5A) 	
		{
			tmp_check = 0x00 ^ in;
			++cp;
		}
	}
	//------------------------------------------	1	检查数据头第二个
	else if((TAG++) == cp)
	{
		if(in == 0x12)
		{
			tmp_check = tmp_check ^ in;
			++cp;
		}
		else 			
		{
			tmp_check = 0;
			cp = 0;
		}
	}
	//------------------------------------------	2	处理ID的第1个字节
	else if((TAG++) == cp)
	{
		tmp_id = in & 0x00FF;
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	3	处理ID的第2个字节
	else if((TAG++) == cp)
	{
		tmp_id = tmp_id | ((in & 0x00FF) * 0x0100);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	4	处理实际发送次数的第1个字节
	else if((TAG++) == cp)
	{
		tmp_count = in & 0x00FF;
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	5	处理实际发送次数的第2个字节
	else if((TAG++) == cp)
	{
		tmp_count = tmp_count | ((in & 0x00FF) * 0x0100);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	6	处理用户数据长度第1个字节
	else if((TAG++) == cp)
	{
		tmp_len = in & 0x00FF;
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	7	处理用户数据长度第2个字节
	else if((TAG++) == cp)
	{
		tmp_len = tmp_len | ((in & 0x00FF) * 0x0100);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	8	处理用户数据长度第3个字节
	else if((TAG++) == cp)
	{
		tmp_len = tmp_len | ((in & 0x00FF) * 0x010000);
		tmp_check = tmp_check ^ in;
		++cp;
	}
	//------------------------------------------	9	处理用户数据长度第4个字节
	else if((TAG++) == cp)
	{
		tmp_len = tmp_len | ((in & 0x00FF) * 0x1000000);
		tmp_check = tmp_check ^ in;
		//	如果超出缓存大小，则复位状态机
		if(tmp_len > SRCOM_RECV_BUFF_SIZE)
		{		
			cp = 0;
			tmp_id = 0;		
			tmp_check = 0;
			tmp_len = 0;
			len_count = 0;
			tmp_count = 0;
		}
		//	如果存在回应的用户数据，则跳转处理用户数据
		else if(tmp_len != 0) 
		{
			++cp;	
		}
		//	如果不存在用户数据，则直接处理校验部分
		else 
		{
			cp = 11;					
		}
		len_count = 0;					//	清空长度累加器
	}
	//------------------------------------------	10	循环处理用户数据
	else if((TAG++) == cp)
	{
		SRCom_RecvBuff[len_count] = in;				//	存入缓冲区
		++len_count;								//	统计已经接收的字节
		tmp_check = tmp_check ^ in;					//	计算校验
		
		//	如果已经达到指定的数目
		if(len_count >= tmp_len) ++cp;				//	跳转下一阶段
	}
	//------------------------------------------	11	检查校验
	else if((TAG++) == cp)
	{
		if(in == tmp_check)
		{
			//	定义临时插入数据
			SRCom_SRecvInfo stmp = {0};
			stmp.status = 0;
			stmp.send_count = tmp_count;
			stmp.pack_id = tmp_id;

			//	设置用户数据长度
			SRCom_RecvUserDataLen = tmp_len;

			//	检查队列是否已经存满
			if(pListC_GetCount(&SRCom_RecvInfoListConf) >= pListC_GetMaxCount(&SRCom_RecvInfoListConf))
			{
				//	删除一个
				pListC_DeleteOneAtOutPos(&SRCom_RecvInfoListConf);
			}
	
			//	插入到队列
			pListC_InsertOneAtInPos(&stmp, &SRCom_RecvInfoListConf);
		}
		
		cp = 0;
		tmp_id = 0;		
		tmp_check = 0;
		tmp_len = 0;
		len_count = 0;
		tmp_count = 0;
	}
	//------------------------------------------	执行错误
	else
	{
		cp = 0;						//	复位状态机
		tmp_id = 0;		
		tmp_check = 0;
		tmp_len = 0;
		len_count = 0;
		tmp_count = 0;
	}
}

//	接收部分的检查是否已经处理该包，已经处理返回1，未处理返回0，错误返回-1
int SRCom_RecvCheckAlreadyUserProcess(int id)
{
	//	定义相关变量
	int i = 0;						//	循环变量

	//	遍历所有接收信息队列
	for(i=0;i<pListC_GetCount(&SRCom_RecvInfoListConf);++i)
	{
		//	获取当前信息
		SRCom_SRecvInfo* ptmp = 0;
		ptmp = (SRCom_SRecvInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_RecvInfoListConf);
		if(ptmp == 0)	return -1;

		//	当有数据包已经处理过，或正在处理的时候，并且包的ID相同
		if((ptmp->status != 0)&&(ptmp->pack_id == id))
		{
			return 1;			//	已经处理过该包
		}
	}

	return 0;			//	未处理该包
}

//	接收处理
void SRCom_RecvProcess(void)
{
	//	定义相关变量
	int i = 0;						//	循环变量

	//	遍历所有接收信息队列
	for(i=0;i<pListC_GetCount(&SRCom_RecvInfoListConf);++i)
	{
		//	获取当前信息
		SRCom_SRecvInfo* ptmp = 0;
		ptmp = (SRCom_SRecvInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_RecvInfoListConf);
		if(ptmp == 0)	break;

		//	当有用户未处理的包
		if(ptmp->status == 0)
		{
			//	检查处理状态
			int re = SRCom_RecvCheckAlreadyUserProcess(ptmp->pack_id);
			if(re == -1) break;			//	错误

			//	执行用户处理
			SRCom_ONRECV_USER_DATA(
				ptmp->pack_id,					//	包的ID
				SRCom_RecvUserDataLen,			//	收到的数据长度
				SRCom_RecvBuff,					//	收到的数据缓冲区首地址
				re 								//	重试标志
				);

			//	改变状态
			ptmp->status = 2;		//	由于回应的发送程序在用户回调函数中执行，所以实际的状态1是不可见的

			//	处理完成
			break;
		}
	}
}

//	统计接收部分的信号质量百分比
int SRCom_GetRecvSignalQualityPer(void)
{
	//	定义相关变量
	int i = 0;						//	循环变量
	int send_count = 0;				//	实际发送的总数
	int total = 0;					//	收到的总数

	//	遍历发送信号
	total = pListC_GetCount(&SRCom_RecvInfoListConf);
	for(i=0;i<total;++i)
	{
		SRCom_SRecvInfo *ptmp = 0;
		ptmp = (SRCom_SRecvInfo*)pListC_GetDataAtN_OutPos(i, &SRCom_RecvInfoListConf);			//	获取指针
		if(ptmp == 0) return 0;				//	如果失败则放弃

		//	统计实际发送总数
		send_count = send_count + ptmp->send_count;	
	}

	//	计算信号质量百分比
	return (total * 100)/send_count;
}


#endif	//	SRCOM_USING_RECV

//=====================================================================================
//	SRCom的收发公共部分
#if defined(SRCOM_USING_SEND) || defined(SRCOM_USING_RECV)
//	接收处理
void SRCom_HARDWARE_ONRECV_ONE_BYTE(unsigned char in)
{
#ifdef SRCOM_USING_SEND
	SRCom_OnRecvAck(in);			//	执行发送处理的，回应处理
#endif	//	SRCOM_USING_SEND

#ifdef SRCOM_USING_RECV
	SRCom_OnRecv(in);				//	执行接收处理
#endif //	SRCOM_USING_RECV

}
#endif	//	SRCOM_USING_SEND || SRCOM_USING_RECV

