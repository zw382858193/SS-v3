#include "config.h"
#include "device_can.h"
#include "MotorDriver.h"
#include "flash.h"

//#include <stm32f10x.h>

unsigned int alarm_system_tick = 0;
treadmill_def treadmill;
window windows[DEVICESNUM];
#define INIT_TICK 30
void ir_init(void)
{
	GPIO_InitTypeDef ir_config;
//	GPIO_InitTypeDef led_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);// PC6 8 sensor
	ir_config.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_7;
	ir_config.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IPU;
	ir_config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&ir_config);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //LED
//	led_config.GPIO_Pin = GPIO_Pin_1;
//	led_config.GPIO_Mode = GPIO_Mode_Out_PP;
//	led_config.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&led_config);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);

}

uint8_t ret_ir(void)
{
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		return 1;
	}else if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)){
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		return 1;
	}	
//	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)){
//		return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
//	}
	return 0;
}

#if 0
void init(void)
{
	int state = 100,check=0;
	unsigned int tick;
//	uchar *buf;
//	buf=get_uart1_buf();

	#ifdef BLDC
	//id_set_mobus_crc(treadmill.SET_485,0,0x00);
	#else
	id_run_ctrl(treadmill.EN_485,0x00);
	id_set_mobus_crc(treadmill.SET_485,5,0x00);
	#endif
	tick = get_system_tick();
	while((get_system_tick()-tick)<2000);//wait for system power up
	while(1){
//		if(get_uart1_length()>0){
//			if (memcmp(buf, "car_tick=", strlen("car_tick="))==0) {
//				int value;
//				value = strtol((char *)&buf[strlen("car_tick=")],NULL,10);
//				treadmill.rs485_tick=value;
//				debug_out1("succ\r",5);
//			}else if(memcmp(buf, "setid", strlen("setid"))==0){
//				char *q;
//				int value;
//				treadmill.SETID[0] = strtol((char *)&buf[strlen("setid")],&q,10);
//				value = strtol((char *)&q[strlen("=")],NULL,10);
//				set_mobus_crc((uchar *)treadmill.SETID,value);
//				debug_out1("succ\r",5);
//			}rs232_clear1();
//		}
		if(state == 0){
			
			rs232_clear2();
			id_run_ctrl((uchar *)treadmill.STOP,CAR1);
			tick = get_system_tick();
			state = 1;

		}	else if(state == 1){
			if((get_system_tick()-tick) > INIT_TICK){
				if(memcmp(get_uart2_buf(),(uchar *)treadmill.STOP,8)==0){
					check=0;
					state = 2;//2
				}else{
					if(check<3){
						check++;
						state=0;
					}else{
						debug_out1("1\r",2);
						check=0;
						state = 2;
					}
				}
			}
		}else if(state == 2){
			rs232_clear2();
			state = 3;
			id_set_mobus_crc((uchar *)treadmill.SET_SPEED,treadmill.speed,CAR1);
			tick = get_system_tick();

		}else if(state == 3){

			if ((get_system_tick()-tick) > INIT_TICK) {
				
				if(memcmp(get_uart2_buf(),(uchar *)treadmill.SET_SPEED,8)==0){//
					check=0;
					state = 4;  //
				}else{
					if(check<3){
						check++;
						state=2;
					}else{
						debug_out1("3\r",2);
						check=0;
						state = 4;
					}
				}
			}

		} else if(state == 4) {
			rs232_clear2();
			id_set_mobus_crc((uchar *)treadmill.ADTIME_A,treadmill.adtime,CAR1);
			state = 5;
			tick = get_system_tick();

		} else if(state == 5) {
			
			if ((get_system_tick()-tick) > INIT_TICK) {
				if(memcmp(get_uart2_buf(),(uchar *)treadmill.ADTIME_A,8)==0){
					check=0;
					state = 6;  //      state = 6;
				}else{
					if(check<3){
						check++;
						state=4;
					}else{
						debug_out1("5\r",2);
						check=0;
						state = 6;
					}
				}
			}
		} else if(state == 6) {
			rs232_clear2();
			id_set_mobus_crc((uchar *)treadmill.ADTIME_D,treadmill.adtime,CAR1);
			state = 7;
			tick = get_system_tick();

		}else if(state == 7){
					
			if((get_system_tick()-tick) > INIT_TICK) {
				if(memcmp(get_uart2_buf(),(uchar *)treadmill.ADTIME_D,8)==0){
					check=0;
					state = 100;//8
				}else{
					if(check<3){
						check++;
						state=6;
					}else{
						debug_out1("7\r",2);
						check=0;
						state = 100;//8
					}
				}
			}

		} else if (state == 8) { //servor_motor

			rs232_clear4();
			debug_out4((char *)cmd.pos_ctrl.DISABLE,8);
			tick =get_system_tick();
			state = 9;
			
		} else if(state ==9){
			if((get_system_tick()-tick) > INIT_TICK){

				if(memcmp(get_uart4_buf(),(uchar *)&cmd.pos_ctrl.DISABLE,8)==0){//0x02
					check=0;
					state = 10;
				}
				else {
					debug_out1("9\r",3);
					if(check<3){
						check++;
						state=8;
					}else{
						check=0;
						state = 10;
					}
				}
			}

		}else if (state == 10) { //servor_motor

			rs232_clear4();
			debug_out4((char *)cmd.pos_ctrl.POS_MODE_PC,8);//PC CTRL MODE
			tick =get_system_tick();
			state = 11;
			
		} else if(state == 11){
			if((get_system_tick()-tick) > INIT_TICK){

				if(memcmp(get_uart4_buf(),(uchar *)&cmd.pos_ctrl.POS_MODE_PC,8)==0){
					check=0;
					state = 12;
				}else {
					debug_out1("11\r",3);
					if(check<3){
						check++;
						state=10;
					}else{
						check=0;
						state = 12;
					}
				}
			}

		}else if (state == 12){
			rs232_clear4();
			send_servor_motor_data((uchar *)cmd.pos_ctrl.Absolute_position,0x01); //0x00绝对位置模式 0x01相对位置模式
			tick = get_system_tick();
			state = 13;

		}else if(state == 13){
			if((get_system_tick()-tick) > INIT_TICK){
				if(memcmp(get_uart4_buf(),(uchar *)&cmd.pos_ctrl.Absolute_position,8)==0){ 
					check=0;
					state = 14;
				}else{
					debug_out1("13\r",3);
					if(check<3){
						check++;
						state=12;
					}else{
						check=0;
						state = 14;
					}
				}
			}
		}else if(state == 14){
				
			rs232_clear4();
			send_servor_motor_data((uchar *)cmd.pos_ctrl.POS_MODE_SPEED,cmd.sevor_motor_data.servo_motor_speed);
			tick = get_system_tick();
			state = 15;

		}else if(state == 15){
			if((get_system_tick()-tick) > INIT_TICK){
				if(memcmp(get_uart4_buf(),(uchar *)&cmd.pos_ctrl.POS_MODE_SPEED,8)==0){ 
					check=0;
					state = 100;//16
				}else{
					debug_out1("15\r",3);
					if(check<3){
						check++;
						state=14;
					}else{
						check=0;
						state = 100;//16
					}
				}
			}
		} else if (state == 16) { //servor_motor

			rs232_clear4();
			debug_out4((char *)cmd.pos_ctrl.ENABLE,8);
			tick =get_system_tick();
			state = 17;
			
		} else if(state ==17){
			if((get_system_tick()-tick) > INIT_TICK){

				if(memcmp(get_uart4_buf(),(uchar *)&cmd.pos_ctrl.ENABLE,8)==0){
					check=0;
					state = 100;
				}
				else {
					debug_out1("17\r",3);
					if(check<3){
						check++;
						state=16;
					}else{
						check=0;
						state = 100;
					}
				}
			}

		}else if(state == 100){
			treadmill.work_onoff=ON;
			cmd.work_status = WAIT;
			treadmill.work_status=WAIT;
			err_code.car_error=0x10;
			err_code.servo_error=0x80;
			rs232_clear1();
			rs232_clear2();
			rs232_clear4();
			debug_out1("init end\r",9);
			
			break;
		}
	}
}
#endif

void device_init(void)
{
	device.motor_error_code.car_error=0xffff;
	device.motor_error_code.servo_error=0xffff;
	device.heartbeat=0;
}




void my_sprintf_32(char *sbuf,int i)
{
	char buffer[32];
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"%s=%d\r",sbuf,i);
	uart2_sendstr(buffer,strlen(buffer));
}

void test_can(void)
{
	int ret;
	static int send_recv_flag=0;
	uchar CAN_Recv_buf[128];
	u8 CanSendMsg[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	static unsigned int can_send_tick_delay=0;
	if(send_recv_flag == 0){
		ret=Can_Send_Msg(device.id,CanSendMsg,8);
		if(ret==1){//failed
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		}else if(ret==0){//succ
			uart2_sendstr("send\r",5);
			GPIO_ResetBits(GPIOC,GPIO_Pin_6);
			GPIO_SetBits(GPIOC,GPIO_Pin_7);
			send_recv_flag=1;
			can_send_tick_delay=get_system_tick();
		}
	}else{
		if((retTickDiff(can_send_tick_delay))>1000&&(retTickDiff(can_send_tick_delay)<2000)){
			GPIO_SetBits(GPIOC,GPIO_Pin_6);
			ret=Can_Receive_Msg(CAN_Recv_buf);
			if(ret==0){
				GPIO_ResetBits(GPIOC,GPIO_Pin_8);
			}else if(memcmp(CAN_Recv_buf,CanSendMsg,8)==0){
				GPIO_SetBits(GPIOC,GPIO_Pin_8);
				send_recv_flag=0;
			}
		}else if(retTickDiff(can_send_tick_delay)>2000){
			send_recv_flag=0;
			//GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		}
	}
}

void test_can_comm(void)
{
	if(Recv_CAN_Data.Recv_CAN_ID!=0){
		my_sprintf_32("id",Recv_CAN_Data.Recv_CAN_ID);
		my_sprintf_32("car error",Recv_CAN_Data.motor_error_code.car_error);
		my_sprintf_32("servo error",Recv_CAN_Data.motor_error_code.servo_error);
		my_sprintf_32("goodsnumber",Recv_CAN_Data.goodsnumber);
		my_sprintf_32("sensor_position",Recv_CAN_Data.sensor_position[Recv_CAN_Data.Recv_CAN_ID]);
		Recv_CAN_Data.Recv_CAN_ID=0;
	}

}

void rs232_process2(void)//UART2
{
	unsigned int rx2_len;
	unsigned char *buf2;
	int delay=100000;

	rx2_len = get_rs232_len2();
	buf2 = get_rs232_buf2();
	if(rx2_len>0){
		while(delay--);
		if(memcmp(buf2, "input", strlen("input"))==0){
			int value;
			value=strtol((char *)&buf2[strlen("input")],NULL,10);
			if(value>=0){
				int ret;
				u8 CanSendMsg[8]={0x02,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
				CanSendMsg[1]=value;
				ret=Can_Send_Msg(device.id,CanSendMsg,8);
				if(ret){
					uart2_sendstr("send failed\n",strlen("send failed\n"));
				}else{
					uart2_sendstr("send succ\n",strlen("send succ\n"));
				}
			}
		}else if(memcmp(buf2, "car1", strlen("car1"))==0){
			int value;
			value=strtol((char *)&buf2[strlen("car1")],NULL,10);
			if(value>0){
				device.motor_error_code.car_error =value;
			}
		}else if(memcmp(buf2, "car2", strlen("car2"))==0){
			int value;
			value=strtol((char *)&buf2[strlen("car2")],NULL,10);
			if(value>0){
				device.motor_error_code.servo_error =value;
			}
		}rs232_clear2();
	}
}

void CAN_Send_Heartbeat(void)
{
	if(device.heartbeat){
		int ret;
		u8 CanSendMsg[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
		ret=Can_Send_Msg(device.id,CanSendMsg,8);
		if(ret){
			uart2_sendstr("send failed\n",strlen("send failed\n"));
		}else{
			device.heartbeat=0;
			uart2_sendstr("send succ\n",strlen("send succ\n"));
		}
	}
}

int main(void)
{
  //SCB->VTOR = FLASH_BASE | 0x00000000;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
  __enable_irq();	
	SysTick_Config(SystemCoreClock / 1000);
	rs232_uart_init1(9600);

	#ifdef BLDC
 	rs232_uart_init2(19200);//9600
	#else
	rs232_uart_init2(19200);
	#endif

	#ifdef LICHUANG
	rs232_uart_init4(19200);
	#else
	rs232_uart_init4(57600);
	#endif

	treadmill_init();
	ir_init();
	servo_motor_rs485_init();
	flash_init();
	flash_read();
	device_init();
	//init();
	CAN_Mode_Init(BaudRate1Mbps,CAN_Mode_Normal,0,0x0000,0x0000);
	can_recv_buf.Recv_Buf_Len0=0;
	my_sprintf_32("",device.id);
 	while(1) 
	{
		//test_can();
		//test_can_comm();
		CAN_Send_Heartbeat();
		can_comm();
		rs232_process2();
		//if(Can_Receive_Msg())
		rs232_process1();
		//check_car_and_servo(50);
		//servo_motor_run_ctrl_func(30);
		//car_run_ctrl_func(50);
		//ir_win();
 	}
}
