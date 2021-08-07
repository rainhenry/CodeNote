/*******************************************************************************

	程序名称：C语言版本的指针队列
	创建日期：20180508
	程序版本：REV 0.2
	设计编写：rainhenry
	
	设计说明：
		1、为了精简节约内存资源，所有队列中的成员均以指针的形式给出
		2、为了实现可以多个实例使用该代码，每个处理过程的缓存都有外部定义实体
		3、该程序专门为微控制器设计，实现类似容器的功能，方便处理各种消息
		4、采用内部统计个数计数器的方式，可以达到一个完整缓冲区的最大容量存储
		
	版本修订
		REV 0.1	20180508		创建程序
		REV 0.2	20180520		添加获取本队列配置中所能容纳最多的成员数量

*******************************************************************************/
//------------------------------------------------------------------------------
//	重定义保护
#ifndef __PLIST_C_H__
#define __PLIST_C_H__

//------------------------------------------------------------------------------
//	C++兼容处理
#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
//	相关定义
//#define P_LIST_C_DEBUG						//	如果定义了该宏，则编译用于调试的部分。正常使用时可以取消该定义

#define P_LIST_C_ERROR			-1			//	操作函数返回的错误
#define P_LIST_C_OK				1			//	操作成功

#ifndef NULL
#define NULL		0	
#endif

//	定义配置数据结构体
typedef struct tagSpListConfig
{
	void* pBuffBase;						//	缓冲区首地址
	int BuffByteSize;						//	缓冲区字节尺寸
	int offset;								//	每个用户数据类型在缓冲区内的偏移量，即每个数据类型占用的字节数目
	int in_pos;								//	入队位置，该位置指偏移量的倍数
	int out_pos;							//	出队位置，该位置指偏移量的倍数
	int count;								//	数据统计
}SpListConfig;

//------------------------------------------------------------------------------
//	导出函数
//	向队列in_pos插入一个用户数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
//	pin为指定用于数据的首地址，pconf为该指针队列的配置
int pListC_InsertOneAtInPos(void* pin, SpListConfig* pconf);

//	向队列out_pos插入一个用户数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
//	pin为指定用于数据的首地址，pconf为该指针队列的配置
int pListC_InsertOneAtOutPos(void* pin, SpListConfig* pconf);

//	获取队列中可用元素个数
int pListC_GetCount(SpListConfig* pconf);

//	获取正数第N个元素，成功该元素的首地址，失败返回NULL，当n为0的时候指最先入队的数据
void* pListC_GetDataAtN_OutPos(int n, SpListConfig* pconf);

//	获取倒数第N个元素，成功该元素的首地址，失败返回NULL，当n为0的时候指最最后入队的数据
void* pListC_GetDataAtN_InPos(int n, SpListConfig* pconf);

//	从in_pos删除一个数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
int pListC_DeleteOneAtInPos(SpListConfig* pconf);

//	从out_pos删除一个数据，操作成功返回P_LIST_C_OK，失败返回P_LIST_C_ERROR
int pListC_DeleteOneAtOutPos(SpListConfig* pconf);

//	从in_pos删除N个数据，返回删除数据的个数
int pListC_DeleteNAtInPos(int n, SpListConfig* pconf);

//	从out_pos删除N个数据，返回删除数据的个数
int pListC_DeleteNAtOutPos(int n, SpListConfig* pconf);
	
//	获取指定配置中所能容纳最多的成员数量
int pListC_GetMaxCount(SpListConfig* pconf);

//------------------------------------------------------------------------------
//	调试相关定义
#ifdef P_LIST_C_DEBUG
//	打印字符串函数
char pListC_DebugStringBuff[200];

//	按缓冲区顺序打印每个用户成员
void pListC_DebugPrintBuffByChar(SpListConfig* pconf);
#endif	//	P_LIST_C_DEBUG

//------------------------------------------------------------------------------
//	C++兼容处理
#ifdef __cplusplus
}
#endif
	
//------------------------------------------------------------------------------
#endif	//	__PLIST_C_H__

