/** 
* (C) Copyright 2009, asiainfo 
* @version  v1.0 
* @author   chenli 
* @brief    base64编码和解码 
* 
* history: 
* <p>chenli 2009-02-17 1.0 build this moudle</p> 
* 原文出处：http://blog.csdn.net/chenxiaohua/article/details/4084602

	使用举例：
	//////////////////////////////////////////////////////////////////////////
	//	编码
	//	测试十六进制数据
	//	设置初始化数据区
	unsigned char in_buff[] = 
	{
	//0x5A,0x6B,0x00,0xF6,0x8A
	0x60,0x61,0x62,0x63,0x64
	};

	//	编码为Base64
	std::string str_tmp = "";
	bool re = CBase64::Encode(
		in_buff,			//	输入明文的首地址
		sizeof(in_buff),	//	输入明文的长度
		str_tmp				//	输出编码目标
		);

	if(re == false)			//	检查处理结果
	{
		printf("Base64编码错误！！程序即将退出！");
		_getch();
		return -1;
	}
	printf("Base64编码结果为：%s\r\n",str_tmp.data());

	//////////////////////////////////////////////////////////////////////////
	//	准备解码
	unsigned long buff_length = 100;							//	指定解码后的长度
	unsigned char* out_buff = new unsigned char[buff_length];	//	分配内存

	//	开始解码
	printf("开始解码\r\n输入的密文：%s\r\n",str_tmp.data());
	unsigned long back_length = buff_length;					//	初始值为缓冲区可用大小

	re = CBase64::Decode(
		str_tmp,				//	输入的密文
		out_buff,				//	输出明文
		&back_length			//	得到的明文长度，注意！也是输入参数，输入为缓冲区的长度
		);

	if(re == false)			//	检查处理结果
	{
		//	释放内存
		delete[] out_buff;
		printf("Base64解码错误！！程序即将退出！");
		_getch();
		return -1;
	}

	printf("解码得到的明文长度：%ld bytes\r\n得到的明文数据为:",back_length);
	for (unsigned int i=0;i<back_length;i++)				//	循环打印输出数据
	{
		int show_tmp = ((int)(out_buff[i]))&0x00FF;
		printf("%02X ",show_tmp);
	}

	//	释放内存
	delete[] out_buff;

*/  
  
#ifndef ___BASE64_H___  
#define ___BASE64_H___  
  
  
#include <string>  
  
  
class CBase64  
{  
public:  
    CBase64();  
    ~CBase64();  
  
    /********************************************************* 
    * 函数说明：将输入数据进行base64编码 
    * 参数说明：[in]pIn      需要进行编码的数据 
                [in]uInLen  输入参数的字节数 
                [out]strOut 输出的进行base64编码之后的字符串 
    * 返回值  ：true处理成功,false失败 
    * 作  者  ：ChenLi 
    * 编写时间：2009-02-17 
    **********************************************************/  
    bool static Encode(const unsigned char *pIn, unsigned long uInLen, std::string& strOut);  
  
    /********************************************************* 
    * 函数说明：将输入数据进行base64编码 
    * 参数说明：[in]pIn          需要进行编码的数据 
                [in]uInLen      输入参数的字节数 
                [out]pOut       输出的进行base64编码之后的字符串 
                [out]uOutLen    输出的进行base64编码之后的字符串长度 
    * 返回值  ：true处理成功,false失败 
    * 作  者  ：ChenLi 
    * 编写时间：2009-02-17 
    **********************************************************/  
    bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);  
      
    /********************************************************* 
    * 函数说明：将输入数据进行base64解码 
    * 参数说明：[in]strIn        需要进行解码的数据 
                [out]pOut       输出解码之后的节数数据 
                [out]uOutLen    输出的解码之后的字节数长度 
    * 返回值  ：true处理成功,false失败 
    * 作  者  ：ChenLi 
    * 编写时间：2009-02-17 
    **********************************************************/  
    bool static Decode(const std::string& strIn, unsigned char *pOut, unsigned long *uOutLen) ;  
  
    /********************************************************* 
    * 函数说明：将输入数据进行base64解码 
    * 参数说明：[in]strIn        需要进行解码的数据 
                [out]pOut       输出解码之后的节数数据 
                [out]uOutLen    输出的解码之后的字节数长度 
    * 返回值  ：true处理成功,false失败 
    * 作  者  ：ChenLi 
    * 编写时间：2009-02-17 
    **********************************************************/  
    //bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen) ;  
};  
  
#endif // ___BASE64_H___ 
