#include "bsp_tm7705.h"

#define PORT_CS		GPIOF
#define PIN_CS		GPIO_Pin_13

#define PORT_RST	GPIOF
#define PIN_RST		GPIO_Pin_15

#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)



/* 通信寄存器bit定义 */
enum
{
	/* 寄存器选择  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* 通信寄存器 */
	REG_SETUP	= 0x10,	/* 设置寄存器 */
	REG_CLOCK	= 0x20,	/* 时钟寄存器 */
	REG_DATA	= 0x30,	/* 数据寄存器 */
	REG_ZERO_CH1	= 0x60,	/* CH1 偏移寄存器 */
	REG_FULL_CH1	= 0x70,	/* CH1 满量程寄存器 */
	REG_ZERO_CH2	= 0x61,	/* CH2 偏移寄存器 */
	REG_FULL_CH2	= 0x71,	/* CH2 满量程寄存器 */

	/* 读写操作 */
	WRITE 		= 0x00,	/* 写操作 */
	READ 		= 0x08,	/* 读操作 */

	/* 通道 */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3		/* AIN1-  AIN2- */
};

/* 设置寄存器bit定义 */
enum
{
	MD_NORMAL		= (0 << 6),	/* 正常模式 */
	MD_CAL_SELF		= (1 << 6),	/* 自校准模式 */
	MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式 */
	MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式 */

	GAIN_1			= (0 << 3),	/* 增益 */
	GAIN_2			= (1 << 3),	/* 增益 */
	GAIN_4			= (2 << 3),	/* 增益 */
	GAIN_8			= (3 << 3),	/* 增益 */
	GAIN_16			= (4 << 3),	/* 增益 */
	GAIN_32			= (5 << 3),	/* 增益 */
	GAIN_64			= (6 << 3),	/* 增益 */
	GAIN_128		= (7 << 3),	/* 增益 */

	/* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
	BIPOLAR			= (0 << 2),	/* 双极性输入 */
	UNIPOLAR		= (1 << 2),	/* 单极性输入 */

	BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
	BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* 不启用 */
};

/* 时钟寄存器bit定义 */
enum
{
	CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
	CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */

	/*
		2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “0”。
		1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “1”
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
		四十九、电子秤应用中提高TM7705 精度的方法
			当使用主时钟为 2.4576MHz 时，强烈建议将时钟寄存器设为 84H,此时数据输出更新率为10Hz,即每0.1S 输出一个新数据。
			当使用主时钟为 1MHz 时，强烈建议将时钟寄存器设为80H, 此时数据输出更新率为4Hz, 即每0.25S 输出一个新数据
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
*	函 数 名: bsp_InitTM7705
*	功能说明: 配置STM32的GPIO和SPI接口，用于连接 TM7705
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTM7705(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 打开GPIO时钟 */

	SPI_Configuration();
	SPI_SetSpeed(SPI_BaudRatePrescaler_16);
	
	
	/* 配置几个推完输出IO */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_RST;
	GPIO_Init(PORT_RST, &GPIO_InitStructure);
	
	GPIO_SetBits(PORT_RST, PIN_RST);
	CS_1();
	
	/* 配置时钟寄存器 */
	TM7705_WriteReg(REG_CLOCK, CLKDIS_0 | CLK_4_9152M | FS_500HZ);
	delayms(1);
//	TM7705_WriteReg(REG_SETUP, MD_CAL_SELF | GAIN_1 | UNIPOLAR | BUF_NO | FSYNC_0);	
//	delayms(1);
	TM7705_WriteReg(REG_SETUP, MD_NORMAL | GAIN_1 | UNIPOLAR | BUF_NO | FSYNC_0);	
	delayms(1);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Delay
*	功能说明: CLK之间的延迟，时序延迟. 用于STM32F407  168M主频
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	delayus(20);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_WriteByte(uint8_t _data)
{
	SPI_ReadWrite(_data);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Recive8Bit
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t TM7705_ReadByte(void)
{
	return SPI_ReadWrite(0xFF);
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。 对于8位的寄存器，取32位形参的低8bit
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* 通信寄存器 */		
		case REG_SETUP:		/* 设置寄存器 8bit */
		case REG_CLOCK:		/* 时钟寄存器 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 偏移寄存器 24bit */
		case REG_FULL_CH1:	/* CH1 满量程寄存器 24bit */
		case REG_ZERO_CH2:	/* CH2 偏移寄存器 24bit */
		case REG_FULL_CH2:	/* CH2 满量程寄存器 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* 数据寄存器 16bit */
		default:
			return;
	}

	CS_0();
	TM7705_WriteByte(_RegID | WRITE | 1);	/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */
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
*	函 数 名: TM7705_ReadReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。 对于8位的寄存器，取32位形参的低8bit
*	返 回 值: 读到的寄存器值。 对于8位的寄存器，取32位形参的低8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID)
{
	uint8_t bits;
	uint32_t read = 0;

	switch (_RegID)
	{
		case REG_COMM:		/* 通信寄存器 */
		case REG_SETUP:		/* 设置寄存器 8bit */
		case REG_CLOCK:		/* 时钟寄存器 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 偏移寄存器 24bit */
		case REG_FULL_CH1:	/* CH1 满量程寄存器 24bit */
		case REG_ZERO_CH2:	/* CH2 偏移寄存器 24bit */
		case REG_FULL_CH2:	/* CH2 满量程寄存器 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* 数据寄存器 16bit */
			bits = 16;
			break;
		default:
			return 0xFFFFFFFF;
	}

	CS_0();
	TM7705_WriteByte(_RegID | READ | 1);	/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */
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
*	函 数 名: TM7705_ReadAdc1
*	功能说明: 读通道1或2的ADC数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int16_t TM7705_ReadAdc(uint8_t _ch)
{
	uint32_t read = 0;

	_ch = _ch & 0x01;
	CS_0();
	TM7705_WriteByte(REG_DATA | READ | _ch);	/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */
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
