
//	程序版本 REV 0.2

//	包含头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plist_c.h"

//	调试相关函数，这里需要用户修改
#ifdef P_LIST_C_DEBUG
//	打印信息函数，这里为了兼容大多数编译器以及微处理器，采用字符串的方式，程序内部采用sprintf进行格式化到缓冲区内部
void pListC_DebugOutString(char* pstr)
{
	printf("%s",pstr);
}

//	按缓冲区顺序打印每个用户成员
void pListC_DebugPrintBuffByChar(SpListConfig* pconf)
{
	//	定义相关变量
	int line_count = 0;					//	行计数器
	int i = 0;							//	循环变量
	int tmp = 0;						//	临时数据变量
	
	//	循环打印
	for(i=0;i<(pconf->BuffByteSize);++i)
	{
		//	打印数据
		tmp = (*(((char*)(pconf->pBuffBase)) + i)) & 0x00FF;
		memset(pListC_DebugStringBuff, 0, sizeof(pListC_DebugStringBuff));
		sprintf(pListC_DebugStringBuff, "%02X ", tmp);
		pListC_DebugOutString(pListC_DebugStringBuff);
		
		//	统计
		++line_count;
		
		//	检查换行
		if(line_count >= 16)
		{
			pListC_DebugOutString("\r\n");
		}
	}	
	
	tmp = pconf->count;
	memset(pListC_DebugStringBuff, 0, sizeof(pListC_DebugStringBuff));
	sprintf(pListC_DebugStringBuff, "  count =  %d\r\n", tmp);
	pListC_DebugOutString(pListC_DebugStringBuff);
}
#endif	//	P_LIST_C_DEBUG

//	向队列in_pos插入一个用户数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
//	pin为指定用于数据的首地址，pconf为该指针队列的配置
int pListC_InsertOneAtInPos(void* pin, SpListConfig* pconf)
{
	//	声明变量
	int next_pos = 0;		//	计算下一个位置
	
	//	如果缓冲区满了，则插入失败
	if(pListC_GetCount(pconf) >= (pconf->BuffByteSize / pconf->offset)) return P_LIST_C_ERROR;
	
	//	复制数据
	memcpy(
		((char*)(pconf->pBuffBase)) + ((pconf->in_pos) * (pconf->offset)),
		pin,
		pconf->offset
		);	
	
	//	超过缓存
	if(((pconf->in_pos + 1)*(pconf->offset)) > (pconf->BuffByteSize - pconf->offset))
	{
		next_pos = 0;
	}
	//	没超过
	else
	{
		next_pos = pconf->in_pos + 1;
	}	
	
	//	应用配置
	pconf->in_pos = next_pos;
	
	//	统计数量
	if(pconf->count < (pconf->BuffByteSize / pconf->offset)) pconf->count = pconf->count + 1;
	
	//	返回成功
	return P_LIST_C_OK;
}

//	向队列out_pos插入一个用户数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
//	pin为指定用于数据的首地址，pconf为该指针队列的配置
int pListC_InsertOneAtOutPos(void* pin, SpListConfig* pconf)
{
	//	声明变量
	int next_pos = 0;		//	计算下一个位置
	
	//	如果缓冲区满了，则插入失败
	if(pListC_GetCount(pconf) >= (pconf->BuffByteSize / pconf->offset)) return P_LIST_C_ERROR;
	
	//	获取本次插入的位置，超过范围
	if((pconf->out_pos) <= 0)
	{
		next_pos = ((pconf->BuffByteSize)/(pconf->offset)) -1;
	}
	//	没超过
	else
	{
		next_pos = pconf->out_pos - 1;
	}	
	
	//	复制数据
	memcpy(
		((char*)(pconf->pBuffBase)) + (next_pos * (pconf->offset)),
		pin,
		pconf->offset
		);	
	
	//	应用配置
	pconf->out_pos = next_pos;
	
	//	统计数量
	if(pconf->count < (pconf->BuffByteSize / pconf->offset)) pconf->count = pconf->count + 1;
	
	//	返回成功
	return P_LIST_C_OK;
}

//	获取队列中可用元素个数
int pListC_GetCount(SpListConfig* pconf)
{
	//	返回统计的数值
	return pconf->count;
}

//	获取正数第N个元素，成功该元素的首地址，失败返回NULL，当n为0的时候指最先入队的数据
void* pListC_GetDataAtN_OutPos(int n, SpListConfig* pconf)
{
	//	定义变量
	int taget_pos = 0;					//	目标位置
	
	//	检查获取位置
	if((n >= pconf->count) || (n < 0)) return NULL;
	
	//	计算目标位置
	if((pconf->out_pos + n) >= (pconf->BuffByteSize / pconf->offset))	taget_pos = (pconf->out_pos + n) - (pconf->BuffByteSize / pconf->offset);
	else																taget_pos = pconf->out_pos + n;
	
	//	返回地址
	return ((char*)(pconf->pBuffBase)) + (taget_pos * (pconf->offset));
}

//	获取倒数第N个元素，成功该元素的首地址，失败返回NULL，当n为0的时候指最最后入队的数据
void* pListC_GetDataAtN_InPos(int n, SpListConfig* pconf)
{
	return pListC_GetDataAtN_OutPos((pconf->BuffByteSize / pconf->offset) - n - 1 ,pconf);
}

//	从in_pos删除一个数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
int pListC_DeleteOneAtInPos(SpListConfig* pconf)
{
	//	检查是否有元素
	if(pListC_GetCount(pconf) <= 0) return P_LIST_C_ERROR;
	
	//	执行删除
	if(pconf->in_pos <= 0) 	pconf->in_pos = (pconf->BuffByteSize / pconf->offset) - 1;
	else					pconf->in_pos = pconf->in_pos - 1;
	
	//	统计
	if(pconf->count > 0) 	pconf->count = pconf->count - 1;
	else					pconf->count = 0;
	
	//	返回成功
	return P_LIST_C_OK;
}

//	从out_pos删除一个数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
int pListC_DeleteOneAtOutPos(SpListConfig* pconf)
{
	//	检查是否有元素
	if(pListC_GetCount(pconf) <= 0) return P_LIST_C_ERROR;
	
	//	执行删除
	if(pconf->out_pos >= ((pconf->BuffByteSize / pconf->offset) - 1)) 	pconf->out_pos =  0;
	else																pconf->out_pos = pconf->out_pos + 1;
	
	//	统计
	if(pconf->count > 0) 	pconf->count = pconf->count - 1;
	else					pconf->count = 0;
	
	//	返回成功
	return P_LIST_C_OK;
}

//	从in_pos删除N个数据，返回删除数据的个数
int pListC_DeleteNAtInPos(int n, SpListConfig* pconf)
{
	int i = 0;			//	定义循环变量
	int count = 0;		//	统计成功计数器
	
	for(i=0;i<n;++i)
	{
		if(pListC_DeleteOneAtInPos(pconf) == P_LIST_C_OK) count = count + 1;
		else return count;
	}
	
	return count;
}

//	从out_pos删除N个数据，返回删除数据的个数
int pListC_DeleteNAtOutPos(int n, SpListConfig* pconf)
{
	int i = 0;			//	定义循环变量
	int count = 0;		//	统计成功计数器
	
	for(i=0;i<n;++i)
	{
		if(pListC_DeleteOneAtOutPos(pconf) == P_LIST_C_OK) count = count + 1;
		else return count;
	}
	
	return count;
}

//	获取指定配置中所能容纳最多的成员数量
int pListC_GetMaxCount(SpListConfig* pconf)
{
	return pconf->BuffByteSize / pconf->offset;
}

