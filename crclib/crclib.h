/************************************************************************/
/*	CRC��ͷ�ļ�                                                         */
/*	֧��CRC8 CRC16 CRC32												*/
/*	�������ڣ�2016.08.19												*/
/*	�汾��REV 1.0														*/
/*	���룺rainhenry														*/
/*	��ƣ�rainhenry														*/
/*	����/У�ԣ�rainhenry												*/
/*	����ƽ̨��Windows 7 64bit, VS2010									*/
/*	��л�������ǵ�����													*/
/*	�������������������ת��ʹ�ã�������ѧϰ�о�������������ҵ��;��	*/
/*		  ת��ʹ��ʱ�뱣����ע�͡�										*/
/************************************************************************/

/*************************************************************************
	ʹ�ò���
		1������CRC��������
		2�����ü��㺯��CRC_Calc_Func������ɹ�������CRC_CALC_SUCCESS

**************************************************************************
	ʹ�þ���

	//	�����û�����
	unsigned char dat[] = {'1','2','3','4','5','6','7','8','9'};

	//	�������CRC����ı���
	unsigned int crc_out;

	//	����CRC������
	CRC_Calc_Config cfg;

	//////////////////////////////////////////////////////////////////////////
	//	����32λ CRC-32 for WinRAR
	//	�������ݿ��Ϊ32λ
	cfg.data_width = CRC32BIT;							
	//	���ö���ʽ
	cfg.calc_poly.u32_dat = 0;
	//cfg.calc_poly.bit_dat.D32 = 1;		//	�˴�Ҫ���Դ����
	cfg.calc_poly.bit_dat.D26 = 1;
	cfg.calc_poly.bit_dat.D23 = 1;
	cfg.calc_poly.bit_dat.D22 = 1;
	cfg.calc_poly.bit_dat.D16 = 1;
	cfg.calc_poly.bit_dat.D12 = 1;
	cfg.calc_poly.bit_dat.D11 = 1;
	cfg.calc_poly.bit_dat.D10 = 1;
	cfg.calc_poly.bit_dat.D8 = 1;
	cfg.calc_poly.bit_dat.D7 = 1;
	cfg.calc_poly.bit_dat.D5 = 1;
	cfg.calc_poly.bit_dat.D4 = 1;
	cfg.calc_poly.bit_dat.D2 = 1;
	cfg.calc_poly.bit_dat.D1 = 1;
	cfg.calc_poly.bit_dat.D0 = 1;
	//	���ó�ֵ
	cfg.Init = 0xFFFFFFFF;
	//	�������뷭ת
	cfg.RefIn = true;
	//	���������ת
	cfg.RefOut = true;
	//	����������
	cfg.XorOut = 0xFFFFFFFF;

	//	����
	if (CRC_Calc_Func(
	dat,
	sizeof(dat),
	&crc_out,
	cfg) == CRC_CALC_SUCCESS)
	{
	printf("CRC32����ɹ������Ϊ��0x%08X\r\n",crc_out);
	}
	else
	{
	printf("CRC32����ʧ�ܣ�");
	}

	//////////////////////////////////////////////////////////////////////////
	//	����16λ CRC-16/MODBUS
	//	�������ݿ��Ϊ16λ
	cfg.data_width = CRC16BIT;							
	//	���ö���ʽ
	cfg.calc_poly.u32_dat = 0;
	cfg.calc_poly.bit_dat.D16 = 1;		//	�������п���
	cfg.calc_poly.bit_dat.D15 = 1;
	cfg.calc_poly.bit_dat.D2 = 1;
	cfg.calc_poly.bit_dat.D0 = 1;
	//	���ó�ֵ
	cfg.Init = 0xFFFF;
	//	�������뷭ת
	cfg.RefIn = true;
	//	���������ת
	cfg.RefOut = true;
	//	����������
	cfg.XorOut = 0x0000;

	//	����
	if (CRC_Calc_Func(
	dat,
	sizeof(dat),
	&crc_out,
	cfg) == CRC_CALC_SUCCESS)
	{
	printf("CRC16����ɹ������Ϊ��0x%04X\r\n",crc_out);
	}
	else
	{
	printf("CRC16����ʧ�ܣ�");
	}

	//////////////////////////////////////////////////////////////////////////
	//	����8λ	CRC-8/ROHC
	//	�������ݿ��Ϊ8λ
	cfg.data_width = CRC8BIT;							
	//	���ö���ʽ
	cfg.calc_poly.u32_dat = 0;
	cfg.calc_poly.bit_dat.D8 = 1;		//	�������п���
	cfg.calc_poly.bit_dat.D2 = 1;
	cfg.calc_poly.bit_dat.D1 = 1;
	cfg.calc_poly.bit_dat.D0 = 1;
	//	���ó�ֵ
	cfg.Init = 0xFF;
	//	�������뷭ת
	cfg.RefIn = true;
	//	���������ת
	cfg.RefOut = true;
	//	����������
	cfg.XorOut = 0x00;

	//	����
	if (CRC_Calc_Func(
	dat,
	sizeof(dat),
	&crc_out,
	cfg) == CRC_CALC_SUCCESS)
	{
	printf("CRC8����ɹ������Ϊ��0x%02X\r\n",crc_out);
	}
	else
	{
	printf("CRC8����ʧ�ܣ�");
	}
*************************************************************************/

#ifndef __CRCLIB_H__
#define __CRCLIB_H__

//�ʹ�������ص�����//////////////////////////////////////////////////////
#define		POSSEQ									//	������������ȡ���ö���Ϊ��������

#ifndef uint32
	typedef unsigned int uint32;					//	��32λ��������
	//	typedef unsigned long int uint32;			//	��16λ����������8λ��������
#endif

#ifndef uint16
	typedef unsigned short int uint16;				//	��32λ��������
	//	typedef unsigned int uint16;				//	��16λ����������8λ��������
#endif

#ifndef uint8
	typedef unsigned char uint8;
#endif 

#ifndef __cplusplus
	typedef unsigned char bool;						//	��C�����ж��岼������
#endif

//��׼���������//////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

#ifndef INVALID_ADDRESS
#define INVALID_ADDRESS ((void*)(-1))
#endif

//����ֵ��ض���//////////////////////////////////////////////////////////
//	����ֵ����
#define CRC_CALC_SUCCESS	0						//	����ɹ�
#define CHECK_OK			0						//	��������
#define POLY_FAIL			1						//	�Ƿ�����ʽ
#define DATA_WIDTH_FAIL		2						//	�Ƿ����ݿ��
#define IN_ERROR_ADDRESS	3						//	�������ָ��Ϊ�����ַ
#define OUT_ERROR_ADDRESS	4						//	�������ָ��Ϊ�����ַ

//���Ͷ���////////////////////////////////////////////////////////////////
//	CRC���ݿ��ö�ٶ���
enum DataWidth {CRC8BIT = 8,CRC16BIT = 16,CRC32BIT = 32};

//	����ʽλ��ṹ��
typedef struct tagPolyBit
{
#ifdef POSSEQ
	uint8 D0:1;
	uint8 D1:1;
	uint8 D2:1;
	uint8 D3:1;
	uint8 D4:1;
	uint8 D5:1;
	uint8 D6:1;
	uint8 D7:1;
	uint8 D8:1;
	uint8 D9:1;
	uint8 D10:1;
	uint8 D11:1;
	uint8 D12:1;
	uint8 D13:1;
	uint8 D14:1;
	uint8 D15:1;
	uint8 D16:1;
	uint8 D17:1;
	uint8 D18:1;
	uint8 D19:1;
	uint8 D20:1;
	uint8 D21:1;
	uint8 D22:1;
	uint8 D23:1;
	uint8 D24:1;
	uint8 D25:1;
	uint8 D26:1;
	uint8 D27:1;
	uint8 D28:1;
	uint8 D29:1;
	uint8 D30:1;
	uint8 D31:1;
#else	//	not POSSEQ
	uint8 D31:1;
	uint8 D30:1;
	uint8 D29:1;
	uint8 D28:1;
	uint8 D27:1;
	uint8 D26:1;
	uint8 D25:1;
	uint8 D24:1;
	uint8 D23:1;
	uint8 D22:1;
	uint8 D21:1;
	uint8 D20:1;
	uint8 D19:1;
	uint8 D18:1;
	uint8 D17:1;
	uint8 D16:1;
	uint8 D15:1;
	uint8 D14:1;
	uint8 D13:1;
	uint8 D12:1;
	uint8 D11:1;
	uint8 D10:1;
	uint8 D9:1;
	uint8 D8:1;
	uint8 D7:1;
	uint8 D6:1;
	uint8 D5:1;
	uint8 D4:1;
	uint8 D3:1;
	uint8 D2:1;
	uint8 D1:1;
	uint8 D0:1;
#endif	//	POSSEQ
}PolyBit;

//	����ʽ������
typedef union tagPolyUnion
{
	PolyBit bit_dat;
	uint32 u32_dat;
}PolyUnion;

//	CRC��������ṹ��
typedef struct tagCRC_Calc_Config 
{
	DataWidth data_width;			//	CRC�����ݿ��
	PolyUnion calc_poly;			//	CRC����Ķ���ʽ
	uint32 Init;					//	��ʼֵ
	bool RefIn;						//	�����Ƿ�ת
	bool RefOut;					//	����Ƿ�ת
	uint32 XorOut;					//	������ֵ
}CRC_Calc_Config;

//	C����
#ifdef __cplusplus
extern "C"
{
#endif
//�����û�ʹ�õĺ���//////////////////////////////////////////////////////
	//	CRC���㺯��
	//	����CRC�����Ƿ�ɹ��Ĵ���
	int CRC_Calc_Func(
		void* dat,						//	�������ݵ��׵�ַ
		uint32 length,					//	�������ݵ��ֽڸ���
		uint32* outdata,				//	������ݵĴ�ŵ�ַ
		CRC_Calc_Config cfg				//	��������
		);

//�������ڲ��Ӻ���////////////////////////////////////////////////////////
	//	���ò������
	int CheckCRCConfig(CRC_Calc_Config cfg);

	//	8λ�ֽڷ�ת
	uint8 CRC_U8Invert(uint8 chSrc);

	//	16λ���ַ�ת
	uint16 CRC_U16Invert(uint16 chSrc);

	//	32λ˫�ַ�ת
	uint32  CRC_U32Invert(uint32 chSrc);

//	C����
#ifdef __cplusplus
};		//	extern "C"
#endif

#endif //	__CRCLIB_H__
