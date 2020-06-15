/******************************************************************************* 
*  ���ļ�ΪSPI����SD���ĵײ������ļ�
*  ����SPIģ�鼰���IO�ĳ�ʼ����SPI��дSD����дָ������ݵȣ�
*******************************************************************************/   
#include "mmc_sd.h"	

u8  SD_Type=0;//SD��������	
//__align(4) u8 SD_DATA_BUFFER[512];	
SD_CardInfo SDCardInfo;

#define SD_CARD_CS_RCC 		RCC_APB2Periph_GPIOB
#define SD_CARD_CS_GPIO		GPIOB
#define SD_CARD_CS_PIN		GPIO_Pin_12

/*******************************************************************************
* Function Name  : SPI_ControlLine
* Description    : SPI2ģ���ʼ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_ControlLine(void)
{	 
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_Pin=SD_CARD_CS_PIN;
	GPIO_Init(SD_CARD_CS_GPIO,&gpio);
	SD_CS_DISABLE();
	
	SPI_Configuration();
}

/*******************************************************************************
* Function Name  : SPI_SetSpeed_Local
* Description    : SPI2�����ٶ�Ϊ����
* Input          : u8 SpeedSet 
*                  ����ٶ���������0�������ģʽ����0�����ģʽ
*                  SPI_SPEED_HIGH   1
*                  SPI_SPEED_LOW    0
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_SetSpeed_Local(u8 SpeedSet)
{
  SPI_InitTypeDef  SPI_InitStructure;
  if(SpeedSet==SPI_SPEED_HIGH)//����
  {
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_Init(SPI2, &SPI_InitStructure);
  /* SPI2 enable */
  SPI_Cmd(SPI2, ENABLE);
  }
  else//����
  {
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI2, &SPI_InitStructure);
    /* SPI2 enable */
    SPI_Cmd(SPI2, ENABLE);	
  }
}
	 
/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI2��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : u8 TxData �����͵���
* Output         : None
* Return         : u8 RxData �յ�����
*******************************************************************************/
u8 SPI_ReadWriteByte(u8 TxData)
{      
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������	
  SPI_I2S_SendData(SPI2, TxData);//����һ��byte
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ�������һ��byte
  return SPI_I2S_ReceiveData(SPI2); //�����յ�������
}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : SD��æ���
* Input          : None
* Output         : None
* Return         : u8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WaitReady(void)
{
  u8 r1;
  u16 retry=0;
  do
  {
    r1 = SPI_ReadWriteByte(0xFF);
    retry++;
    if(retry==0xfffe)
      return 1; 
  }while(r1!=0xFF); 
    return 0;
}	 
/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : ��SD������һ������
* Input          : u8 cmd   ���� 
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
u8 SD_SendCommand(u8 cmd, u32 arg,u8 crc)
{
  unsigned char r1;
  unsigned int Retry = 0;

  SD_CS_DISABLE();
  SPI_ReadWriteByte(0xff);//��߼����ԣ����û�������ЩSD�����ܲ�֧��
  SD_CS_ENABLE();//Ƭѡ���õͣ�ѡ��SD��
  
  /* ������������ */
  SPI_ReadWriteByte(cmd | 0x40);                      
  SPI_ReadWriteByte((u8)(arg >> 24));//����[31..24]
  SPI_ReadWriteByte((u8)(arg >> 16));//����[23..16]
  SPI_ReadWriteByte((u8)(arg >> 8));//����[15..8]
  SPI_ReadWriteByte((u8)arg);    //����[7..0]
  SPI_ReadWriteByte(crc);
    
  //�ȴ���Ӧ����ʱ�˳�
  while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
  {
    Retry++;
    if(Retry > 800)break; //����ʵ���ã�������Դ������
  }   
  //�ر�Ƭѡ
  SD_CS_DISABLE();
  //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
  SPI_ReadWriteByte(0xFF);

  //����״ֵ̬
  return r1;
}


/*******************************************************************************
* Function Name  : SD_SendCommand_NoDeassert
* Description    : ��SD������һ������(����ʱ��ʧ��Ƭѡ�����к������ݴ�����
* Input          : u8 cmd   ���� 
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg,u8 crc)
{
  unsigned char r1;
  unsigned int Retry = 0;

  SD_CS_DISABLE();
  SPI_ReadWriteByte(0xff);//��߼����ԣ����û�������ЩSD�����ܲ�֧��
  SD_CS_ENABLE();//Ƭѡ���õͣ�ѡ��SD��
  
  /* ������������ */
  SPI_ReadWriteByte(cmd | 0x40);                      
  SPI_ReadWriteByte((u8)(arg >> 24));//����[31..24]
  SPI_ReadWriteByte((u8)(arg >> 16));//����[23..16]
  SPI_ReadWriteByte((u8)(arg >> 8));//����[15..8]
  SPI_ReadWriteByte((u8)arg);    //����[7..0]
  SPI_ReadWriteByte(crc);
  
  //�ȴ���Ӧ����ʱ�˳�
  while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
  {
    Retry++;
    if(Retry > 600)break; //����ʵ���ã�������Դ������  
  }
  //������Ӧֵ
  return r1;
}

/*******************************************************************************
* Function Name  : SD_Init
* Description    : ��ʼ��SD��
* Input          : None
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  99��NO_CARD
*******************************************************************************/
u8 SD_Init(void)
{
  u16 i;      // ����ѭ������
  u8 r1;      // ���SD���ķ���ֵ
  u16 retry;  // �������г�ʱ����
  u8 buff[6];
					 
  SPI_ControlLine(); 	//SPI�����ó�ʼ��
  SPI_SetSpeed_Local(SPI_SPEED_HIGH);
  SD_CS_ENABLE();  
  // ����ʱ���ȴ�SD���ϵ����
  for(i=0;i<0xf00;i++);

  //�Ȳ�������74�����壬��SD���Լ���ʼ�����
//  for(i=0;i<10;i++)
//  {
//    SPI_ReadWriteByte(0xFF);//80clks
//  }

  //-----------------SD����λ��idle��ʼ-----------------
  //ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
  //��ʱ��ֱ���˳�
  retry = 0;
  do	
  {
    //����CMD0����SD������IDLE״̬
    r1 = SD_SendCommand(CMD0, 0,0x95);
			retry++;
  }while((r1 != 0x01) && (retry<200));
  //����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
  if(retry==200) return 1;   //��ʱ����1	
  
  //-----------------SD����λ��idle����-----------------	 
  //��ȡ��Ƭ��SD�汾��Ϣ
  r1 = SD_SendCommand_NoDeassert(CMD8, 0x1aa,0x87);	     
    //�����Ƭ�汾��Ϣ��v1.0�汾�ģ���r1=0x05����������³�ʼ��
  if(r1 == 0x05)
  {
     //���ÿ�����ΪSDV1.0����������⵽ΪMMC�������޸�ΪMMC
     SD_Type = SD_TYPE_V1;	   
     //�����V1.0����CMD8ָ���û�к�������
     //Ƭѡ�øߣ�������������
     SD_CS_DISABLE();
     //�෢8��CLK����SD������������
     SPI_ReadWriteByte(0xFF);	  
     //-----------------SD����MMC����ʼ����ʼ-----------------	 
     //������ʼ��ָ��CMD55+ACMD41
     // �����Ӧ��˵����SD�����ҳ�ʼ�����
     // û�л�Ӧ��˵����MMC�������������Ӧ��ʼ��
     retry = 0;
     do
     {
       //�ȷ�CMD55��Ӧ����0x01���������
       r1 = SD_SendCommand(CMD55, 0, 0);
       if(r1 != 0x01)
         return r1;	  
       //�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������400��
       r1 = SD_SendCommand(ACMD41, 0, 0);
       retry++;
     }while((r1!=0x00) && (retry<400));
     // �ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
     // ���л�Ӧ����SD����û�л�Ӧ����MMC��
        
     //----------MMC�������ʼ��������ʼ------------
     if(retry==400)
     {
        retry = 0;
        //����MMC����ʼ�����û�в��ԣ�
        do
        {
           r1 = SD_SendCommand(CMD1, 0, 0);
           retry++;
        }while((r1!=0x00)&& (retry<400));
        if(retry==400)return 1;   //MMC����ʼ����ʱ		    
        //д�뿨����
        SD_Type = SD_TYPE_MMC;
     }
     //----------MMC�������ʼ����������------------	    
     //����SPIΪ����ģʽ
     SPI_SetSpeed_Local(SPI_SPEED_HIGH);   
     SPI_ReadWriteByte(0xFF);
        
     //��ֹCRCУ��	   
     r1 = SD_SendCommand(CMD59, 0, 0x95);
     if(r1 != 0x00)return r1;  //������󣬷���r1   	   
     //����Sector Size
     r1 = SD_SendCommand(CMD16, 512, 0x95);
     if(r1 != 0x00)return r1;//������󣬷���r1		 
     //-----------------SD����MMC����ʼ������-----------------

  }//SD��ΪV1.0�汾�ĳ�ʼ������	 
  //������V2.0���ĳ�ʼ��
  //������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��
  else if(r1 == 0x01)
  {
    //V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ٽ���������
    buff[0] = SPI_ReadWriteByte(0xFF);  //should be 0x00
    buff[1] = SPI_ReadWriteByte(0xFF);  //should be 0x00
    buff[2] = SPI_ReadWriteByte(0xFF);  //should be 0x01
    buff[3] = SPI_ReadWriteByte(0xFF);  //should be 0xAA	    
    SD_CS_DISABLE();	  
    SPI_ReadWriteByte(0xFF);//the next 8 clocks			 
    //�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
    //if(buff[2]==0x01 && buff[3]==0xAA) //�粻�жϣ�����֧�ֵĿ�����
   // {	  
       retry = 0;
       //������ʼ��ָ��CMD55+ACMD41
       do
       {
    	  r1 = SD_SendCommand(CMD55, 0, 0);
    	  if(r1!=0x01)	return r1;	   
    	  r1 = SD_SendCommand(ACMD41, 0x40000000, 1);
        retry++;
				delayus(200);
				if(retry>200)	return r1;  //��ʱ�򷵻�r1״̬  
       }while(r1!=0);
       //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ		   
       //-----------����SD2.0���汾��ʼ-----------
       r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
       if(r1!=0x00)  return r1;  //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��		 
       //��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
       buff[0] = SPI_ReadWriteByte(0xFF);
       buff[1] = SPI_ReadWriteByte(0xFF); 
       buff[2] = SPI_ReadWriteByte(0xFF); 
       buff[3] = SPI_ReadWriteByte(0xFF);

       //OCR������ɣ�Ƭѡ�ø�
       SD_CS_DISABLE();
       SPI_ReadWriteByte(0xFF);

       //�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
       //���CCS=1��SDHC   CCS=0��SD2.0
       if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //���CCS	 
       else SD_Type = SD_TYPE_V2;	    
       //-----------����SD2.0���汾����----------- 
       //����SPIΪ����ģʽ
       SPI_SetSpeed_Local(1);  
   // }	    
  }
    return r1;
}



/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : ��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
* Input          : u8 *data(��Ŷ������ݵ��ڴ�>len)
*                  u16 len(���ݳ��ȣ�
*                  u8 release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  other��������Ϣ
*******************************************************************************/
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
  u16 retry;
  u8 r1;
  // ����һ�δ���
  SD_CS_ENABLE();
  //�ȴ�SD������������ʼ����0xFE
  retry = 0;										   
  do
  {	
    r1 = SPI_ReadWriteByte(0xFF);
    retry++;
    if(retry>4000)  //4000�εȴ���û��Ӧ���˳�����(����ʵ����ԣ��˴���ö��Լ���
    {
      SD_CS_DISABLE();
        return 1;
    }
	}while(r1 != 0xFE);
		   
	//��ʼ��������
	while(len--)
	{
		*data = SPI_ReadWriteByte(0xFF);
		data++;
	}
	//������2��αCRC��dummy CRC��
	SPI_ReadWriteByte(0xFF);
	SPI_ReadWriteByte(0xFF);
	//�����ͷ����ߣ���CS�ø�
	if(release == RELEASE)
	{
		//�������
		SD_CS_DISABLE();
		SPI_ReadWriteByte(0xFF);
	}											  					    
	return 0;
}


/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : ��ȡSD����CID��Ϣ��������������Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
u8 SD_GetCID(u8 *cid_data)
{
  u8 r1;
  //��CMD10�����CID
  r1 = SD_SendCommand(CMD10, 0, 0xFF);
  if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������    
  //����16���ֽڵ�����
  SD_ReceiveData(cid_data, 16, RELEASE);	 
  return 0;
}


/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
u8 SD_GetCSD(u8 *csd_data)
{
  u8 r1;

  //��CMD9�����CSD
  r1 = SD_SendCommand(CMD9, 0, 0xFF);
  if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������  
  //����16���ֽڵ�����
  SD_ReceiveData(csd_data, 16, RELEASE);

  return 0;
}


/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : ��ȡSD�����������ֽڣ�
* Input          : None
* Output         : None
* Return         : u32 capacity 
*                   0�� ȡ�������� 
*******************************************************************************/
u32 SD_GetCapacity(void)
{
  u8 csd[16];
  u32 Capacity;
  u8 r1;
  u16 i;
  u16 temp;
  //ȡCSD��Ϣ������ڼ��������0
  if(SD_GetCSD(csd)!=0) return 0;	    
  //���ΪSDHC�����������淽ʽ����
  if((csd[0]&0xC0)==0x40)
  {									  
    Capacity=((u32)csd[8])<<8;
    Capacity+=(u32)csd[9]+1;	 
    Capacity = (Capacity)*1024;//�õ�������
    Capacity*=512;//�õ��ֽ���			   
  }
  else
  {		    
    i = csd[6]&0x03;
    i<<=8;
    i += csd[7];
    i<<=2;
    i += ((csd[8]&0xc0)>>6);
    
    //C_SIZE_MULT
    r1 = csd[9]&0x03;
    r1<<=1;
    r1 += ((csd[10]&0x80)>>7);	 
    r1+=2;//BLOCKNR
    temp = 1;
    while(r1)
    {
      temp*=2;
      r1--;
    }
    Capacity = ((u32)(i+1))*((u32)temp);	 
    // READ_BL_LEN
    i = csd[5]&0x0f;
    //BLOCK_LEN
    temp = 1;
    while(i)
    {
      temp*=2;
      i--;
    }
    //The final result
    Capacity *= (u32)temp;//�ֽ�Ϊ��λ 	  
  }

  return (u32)Capacity;
}


/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : ��SD����һ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
  u8 r1;
	u16 i;
	u16 retry;

  //����Ϊ����ģʽ
  SPI_SetSpeed_Local(SPI_SPEED_HIGH);

	if(SD_Type!=SD_TYPE_V2HC){
     sector = sector<<9;//512*sector�����������ı߽�����ַ
  }
  r1 = SD_SendCommand(CMD17, sector, 1);//������

  if(r1 != 0x00)return r1; 		   							  

  SD_CS_ENABLE();
  //�ȴ�SD������������ʼ����0xFE
  retry = 0;										   
  do{	
    r1 = SPI_ReadWriteByte(0xFF);
    retry++;
    if(retry>4000)  //4000�εȴ���û��Ӧ���˳�����(����ʵ����ԣ��˴���ö��Լ���
    {
      SD_CS_DISABLE();
        return 1;
    }
	}while(r1 != 0xFE);
	
	
//	SDCard_DMA_RecvConfig((u32)&SPI2->DR, (u32)buffer, 512);

	//��ʼ��������
	for(i=0;i<512;i++){
		*buffer = SPI_ReadWriteByte(0xFF);
		buffer++;
	}
	//������2��αCRC��dummy CRC��
	SPI_ReadWriteByte(0xFF);
	SPI_ReadWriteByte(0xFF);
	//�������
	SD_CS_DISABLE();
	SPI_ReadWriteByte(0xFF);
	return 0;
}


/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : д��SD����һ��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
  u8 r1;
  u16 i;
  u16 retry;

  //����Ϊ����ģʽ
  SPI_SetSpeed_Local(SPI_SPEED_HIGH);

  //�������SDHC����������sector��ַ������ת����byte��ַ
  if(SD_Type!=SD_TYPE_V2HC){
     sector = sector<<9;//512*sector�����������ı߽�����ַ
  }

  r1 = SD_SendCommand(CMD24, sector, 0x00);
  if(r1 != 0x00){
    return r1;  //Ӧ����ȷ��ֱ�ӷ���
  }
    
  //��ʼ׼�����ݴ���
  SD_CS_ENABLE();
  //�ȷ�3�������ݣ��ȴ�SD��׼����
  SPI_ReadWriteByte(0xff);
  SPI_ReadWriteByte(0xff);
  SPI_ReadWriteByte(0xff);
  //����ʼ����0xFE
  SPI_ReadWriteByte(0xFE);

//	SDCard_DMA_SendConfig((u32)&SPI2->DR, (u32)data, 512);

  //��һ��sector������
  for(i=0;i<512;i++){
     SPI_ReadWriteByte(*data++);
  }
  //��2��Byte��dummy CRC
  SPI_ReadWriteByte(0xff);
  SPI_ReadWriteByte(0xff);
    
  //�ȴ�SD��Ӧ��
  r1 = SPI_ReadWriteByte(0xff);
  if((r1&0x1F)!=0x05){
     SD_CS_DISABLE();
     return r1;
  }
    
  //�ȴ��������
  retry = 0;
  while(!SPI_ReadWriteByte(0xff)){//���Ա��ʱ�������߱�����
     retry++;
     if(retry>65534){       //�����ʱ��д��û����ɣ������˳�
        SD_CS_DISABLE();
        return 1;           //д�볬ʱ����1
     }
  }
  //д����ɣ�Ƭѡ��1
  SD_CS_DISABLE();
  SPI_ReadWriteByte(0xff);
	return 0;
}

//��SD��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;				  				 
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 sta=SD_OK;
	sta=SD_ReadSingleBlock(sector,buf);
	return sta;
}

//дSD��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������	
//����ֵ:����״̬;0,����;����,�������;	
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 sta=SD_OK;
	sta=SD_WriteSingleBlock(sector,buf);
	return sta;
}
