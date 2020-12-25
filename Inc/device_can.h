#include <stm32f10x.h>

typedef enum
{
	BaudRate5Kbps = 0,
	BaudRate10Kbps,
	BaudRate20Kbps,
	BaudRate25Kbps,
	BaudRate40Kbps,
	BaudRate50Kbps,
	BaudRate80Kbps,
	BaudRate100Kbps,
	BaudRate125Kbps,
	BaudRate200Kbps,
	BaudRate250Kbps,
	BaudRate400Kbps,
	BaudRate500Kbps,
	BaudRate800Kbps,
	BaudRate1Mbps
}CANBaudRate;

#define CAN_RECV_BUF_LEN 8
typedef struct
{
	u8 CAN_Recv_Buf0[CAN_RECV_BUF_LEN];
	u8 CAN_Recv_Buf1[CAN_RECV_BUF_LEN];
	u8 Recv_Buf_Len0;
	u8 Recv_Buf_Len1;
}can_recv_buf_stu;
extern can_recv_buf_stu can_recv_buf;

u8 CAN_Mode_Init(CANBaudRate BaudRate,u8 mode,unsigned int FilterNumber,unsigned int FilterId,unsigned int FilterMaskId);
u8 Can_Receive_Msg(u8 *buf);
u8 Can_Send_Msg(unsigned int id,u8* msg,u8 len);
void can_comm(void);
