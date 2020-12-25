#include "device_can.h"
#include "uart.h"
#include "config.h"
#include "MotorDriver.h"

void can_comm(void)
{
	int i;
	unsigned char send_modbus_buf[8];
	uint16_t data_tmp;
	if(can_recv_buf.Recv_Buf_Len0==CAN_RECV_BUF_LEN){
		switch(can_recv_buf.CAN_Recv_Buf0[1]){//协议功能
			case 0x01://win cmd
				if(device.id<can_recv_buf.CAN_Recv_Buf0[0]){
					device.goodsnumber++;
				}else if(device.id==can_recv_buf.CAN_Recv_Buf0[0]){
					for(i=0;i<DEVICESNUM;i++){
						if(windows[i].Win==0){
							windows[i].Win=can_recv_buf.CAN_Recv_Buf0[2];
							windows[i].goodslist=device.goodsnumber;
							break;
						}
					}
				}
				break;
			case 0x02://heartbeat cmd
				if(device.id==can_recv_buf.CAN_Recv_Buf0[0]){
					device.heartbeat=1;
				}
				break;
			case 0x03://send read modbus
				
				if(device.id == can_recv_buf.CAN_Recv_Buf0[0]){
					if(can_recv_buf.CAN_Recv_Buf0[2]==0x1){//car1
						//send
						send_modbus_buf[0]=0x01;
						send_modbus_buf[1]=0x03;
						send_modbus_buf[2]=can_recv_buf.CAN_Recv_Buf0[3];
						send_modbus_buf[3]=can_recv_buf.CAN_Recv_Buf0[4];
						data_tmp=(can_recv_buf.CAN_Recv_Buf0[5]<<8)|can_recv_buf.CAN_Recv_Buf0[6];
						set_mobus_crc(send_modbus_buf,data_tmp);
					}else if(can_recv_buf.CAN_Recv_Buf0[2]==0x2){//car2 servo motor
						send_modbus_buf[0]=0x01;
						send_modbus_buf[1]=0x03;
						send_modbus_buf[2]=can_recv_buf.CAN_Recv_Buf0[3];
						send_modbus_buf[3]=can_recv_buf.CAN_Recv_Buf0[4];
						data_tmp=(can_recv_buf.CAN_Recv_Buf0[5]<<8)|can_recv_buf.CAN_Recv_Buf0[6];
						set_mobus_crc(send_modbus_buf,data_tmp);
					}
				}
				break;
			case 0x06://send write modbus
				
				if(device.id == can_recv_buf.CAN_Recv_Buf0[0]){
					if(can_recv_buf.CAN_Recv_Buf0[2]==0x1){//car1
						//send
						send_modbus_buf[0]=0x01;
						send_modbus_buf[1]=0x06;
						send_modbus_buf[2]=can_recv_buf.CAN_Recv_Buf0[3];
						send_modbus_buf[3]=can_recv_buf.CAN_Recv_Buf0[4];
						data_tmp=(can_recv_buf.CAN_Recv_Buf0[5]<<8)|can_recv_buf.CAN_Recv_Buf0[6];
						set_mobus_crc(send_modbus_buf,data_tmp);

					}else if(can_recv_buf.CAN_Recv_Buf0[2]==0x2){//car2 servo motor
						send_modbus_buf[0]=0x01;
						send_modbus_buf[1]=0x06;
						send_modbus_buf[2]=can_recv_buf.CAN_Recv_Buf0[3];
						send_modbus_buf[3]=can_recv_buf.CAN_Recv_Buf0[4];
						data_tmp=(can_recv_buf.CAN_Recv_Buf0[5]<<8)|can_recv_buf.CAN_Recv_Buf0[6];
						set_mobus_crc(send_modbus_buf,data_tmp);
					}
				}
				break;
			default:break;
			
		}can_recv_buf.Recv_Buf_Len0=0;
	}
}
