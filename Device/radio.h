#ifndef _RADIO_H_
#define _RADIO_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "usart2.h"

#define RADIO_SEND_HEAD_0				0xA5
#define RADIO_SEND_HEAD_1				0x0C
#define RADIO_SEND_HEAD_CMD			0x0C
#define RADIO_SEND_HEAD_DATA		0xF0
#define RADIO_SEND_HEAD_ACK			0x02
#define RADIO_SEND_HEAD_NOACK		0x12
#define RADIO_SEND_HEAD_ACKDATA	0x03

#define RADIO_PAYLOAD_LEN		0x10

typedef struct RadioAckType{
	u8 ack_num;
	u8 task_cmd;
	u8 task_frame;
	u8 payload[RADIO_PAYLOAD_LEN];
}RadioAckType;

void Radio_Configuration(void);
void Radio_SendData(u8* data, u32 len);

void Radio_SendRecordDataFrameConstruction(u8* data, u32 data_len, u8 *radio_buffer, u32 *buf_len);
void Radio_SendAckFrameConstruction(u8 ack_num, u8 cmd_id, u8 *radio_buffer, u32 *len);
void Radio_SendNoAckFrameConstruction(u8 ack_num, u8 cmd_id, u8 *radio_buffer, u32 *len);
void Radio_SendAckWithDataFrameConstruction(u8 ack_num, u8 cmd_id, u8* data, u32 len, u8 *radio_buffer, u32 *buf_len);


extern void (*Radio_RecvFinishedHandler)(u8 id, u8 ack_num, u32 len, u8* arg);

#endif


