#include "radio.h"

#define RADIO_RECV_BUFFER_LEN		256

void Radio_GetChar(u8 ch);
void Radio_RecvFinished(u8 id, u8 ack_num, u32 len, u8* arg);
void (*Radio_RecvFinishedHandler)(u8 id, u8 ack_num, u32 len, u8* arg)=Radio_RecvFinished;

u8 radio_recv_head_buffer[5];
u8 radio_recv_head_buffer_p=0;
u8 radio_recv_state=0;
u8 radio_recv_len=0;
u8 radio_recv_buffer[RADIO_RECV_BUFFER_LEN];
u8 radio_recv_p=0;

void Radio_Configuration(void)
{
	USART2_Configuration();
	USART2_Recv=Radio_GetChar;
}

void Radio_GetChar(u8 ch)
{
	if(radio_recv_head_buffer_p<5 && radio_recv_head_buffer_p>0){
		radio_recv_head_buffer[radio_recv_head_buffer_p++]=ch;
	}
	
	if(ch==RADIO_SEND_HEAD_0){
		radio_recv_head_buffer_p=0;
		radio_recv_head_buffer[radio_recv_head_buffer_p++]=ch;
	}
	
	if(radio_recv_head_buffer_p==5){
		if( radio_recv_head_buffer[0]==RADIO_SEND_HEAD_0 && 
			  radio_recv_head_buffer[1]==RADIO_SEND_HEAD_1 && (
			  radio_recv_head_buffer[2]==RADIO_SEND_HEAD_CMD)){
			radio_recv_head_buffer_p=0;
			radio_recv_len=radio_recv_head_buffer[4];
			radio_recv_p=0;
			radio_recv_state=1;
			return;
		}
	}
	
	if(radio_recv_state==1){
		radio_recv_buffer[radio_recv_p++]=ch;
		if(radio_recv_len==radio_recv_p){
			radio_recv_state=0;
			Radio_RecvFinishedHandler(radio_recv_buffer[1], radio_recv_buffer[0], radio_recv_len-2,radio_recv_buffer+2);
			radio_recv_len=0;
		}
	}
}

void Radio_RecvFinished(u8 id, u8 ack_num, u32 len, u8* arg){}

void Radio_SendData(u8* data, u32 len)
{
	if(len>0 && len<4096){
		USART2_DMASendWords(data,len);
	}
}

void Radio_SendRecordDataFrameConstruction(u8* data, u32 data_len, u8 *radio_buffer, u32 *buf_len)
{
	u32 i;
	u8 buf[10];
	u8 b_len=5;
	buf[0]=RADIO_SEND_HEAD_0;
	buf[1]=RADIO_SEND_HEAD_1;
	buf[2]=RADIO_SEND_HEAD_DATA;
	buf[3]=(data_len)>>8;
	buf[4]=(data_len);
	for(i=0;i<b_len;i++){
		radio_buffer[i]=buf[i];
	}
	for(i=0;i<data_len;i++){
		radio_buffer[i+b_len]=data[i];
	}
	*buf_len+=data_len+b_len;
}

void Radio_SendAckFrameConstruction(u8 ack_num, u8 cmd_id, u8 *radio_buffer, u32 *len)
{
	u8 i;
	u8 buf[7];
	u8 b_len=7;
	buf[0]=RADIO_SEND_HEAD_0;
	buf[1]=RADIO_SEND_HEAD_1;
	buf[2]=RADIO_SEND_HEAD_ACK;
	buf[3]=0;
	buf[4]=2;
	buf[5]=ack_num;
	buf[6]=cmd_id;
	for(i=0;i<b_len;i++){
		radio_buffer[i]=buf[i];
	}
	*len+=b_len;
}

void Radio_SendNoAckFrameConstruction(u8 ack_num, u8 cmd_id, u8 *radio_buffer, u32 *len)
{
	u8 i;
	u8 buf[10];
	u8 b_len=7;
	buf[0]=RADIO_SEND_HEAD_0;
	buf[1]=RADIO_SEND_HEAD_1;
	buf[2]=RADIO_SEND_HEAD_NOACK;
	buf[3]=0;
	buf[4]=2;
	buf[5]=ack_num;
	buf[6]=cmd_id;
	for(i=0;i<b_len;i++){
		radio_buffer[i]=buf[i];
	}
	*len+=b_len;
}

void Radio_SendAckWithDataFrameConstruction(u8 ack_num, u8 cmd_id, u8* data, u32 len, u8 *radio_buffer, u32 *buf_len)
{
	u32 i;
	u8 b_len=7;
	u8 buf[10];

	buf[0]=RADIO_SEND_HEAD_0;
	buf[1]=RADIO_SEND_HEAD_1;
	buf[2]=RADIO_SEND_HEAD_ACKDATA;
	buf[3]=0;
	buf[4]=len+2;
	buf[5]=ack_num;
	buf[6]=cmd_id;
	for(i=0;i<b_len;i++){
		radio_buffer[i]=buf[i];
	}
	for(i=0;i<len;i++){
		radio_buffer[i+b_len]=data[i];
	}
	*buf_len+=b_len+len;
}
