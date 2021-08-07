/** 
* (C) Copyright 2009, asiainfo 
* @version  v1.0 
* @author   chenli 
* @brief    base64����ͽ��� 
* 
* history: 
* <p>chenli 2009-02-17 1.0 build this moudle</p> 
* ԭ�ĳ�����http://blog.csdn.net/chenxiaohua/article/details/4084602

	ʹ�þ�����
	//////////////////////////////////////////////////////////////////////////
	//	����
	//	����ʮ����������
	//	���ó�ʼ��������
	unsigned char in_buff[] = 
	{
	//0x5A,0x6B,0x00,0xF6,0x8A
	0x60,0x61,0x62,0x63,0x64
	};

	//	����ΪBase64
	std::string str_tmp = "";
	bool re = CBase64::Encode(
		in_buff,			//	�������ĵ��׵�ַ
		sizeof(in_buff),	//	�������ĵĳ���
		str_tmp				//	�������Ŀ��
		);

	if(re == false)			//	��鴦����
	{
		printf("Base64������󣡣����򼴽��˳���");
		_getch();
		return -1;
	}
	printf("Base64������Ϊ��%s\r\n",str_tmp.data());

	//////////////////////////////////////////////////////////////////////////
	//	׼������
	unsigned long buff_length = 100;							//	ָ�������ĳ���
	unsigned char* out_buff = new unsigned char[buff_length];	//	�����ڴ�

	//	��ʼ����
	printf("��ʼ����\r\n��������ģ�%s\r\n",str_tmp.data());
	unsigned long back_length = buff_length;					//	��ʼֵΪ���������ô�С

	re = CBase64::Decode(
		str_tmp,				//	���������
		out_buff,				//	�������
		&back_length			//	�õ������ĳ��ȣ�ע�⣡Ҳ���������������Ϊ�������ĳ���
		);

	if(re == false)			//	��鴦����
	{
		//	�ͷ��ڴ�
		delete[] out_buff;
		printf("Base64������󣡣����򼴽��˳���");
		_getch();
		return -1;
	}

	printf("����õ������ĳ��ȣ�%ld bytes\r\n�õ�����������Ϊ:",back_length);
	for (unsigned int i=0;i<back_length;i++)				//	ѭ����ӡ�������
	{
		int show_tmp = ((int)(out_buff[i]))&0x00FF;
		printf("%02X ",show_tmp);
	}

	//	�ͷ��ڴ�
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
    * ����˵�������������ݽ���base64���� 
    * ����˵����[in]pIn      ��Ҫ���б�������� 
                [in]uInLen  ����������ֽ��� 
                [out]strOut ����Ľ���base64����֮����ַ��� 
    * ����ֵ  ��true����ɹ�,falseʧ�� 
    * ��  ��  ��ChenLi 
    * ��дʱ�䣺2009-02-17 
    **********************************************************/  
    bool static Encode(const unsigned char *pIn, unsigned long uInLen, std::string& strOut);  
  
    /********************************************************* 
    * ����˵�������������ݽ���base64���� 
    * ����˵����[in]pIn          ��Ҫ���б�������� 
                [in]uInLen      ����������ֽ��� 
                [out]pOut       ����Ľ���base64����֮����ַ��� 
                [out]uOutLen    ����Ľ���base64����֮����ַ������� 
    * ����ֵ  ��true����ɹ�,falseʧ�� 
    * ��  ��  ��ChenLi 
    * ��дʱ�䣺2009-02-17 
    **********************************************************/  
    bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);  
      
    /********************************************************* 
    * ����˵�������������ݽ���base64���� 
    * ����˵����[in]strIn        ��Ҫ���н�������� 
                [out]pOut       �������֮��Ľ������� 
                [out]uOutLen    ����Ľ���֮����ֽ������� 
    * ����ֵ  ��true����ɹ�,falseʧ�� 
    * ��  ��  ��ChenLi 
    * ��дʱ�䣺2009-02-17 
    **********************************************************/  
    bool static Decode(const std::string& strIn, unsigned char *pOut, unsigned long *uOutLen) ;  
  
    /********************************************************* 
    * ����˵�������������ݽ���base64���� 
    * ����˵����[in]strIn        ��Ҫ���н�������� 
                [out]pOut       �������֮��Ľ������� 
                [out]uOutLen    ����Ľ���֮����ֽ������� 
    * ����ֵ  ��true����ɹ�,falseʧ�� 
    * ��  ��  ��ChenLi 
    * ��дʱ�䣺2009-02-17 
    **********************************************************/  
    //bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen) ;  
};  
  
#endif // ___BASE64_H___ 
