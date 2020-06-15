#include "bsp_tm7705.h"

#define PORT_CS		GPIOF
#define PIN_CS		GPIO_Pin_13

#define PORT_RST	GPIOF
#define PIN_RST		GPIO_Pin_15

#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)



/* ͨ�żĴ���bit���� */
enum
{
	/* �Ĵ���ѡ��  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* ͨ�żĴ��� */
	REG_SETUP	= 0x10,	/* ���üĴ��� */
	REG_CLOCK	= 0x20,	/* ʱ�ӼĴ��� */
	REG_DATA	= 0x30,	/* ���ݼĴ��� */
	REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
	REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
	REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
	REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

	/* ��д���� */
	WRITE 		= 0x00,	/* д���� */
	READ 		= 0x08,	/* ������ */

	/* ͨ�� */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3		/* AIN1-  AIN2- */
};

/* ���üĴ���bit���� */
enum
{
	MD_NORMAL		= (0 << 6),	/* ����ģʽ */
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

	GAIN_1			= (0 << 3),	/* ���� */
	GAIN_2			= (1 << 3),	/* ���� */
	GAIN_4			= (2 << 3),	/* ���� */
	GAIN_8			= (3 << 3),	/* ���� */
	GAIN_16			= (4 << 3),	/* ���� */
	GAIN_32			= (5 << 3),	/* ���� */
	GAIN_64			= (6 << 3),	/* ���� */
	GAIN_128		= (7 << 3),	/* ���� */

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* ������ */
};

/* ʱ�ӼĴ���bit���� */
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	/*
		��ʮ�š����ӳ�Ӧ�������TM7705 ���ȵķ���
			��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
			��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
	*/
	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};

void TM7705_WriteByte(uint8_t _data);
uint8_t TM7705_ReadByte(void);
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue);
static void TM7705_Delay(void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTM7705
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� TM7705
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTM7705(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ��GPIOʱ�� */

	SPI_Configuration();
	SPI_SetSpeed(SPI_BaudRatePrescaler_16);
	
	
	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_RST;
	GPIO_Init(PORT_RST, &GPIO_InitStructure);
	
	GPIO_SetBits(PORT_RST, PIN_RST);
	CS_1();
	
	/* ����ʱ�ӼĴ��� */
	TM7705_WriteReg(REG_CLOCK, CLKDIS_0 | CLK_4_9152M | FS_500HZ);
	delayms(1);
//	TM7705_WriteReg(REG_SETUP, MD_CAL_SELF | GAIN_1 | UNIPOLAR | BUF_NO | FSYNC_0);	
//	delayms(1);
	TM7705_WriteReg(REG_SETUP, MD_NORMAL | GAIN_1 | UNIPOLAR | BUF_NO | FSYNC_0);	
	delayms(1);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Delay
*	����˵��: CLK֮����ӳ٣�ʱ���ӳ�. ����STM32F407  168M��Ƶ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	delayus(20);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_WriteByte(uint8_t _data)
{
	SPI_ReadWrite(_data);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t TM7705_ReadByte(void)
{
	return SPI_ReadWrite(0xFF);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */		
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return;
	}

	CS_0();
	TM7705_WriteByte(_RegID | WRITE | 1);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */
	CS_1();
	TM7705_Delay();
	CS_0();
	if (bits == 8)
	{
		TM7705_WriteByte((uint8_t)_RegValue);
	}
	else	/* 24bit */
	{
		TM7705_WriteByte(_RegValue>>16);
		TM7705_WriteByte(_RegValue>>8);
		TM7705_WriteByte(_RegValue);
	}
	CS_1();
	TM7705_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: �����ļĴ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID)
{
	uint8_t bits;
	uint32_t read = 0;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
			bits = 16;
			break;
		default:
			return 0xFFFFFFFF;
	}

	CS_0();
	TM7705_WriteByte(_RegID | READ | 1);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */
	CS_1();
	TM7705_Delay();
	CS_0();
	if (bits == 16)
	{
		read = TM7705_ReadByte();
		read = read << 8;
		read = read + TM7705_ReadByte();
	}
	else if (bits == 8)
	{
		read = TM7705_ReadByte();
	}
	else	/* 24bit */
	{
		read = TM7705_ReadByte();
		read = read << 8;
		read = read + TM7705_ReadByte();
		read = read << 8;
		read = read + TM7705_ReadByte();
	}
	CS_1();
	TM7705_Delay();
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadAdc1
*	����˵��: ��ͨ��1��2��ADC����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int16_t TM7705_ReadAdc(uint8_t _ch)
{
	uint32_t read = 0;

	_ch = _ch & 0x01;
	CS_0();
	TM7705_WriteByte(REG_DATA | READ | _ch);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */
	CS_1();
	TM7705_Delay();
	CS_0();
	read = TM7705_ReadByte();
	read = read << 8;
	read = read + TM7705_ReadByte();
	CS_1();
	TM7705_Delay();
	return read;
}

void TM7705_ReadAll(u32 *buf)
{
	u8  i;
	for(i=1;i<8;i++){
		buf[i]=TM7705_ReadReg((i<<4)|1);
	}
}
