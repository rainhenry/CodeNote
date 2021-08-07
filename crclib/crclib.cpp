/************************************************************************/
/*	CRC��Դ�ļ�                                                         */
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

//����ͷ�ļ�//////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "crclib.h"

//��������////////////////////////////////////////////////////////////////
//	CRC���㺯��
//	����CRC�����Ƿ�ɹ��Ĵ���
int CRC_Calc_Func(
	void* dat,						//	�������ݵ��׵�ַ
	uint32 length,					//	�������ݵ��ֽڸ���
	uint32* outdata,				//	������ݵĴ�ŵ�ַ
	CRC_Calc_Config cfg				//	��������
	)
{
	//	������ر���������C�淶
	uint8 intmp;							//	��������CRC�������ʱ����
	uint8 *pdat = (uint8 *)dat;				//	��ʼ�����뻺�������׵�ַ
	uint32 i;								//	����ѭ������
	uint32 outtmp;							//	����CRC����������ʱ����
	uint32 CRC_POLY = 0x00000000;			//	�������ʽֵ
	int re;									//	���������鷵��ֵ

	uint32 ShiftBitNum;						//	�������ʱ��Ҫ�ƶ���λ����Ŀ
	uint32 HighBitFilter;					//	��λ��������

	//	������ָ��
	if ((dat==NULL)||(dat==INVALID_ADDRESS))
	{
		return IN_ERROR_ADDRESS;
	}

	//	������ָ��
	if ((outdata==NULL)||(outdata==INVALID_ADDRESS))
	{
		return OUT_ERROR_ADDRESS;
	}

	//	�������
	re = CheckCRCConfig(cfg);
	if (re != CHECK_OK)
	{
		return re;					//	ʧ��
	} 

	//	����CRC�ĳ�ֵ
	switch(cfg.data_width)
	{
	case CRC8BIT:
		outtmp = 0x000000FF & cfg.Init;
		break;
	case CRC16BIT:
		outtmp = 0x0000FFFF & cfg.Init;
		break;
	case CRC32BIT:
		outtmp = 0xFFFFFFFF & cfg.Init;	
		break;
	}

	//	���ö���ʽ��ֵ
	switch(cfg.data_width)
	{
	case CRC8BIT:
		CRC_POLY = 0x000000FF & cfg.calc_poly.u32_dat;
		break;
	case CRC16BIT:
		CRC_POLY = 0x0000FFFF & cfg.calc_poly.u32_dat;
		break;
	case CRC32BIT:
		CRC_POLY = 0xFFFFFFFF & cfg.calc_poly.u32_dat;		
		break;
	}

	//	���������λ��Ŀ
	switch(cfg.data_width)
	{
	case CRC8BIT:
		ShiftBitNum = 0;
		break;
	case CRC16BIT:
		ShiftBitNum = 8;
		break;
	case CRC32BIT:
		ShiftBitNum = 24;	
		break;
	}

	//	�����λ����
	switch(cfg.data_width)
	{
	case CRC8BIT:
		HighBitFilter = 0x00000080;
		break;
	case CRC16BIT:
		HighBitFilter = 0x00008000;
		break;
	case CRC32BIT:
		HighBitFilter = 0x80000000;
		break;
	}

	//	��ʼ����CRC
	while (length--)
	{
		intmp = *pdat++;										//	����һ���ֽ�
		if(cfg.RefIn)	intmp = CRC_U8Invert(intmp);			//	���뷭ת
		outtmp ^= (((uint32) intmp) << ShiftBitNum);		//	���뵽���λ
		for (i = 0; i < 8; i++)
		{
			if (outtmp & HighBitFilter)
				outtmp = (outtmp << 1) ^ CRC_POLY;
			else
				outtmp <<= 1;
		}
	}
	switch(cfg.data_width)										//	�����ת
	{
	case CRC8BIT:
		if(cfg.RefOut)	outtmp = CRC_U8Invert((uint8)(outtmp&0x000000FF));
		break;
	case CRC16BIT:
		if(cfg.RefOut)	outtmp = CRC_U16Invert((uint16)(outtmp&0x0000FFFF));
		break;
	case CRC32BIT:
		if(cfg.RefOut)	outtmp = CRC_U32Invert(outtmp);
		break;
	}
	outtmp ^= cfg.XorOut;										//	������

	//	������
	switch(cfg.data_width)										//	�����ת
	{
	case CRC8BIT:
		*outdata = outtmp & 0x000000FF;
		break;
	case CRC16BIT:
		*outdata = outtmp & 0x0000FFFF;
		break;
	case CRC32BIT:
		*outdata = outtmp & 0xFFFFFFFF;
		break;
	}

	//	���ؼ���״̬
	return CRC_CALC_SUCCESS;
}

//	���ò������
int CheckCRCConfig(CRC_Calc_Config cfg)
{
	//	������ݿ��
	if (!( \
		(cfg.data_width == CRC8BIT) || \
		(cfg.data_width == CRC16BIT) || \
		(cfg.data_width == CRC32BIT) \
		))
	{
		return DATA_WIDTH_FAIL;			//	ʧ��
	}

	//	������ʽ
	if (cfg.calc_poly.u32_dat == 0)
	{
		return POLY_FAIL;				//	ʧ��
	}

	return CHECK_OK;
}

//	8λ�ֽڷ�ת
uint8 CRC_U8Invert(uint8 chSrc)
{
	int i;
	uint8 det = 0x00;
	for (i=0;i<8;i++)
	{
		if ((chSrc&(1<<i)) == 0)
		{
		} 
		else
		{
			det |= 1<<(7-i);
		}
	}
	return det;
}

//	16λ���ַ�ת
uint16 CRC_U16Invert(uint16 chSrc)
{
	int i;
	uint16 det = 0x0000;
	for (i=0;i<16;i++)
	{
		if ((chSrc&(1<<i)) == 0)
		{
		} 
		else
		{
			det |= 1<<(15-i);
		}
	}
	return det;
}

//	32λ˫�ַ�ת
uint32  CRC_U32Invert(uint32 chSrc)
{
	int i;
	uint32 det = 0x00000000;
	for (i=0;i<32;i++)
	{
		if ((chSrc&(1<<i)) == 0)
		{
		} 
		else
		{
			det |= 1<<(31-i);
		}
	}
	return det;
}