#ifndef __MYIIC_H
#define __MYIIC_H

#include "hardware.h"
#include "MS51_16K.h"

/******************************************************************************************/
/* IIC1���� ����*/
// tm1650 2 scl <-----------------> mcu 16 p0.0 <--------> devboard B4
// tm1650 3 sda <-----------------> mcu 20 p0.4 <--------> devboard B0


/* i2c is driven by i2c module on devboard but driven by GPIO on production board */
sbit TM1650_SDA = P0^4;
sbit TM1650_SCL = P0^0;

/* IIC���в������� */
void IIC_Init(void);            /* ��ʼ��IIC��IO�� */
void IIC_Start(void);           /* ����IIC��ʼ�ź� */
void IIC_Stop(void);            /* ����IICֹͣ�ź� */
void IIC_Ack(void);             /* IIC����ACK�ź� */
void IIC_Nack(void);            /* IIC������ACK�ź� */
uint8_t IIC_Wait_Ack(void);     /* IIC�ȴ�ACK�ź� */
void IIC_Send_Byte(uint8_t txd);						/* IIC����һ���ֽ� */
uint8_t IIC_Receive_Byte(unsigned char ack);/* IIC��ȡһ���ֽ� */


void I2C_Write_Byte(uint8_t u8Addr, uint8_t u8DAT);
void I2C_Write_Word(uint8_t u8Addr, uint8_t u8DAT[], uint8_t siz);
uint8_t I2C_Read_Byte(uint8_t u8Addr);
//========================================================================================================



#endif
