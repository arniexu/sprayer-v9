#include "i2c_gpio.h"
#include "timer.h"
#include "hardware.h"
#include <intrins.h>
/**
 * @brief       ��ʼ��IIC
 * @param       ��
 * @retval      ��
 */
void IIC_Init(void)
{
	#ifdef DEVOARD 
	P13_OPENDRAIN_MODE;
	P14_OPENDRAIN_MODE;
	#else
	P00_OPENDRAIN_MODE;
	P04_OPENDRAIN_MODE;
	#endif
}

static void IIC_SDA(uint8_t val)
{
	if(val)
		TM1650_SDA = 1;
	else
		TM1650_SDA = 0;
}

static void IIC_SCL(uint8_t val)
{
	if(val)
		TM1650_SCL = 1;
	else
		TM1650_SCL = 0;
}

/**
 * @brief       IIC��ʱ����,���ڿ���IIC��д�ٶ�
 * @param       ��
 * @retval      ��
 */
static void IIC_delay2us(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

/**
 * @brief       ����IIC��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void IIC_Start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    IIC_delay2us();
    IIC_SDA(0);     /* START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź� */
    IIC_delay2us();
    IIC_SCL(0);     /* ǯסI2C���ߣ�׼�����ͻ�������� */
    IIC_delay2us();
}

/**
 * @brief       ����IICֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void IIC_Stop(void)
{
    IIC_SDA(0);     /* STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź� */
    IIC_delay2us();
    IIC_SCL(1);
    IIC_delay2us();
    IIC_SDA(1);     /* ����I2C���߽����ź� */
    IIC_delay2us();
}

/**
 * @brief       �ȴ�Ӧ���źŵ���
 * @param       ��
 * @retval      1������Ӧ��ʧ��
 *              0������Ӧ��ɹ�
 */
uint8_t IIC_Wait_Ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);     /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    IIC_delay2us();
    IIC_SCL(1);     /* SCL=1, ��ʱ�ӻ����Է���ACK */
    IIC_delay2us();

    while (TM1650_SDA)    /* �ȴ�Ӧ�� */
    {
        waittime++;

        if (waittime > 250)
        {
            IIC_Stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, ����ACK��� */
    IIC_delay2us();
    return rack;
}
#if 0
/**
 * @brief       ����ACKӦ��
 * @param       ��
 * @retval      ��
 */
void IIC_Ack(void)
{
    IIC_SDA(0);     /* SCL 0 -> 1  ʱ SDA = 0,��ʾӦ�� */
    IIC_delay2us();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    IIC_delay2us();
    IIC_SCL(0);
    IIC_delay2us();
    IIC_SDA(1);     /* �����ͷ�SDA�� */
    IIC_delay2us();
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
void IIC_Nack(void)
{
    IIC_SDA(1);     /* SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ�� */
    IIC_delay2us();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    IIC_delay2us();
    IIC_SCL(0);
    IIC_delay2us();
}
#endif
/**
 * @brief       IIC����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      ��
 */
void IIC_Send_Byte(uint8_t dat)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((dat & 0x80) >> 7);    /* ��λ�ȷ��� */
        IIC_delay2us();
        IIC_SCL(1);
        IIC_delay2us();
        IIC_SCL(0);
        dat <<= 1;     /* ����1λ,������һ�η��� */
    }
    IIC_SDA(1);         /* �������, �����ͷ�SDA�� */
}

#if 0
/**
 * @brief       IIC��ȡһ���ֽ�
 * @param       ack:  ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t IIC_Receive_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;  /* ��λ�����,�������յ�������λҪ���� */
        IIC_SCL(1);
        IIC_delay2us();

        if (TM1650_SDA)      /*�ߵ�ƽ������*/
        {
            receive++;
        }
        
        IIC_SCL(0);
        IIC_delay2us();
    }

    if (!ack)
    {
        IIC_Nack();     /* ����nACK */
    }
    else
    {
        IIC_Ack();      /* ����ACK */
    }

    return receive;
}
#endif
//--------------------------------------------------------------------------------------------
//----  Byte Write----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void I2C_Write_Byte(UINT8 u8Addr, UINT8 u8DAT)
{
	IIC_Start();
	IIC_Send_Byte(u8Addr & ~0x01);
	if (IIC_Wait_Ack() != 0)
		return;
	IIC_Send_Byte(u8DAT);
	if (IIC_Wait_Ack() != 0)
		return;
	IIC_Stop();
}

#if 0	
void I2C_Write_Word(UINT8 u8Addr, UINT8 u8DAT[], UINT8 siz)
{
	unsigned char  u8Count;
	/* Write Step1 */
	IIC_Start();
	
	/* Write Step2 */
	IIC_Send_Byte(u8Addr & ~0x01);
	if (IIC_Wait_Ack() != 0)
		return;
	
	/* Write Step3 */
	for (u8Count = 0; u8Count < siz; u8Count++)
	{
		/* Write Step2 */
		IIC_Send_Byte(u8DAT[u8Count]);
		if (IIC_Wait_Ack() != 0)
			return;
	}

	IIC_Stop();
}

//--------------------------------------------------------------------------------------------
//----  Byte Read ----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
UINT8 I2C_Read_Byte(UINT8 u8Addr)
{
	UINT8 u8Data = 0;
	
	IIC_Start();
	/* Write Step2 */
	IIC_Send_Byte(u8Addr & ~0x01);
	if (IIC_Wait_Ack() != 0)
		return 0xff;
	/* Step13 */
	u8Data = IIC_Receive_Byte(1);
        
  /* Step15 */
	IIC_Stop();
	return u8Data;
}
//========================================================================================================

#endif