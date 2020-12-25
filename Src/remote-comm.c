#include "config.h"
#include "MotorDriver.h"
#include "device_can.h"
unsigned short uart2_485_delay;
// 232 communication.

//void servo_motorconnectfailed_start(void);
//void servo_motorconnectfailed_stop(void);
//void treadmill5_cmd(int win);
//void treadmill2_cmd(int win);
//unsigned int delay_break;
//unsigned int tick;
devices_stu device;
LiChuang servo;
char VERSION[]={"version v1.1"};

void servo_motor_rs485_init(void)
{
	//#ifdef DAYONE
//		uchar ENABLE[8] = {0x01,0x06,0x00,0x00,0x00,0x01,0x48,0x0a};
//		uchar DISABLE[8] = {0x01,0x06,0x00,0x00,0x00,0x00,0x89,0xca};
//		uchar POS_MODE_PC[8] = {0x01,0x06,0x00,0x02,0x00,0xd0,0x29,0x96};
//		uchar Absolute_position[8] = {0x01,0x06,0x00,0x51,0x00,0x00,0xd8,0x1b};//绝对位置
//		uchar POS_MODE_H16[4] = {0x01,0x06,0x50,0x00};
//		uchar POS_MODE_L16[4] = {0x01,0x06,0x05,0x00};
//		uchar POS_SET[7] = {0x01,0x10,0x00,0x50,0x00,0x02,0x04};
//		uchar POS_SET_RET[8] = {0x01,0x10,0x00,0x50,0x00,0x02,0x41,0xd9};
//		uchar POS_MODE_SPEED[4] = {0x01,0x06,0x00,0x1d};
//		uchar READ_DATA_POS_BACK_H16[8] = {0x01,0x03,0x00,0xe8,0x00,0x01,0x04,0x3e};
//		uchar READ_DATA_POS_BACK_L16[8] = {0x01,0x03,0x00,0xe9,0x00,0x01,0x55,0xfe};
//		uchar READ_DATA_POS_BACK[8] = {0x01,0x03,0x00,0xe8,0x00,0x02,0x44,0x3f};
//		uchar CHECK_I[8] = {0x01,0x03,0x00,0xe2,0x00,0x01,0x24,0x3c};
//		uchar CHECK_STATUS[6] = {0x01,0x03,0x00,0xe3,0x00,0x01};//0xe3
//		uchar POS_MODE_ADTIME[6]={0x01,0x06,0x00,0x09,0x00,0x00};//位置模式的加减速时间
//		uchar Z_POS[6] = {0x01,0x06,0x00,0x53,0x00,0x00};
//		
//		memcpy(cmd.pos_ctrl.ENABLE,ENABLE,8);
//		memcpy(cmd.pos_ctrl.DISABLE,DISABLE,8);
//		memcpy(cmd.pos_ctrl.POS_MODE_PC,POS_MODE_PC,8);
//		memcpy(cmd.pos_ctrl.Absolute_position,Absolute_position,8);
//		memcpy(cmd.pos_ctrl.POS_MODE_H16,POS_MODE_H16,4);
//		memcpy(cmd.pos_ctrl.POS_MODE_L16,POS_MODE_L16,4);
//		memcpy(cmd.pos_ctrl.POS_MODE_SPEED,POS_MODE_SPEED,4);
//		memcpy(cmd.pos_ctrl.READ_DATA_POS_BACK_H16,READ_DATA_POS_BACK_H16,8);
//		memcpy(cmd.pos_ctrl.READ_DATA_POS_BACK_L16,READ_DATA_POS_BACK_L16,8);
//		memcpy(cmd.pos_ctrl.CHECK_I,CHECK_I,8);
//		memcpy(cmd.pos_ctrl.CHECK_STATUS,CHECK_STATUS,6);	
//		memcpy(cmd.pos_ctrl.POS_SET,POS_SET,7);
//		memcpy(cmd.pos_ctrl.POS_SET_RET,POS_SET_RET,8);
//		memcpy(cmd.pos_ctrl.READ_DATA_POS_BACK,READ_DATA_POS_BACK,8);
//		
//		memcpy(cmd.pos_ctrl.POS_MODE_ADTIME,POS_MODE_ADTIME,8);
//		memcpy(cmd.pos_ctrl.Z_POS,Z_POS,8);
	//#endif
	//#ifdef LICHUANG
		uchar SetPos[13] = {0x01,0x10,0x01,0x68,0x00,0x02,0x04,0x86,0xa0,0x00,0x01,0x11,0x1b};//位置指令
		uchar DisplayMotorSpeed[8] = {0x01,0x06,0x00,0x01,0x00,0x01,0x19,0xca};//显示速度
		uchar PosSpeed[8] = {0x01,0x06,0x01,0x90,0x01,0xf4,0x88,0x0c};//第0位置模式速度
		uchar RetSetPos[8] = {0x01,0x10,0x01,0x68,0x00,0x02,0xC1,0xE8};//绝对位置
		uchar PostionSpeedMode[8] = {0x01,0x06,0x00,0x02,0x00,0x03,0x68,0x0b};//位置速度模式切换
		uchar LitMode[8] = {0x01,0x06,0x00,0x04,0x00,0x00,0xc8,0x0b};//限位模式选择
		uchar ATime[8] = {0x01,0x10,0x00,0x50,0x00,0x02,0x04};//加速时间
		uchar DTime[8] = {0x01,0x10,0x00,0x50,0x00,0x02,0x41,0xd9};//减速时间
		uchar CommCtrl[8] = {0x01,0x06,0x00,0x90,0x00,0x01,0x48,0x27};//扩展模式 通讯控制
		uchar PosModeSelect[8] ={0x01,0x06,0x00,0x94,0x00,0x01,0x09,0xe6};//位置模式选择  相对位置模式
//		uchar PosLoadMode[8] ={0x01,0x06,0x00,0x96,0x00,0x02,0xe8,0x27};//位置模式装置模式 上升沿装载
		uchar Load[8] ={0x01,0x06,0x00,0x96,0x00,0x00,0x69,0xe6};//位置模式装载模式 一直装载
		uchar DI0Config[8] ={0x01,0x06,0x00,0x80,0x00,0x00,0x98,0x2c};
		uchar DI1Config[8] ={0x01,0x06,0x00,0x81,0x00,0x01,0x98,0x2c};
		uchar DI4Config[8] ={0x01,0x06,0x00,0x84,0x00,0x04,0xc8,0x20};//DI4控制模式切换
		uchar DI5Config[8] ={0x01,0x06,0x00,0x85,0x00,0x14,0x98,0x2c};//DI5位置装载信号
		uchar DI6Config[8] ={0x01,0x06,0x00,0x86,0x00,0x11,0xa8,0x2f};//DI6原点开关
		uchar DI7Config[8] ={0x01,0x06,0x00,0x87,0x00,0x10,0x38,0x2f};//DI7回零启动
		uchar CommCtrlDI[8]={0x01,0x06,0x01,0xa0,0x00,0xff,0xc8,0x54};//通讯控制DI口
		uchar ServoEnableSelection[8]={0x01,0x06,0x00,0x8f,0x00,0x01,0x79,0xe1};//伺服使能选择 自动使能
		uchar clean[8]={0x01,0x06,0x01,0xa4,0x00,0x02,0x48,0x14};//清除报警
		uchar check[8]={0x01,0x03,0x04,0x78,0x00,0x01,0x05,0x23};//检查设备
		uchar servoEnable[8]={0x01,0x06,0x01,0xa4,0x00,0x01,0x08,0x15};//使能
		uchar servoDisable[8]={0x01,0x06,0x01,0xa4,0x00,0x00,0xc9,0xd5};//失能
		uchar save[8] ={0x01,0x06,0x01,0xa7,0x08,0x01,0xff,0xd5};//保存参数
		
		memcpy(servo.SetPos,SetPos,13);
		memcpy(servo.DisplayMotorSpeed,DisplayMotorSpeed,8);
		memcpy(servo.PosSpeed,PosSpeed,8);
		memcpy(servo.RetSetPos,RetSetPos,8);
		memcpy(servo.PostionSpeedMode,PostionSpeedMode,8);
		memcpy(servo.LitMode,LitMode,8);
		memcpy(servo.ATime,ATime,8);
		memcpy(servo.DTime,DTime,8);
		memcpy(servo.CommCtrl,CommCtrl,8);
		memcpy(servo.PosModeSelect,PosModeSelect,8);
		memcpy(servo.PosLoadMode,Load,8);	
		memcpy(servo.DI0Config,DI0Config,8);
		memcpy(servo.DI1Config,DI1Config,8);
		memcpy(servo.DI4Config,DI4Config,8);
		memcpy(servo.DI5Config,DI5Config,8);
		memcpy(servo.DI6Config,DI6Config,8);
		memcpy(servo.DI7Config,DI7Config,8);
		memcpy(servo.CommCtrlDI,CommCtrlDI,8);
		memcpy(servo.ServoEnableSelection,ServoEnableSelection,8);
		memcpy(servo.clean,clean,8);
		memcpy(servo.check,check,8);
		memcpy(servo.enable,servoEnable,8);
		memcpy(servo.disable,servoDisable,8);
		memcpy(servo.save,save,8);
	//#endif
}

void initServoLiChuang(void)
{
	int check=0,state=0;
	unsigned int init_tick=0;
	while(1){
		if(state==0){
			rs232_clear2();
			debug_out2((char *)servo.PostionSpeedMode,8);
			init_tick=get_system_tick();
			state=1;
		}else if(state==1){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.PostionSpeedMode,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==2){
			rs232_clear2();
			debug_out2((char *)servo.LitMode,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==3){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.LitMode,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==4){
			rs232_clear2();
			set_mobus_crc(servo.ATime,servo.adtime);
			init_tick=get_system_tick();
			state++;
		}else if(state==5){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.ATime,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==6){
			rs232_clear2();
			set_mobus_crc(servo.DTime,servo.adtime);
			init_tick=get_system_tick();
			state++;
		}else if(state==7){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.DTime,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==8){
			rs232_clear2();
			debug_out2((char *)servo.CommCtrl,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==9){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.CommCtrl,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==10){
			rs232_clear2();
			debug_out2((char *)servo.PosModeSelect,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==11){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.PosModeSelect,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==12){
			rs232_clear2();
			debug_out2((char *)servo.PosLoadMode,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==13){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.PosLoadMode,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==14){
			rs232_clear2();
			debug_out2((char *)servo.DI4Config,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==15){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.DI4Config,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==16){
			rs232_clear2();
			debug_out2((char *)servo.DI5Config,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==17){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.DI5Config,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==18){
			rs232_clear2();
			debug_out2((char *)servo.DI6Config,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==19){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.DI6Config,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==20){
			rs232_clear2();
			debug_out2((char *)servo.DI7Config,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==21){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.DI7Config,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==22){
			rs232_clear2();
			debug_out2((char *)servo.ServoEnableSelection,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==23){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.ServoEnableSelection,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==24){
			rs232_clear2();
			debug_out2((char *)servo.CommCtrlDI,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==25){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.CommCtrlDI,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==26){
			rs232_clear2();
			debug_out2((char *)servo.PosSpeed,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==27){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.PosSpeed,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==28){
			rs232_clear2();
			send_servor_motor_pos(servo.SetPos,0);
			init_tick=get_system_tick();
			state++;
		}else if(state==29){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.RetSetPos,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==30){
			rs232_clear2();
			debug_out2((char *)servo.save,8);
			init_tick=get_system_tick();
			state++;
		}else if(state==31){
			if(retTickDiff(init_tick)>50){
				if(memcmp(get_uart2_buf(),servo.save,8)){
					state++;
					check=0;
				}else{
					if(check<3){
						check++;
						state--;
					}else{
						state++;
						check=0;
					}
				}
			}
		}else if(state==32){
			break;
		}
	}
}

void treadmill_init(void)
{
	uchar SETID[4] = {0x00,0x06,0x20,0x08};
	uchar STOP[6] = {0x01,0x06,0x20,0x00,0x00,0x05};
	uchar FOREWARD[6] = {0x01,0x06,0x20,0x00,0x00,0x01};
	uchar BACKWARD[6] = {0x01,0x06,0x20,0x00,0x00,0x02};
	uchar SET_SPEED[6] = {0x01,0x06,0x20,0x01,0x05,0xDC};
	uchar ADTIME_A[4] = {0x01,0x06,0x20,0x03};
	uchar ADTIME_D[4] = {0x01,0x06,0x20,0x04};
	uchar R_ADTIME_A[6] = {0x01,0x03,0x20,0x03,0x00,0x01};
	uchar R_ADTIME_D[6] = {0x01,0x03,0x20,0x04,0x00,0x01};
	uchar ERR_RESET[6] = {0x01,0x06,0x20,0x00,0x00,0x07};
	uchar BAUDRATE[6] = {0x01,0x06,0x20,0x09,0x00,0x03};
	#ifdef BLDC
		uchar BLDC_CHECK_XIAOCHE_STATE[6] = {0x01,0x03,0x21,0x04,0x00,0x01};
		uchar BLDC_R_SET_SPEED[6] = {0x01,0x03,0x20,0x01,0x00,0x01};
		uchar BLDC_SET_485[6] = {0x00,0x06,0x20,0x0b,0x00,0x05};
		uchar BLDC_DIANJI_JIDUISHU[6] = {0x00,0x06,0x20,0x02,0x00,0x02};
		uchar BLDC_RUN_MODE_CHANNEL[6] = {0x00,0x06,0x20,0x06,0x00,0x02};
		uchar BLDC_RUN_DIR[6] = {0x00,0x06,0x20,0x0A,0x00,0x01};
	#else
		uchar EN_485[6] = {0x01,0x06,0x20,0x0E,0x00,0x01};
		uchar SET_485[6] = {0x01,0x06,0x08,0x03,0x00,0x05};
		uchar CHECK_XIAOCHE_STATE[6] = {0x00,0x03,0x21,0x02,0x00,0x01};
		uchar R_SET_SPEED[8] = {0x01,0x03,0x30,0x05,0x00,0x01,0xB9,0x0B};
		uchar R_UI[6] = {0x01,0x03,0x30,0x07,0x00,0x01};
	#endif
	
	memcpy(treadmill.SETID,SETID,4);
	memcpy(treadmill.STOP,STOP,6);
	memcpy(treadmill.BACKWARD,BACKWARD,6);
	memcpy(treadmill.FOREWARD,FOREWARD,6);
	memcpy(treadmill.SET_SPEED,SET_SPEED,6);
	memcpy(treadmill.ADTIME_A,ADTIME_A,4);
	memcpy(treadmill.ADTIME_D,ADTIME_D,4);
	memcpy(treadmill.R_ADTIME_A,R_ADTIME_A,6);
	memcpy(treadmill.R_ADTIME_D,R_ADTIME_D,6);
	#ifdef BLDC
		memcpy(treadmill.BLDC_RUN_DIR,BLDC_RUN_DIR,6);
		memcpy(treadmill.BLDC_DIANJI_JIDUISHU,BLDC_DIANJI_JIDUISHU,6);
		memcpy(treadmill.BLDC_RUN_MODE_CHANNEL,BLDC_RUN_MODE_CHANNEL,6);
		memcpy(treadmill.CHECK_XIAOCHE_STATE,BLDC_CHECK_XIAOCHE_STATE,6);
		memcpy(treadmill.SET_485,BLDC_SET_485,6);
		memcpy(treadmill.R_SET_SPEED,BLDC_R_SET_SPEED,6);
	#else
		memcpy(treadmill.CHECK_XIAOCHE_STATE,CHECK_XIAOCHE_STATE,6);
		memcpy(treadmill.EN_485,EN_485,6);
		memcpy(treadmill.SET_485,SET_485,6);
		memcpy(treadmill.R_SET_SPEED,R_SET_SPEED,6);
		memcpy(treadmill.R_UI,R_UI,6);
	#endif
	memcpy(treadmill.ERR_RESET,ERR_RESET,6);	
	memcpy(treadmill.BAUDRATE,BAUDRATE,6);
	treadmill.work_onoff=ON;
}



#if 0
void check_car_and_servo(unsigned int tick_delay)
{
	static int state = 0,check=0;
	static unsigned int check_tick = 0;
	static uchar *p2;
	
	p2=get_rs232_buf2();
//	p4=get_rs232_buf4();

	if((cmd.work_status==WAIT)&&(treadmill.work_status==WAIT)){
		if(state == 0){
			rs232_clear2();
			debug_out2((char *)servo.check,8);
			check_tick = get_system_tick();
			state = 1;
		}else if(state == 1){
			if((get_system_tick()-check_tick)>tick_delay){
				if(memcmp(get_rs232_buf2(),servo.check,2) == 0){
					err_code.car_error=(p2[3]<<8)|p2[4];
					state++;
				}else{
					if(check<3){
						state--;
						check++;
					}else{
						err_code.car_error=0xffff;
						state++;
					}
				}
				state = 3;
			}
		}
		else if(state == 3){
//			rs232_clear4();
//			send_servor_motor_data(cmd.pos_ctrl.CHECK_STATUS,0x01);
//			check_tick = get_system_tick();
			state = 0;
		}//else if(state == 4){
//			if((get_system_tick()-check_tick)>tick_delay){
//				if(get_rs232_len4() >= 7){
//					err_code.servo_error=p4[4];
//				}else {
//					err_code.servo_error=0x80;
//				}
//				state = 0;
//			}
//		}
	}else{
		state=0;
	}
}
#endif

void rs232_process1(void)//UART1
{
	unsigned int rx1_len;
	unsigned char *buf;
	int value;

	rx1_len = get_rs232_len1();

	if (rx1_len > 0) {
		buf = get_rs232_buf1();
		if (memcmp(buf, "status_work", strlen("status_work"))==0) {
			unsigned int status_work;
			char sbuf[32];
				status_work = device.motor_error_code.car_error;
				if((status_work ^ 1)){
					debug_out1("Stop\r\n",strlen("Stop\r\n"));
				}if(!status_work ^ 1){
					debug_out1("Start\r\n",strlen("Start\r\n"));
				}if(!(status_work ^ 2)){
					debug_out1("state_ov_i\r\n",strlen("state_ov_i\r\n"));
				}if(!(status_work ^ 3)){
					debug_out1("state_ov_u\r\n",strlen("state_ov_u\r\n"));
				}if(!(status_work ^ 4)){
					debug_out1("state_err_enc\r\n",strlen("state_err_enc\r\n"));
				}if(!(status_work ^ 5)){
					debug_out1("state_ov_t\r\n",strlen("state_ov_t\r\n"));
				}if(!(status_work ^ 6)){
					debug_out1("state_ov_q\r\n",strlen("state_ov_q\r\n"));
				}if(!(status_work ^ 7)){
					debug_out1("state_ov_load\r\n",strlen("state_ov_load\r\n"));
				}if((status_work ^ 8)){
					debug_out1("pc ctrl\r\n",strlen("pc ctrl\r\n"));
				}if(!(status_work ^ 8)){
					debug_out1("PLC ctrl\r\n",strlen("PLC ctrl\r\n"));
				}

					switch(device.motor_error_code.car_error){
						case 0x00:break;
						case 0x01:sprintf(sbuf,"over current\r");debug_out1(sbuf,strlen(sbuf));break;//过流
						case 0x02:sprintf(sbuf,"over voltage\r");debug_out1(sbuf,strlen(sbuf));break;//过压
						case 0x03:sprintf(sbuf,"under voltage\r");debug_out1(sbuf,strlen(sbuf));break;//欠压
						case 0x04:sprintf(sbuf,"locked rotor\r");debug_out1(sbuf,strlen(sbuf));break;//堵转
						case 0x05:sprintf(sbuf,"holzer fault\r");debug_out1(sbuf,strlen(sbuf));break;//霍尔异常
						case 0x06:sprintf(sbuf,"too hot\r");debug_out1(sbuf,strlen(sbuf));break;//过热
						default:debug_out1("unkown err\r",11);break;
					}
					sprintf(sbuf,"car_error:%d,servo_error:%d\r",device.motor_error_code.car_error,device.motor_error_code.servo_error);debug_out1(sbuf,strlen(sbuf));

		}else if (memcmp(buf, "win", strlen("win"))==0) {
			int value;
			value=strtol((char *)&buf[strlen("win")],NULL,10);
			if(value>0){
				int i;
				int ret;
				u8 CanSendMsg[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
				for(i=0;i<DEVICESNUM;i++){
					if(windows[i].Win==0){
						//can send
						windows[i].Win=((value+1)/2)+1;
						CanSendMsg[0]=windows[i].Win;
						CanSendMsg[1]=value;
						ret=Can_Send_Msg(device.id,CanSendMsg,8);
						if(ret == 0){
							debug_out2("send failed\n",strlen("send failed\n"));
						}
						windows[i].Win=value;
//						windows[i].goodspoition=0;
						if(device.goodsnumber<i){
							device.goodsnumber=i;
						}
						break;
					}
				}
			}
			debug_out1("win\r",4);
		}else if (memcmp(buf, "addr", strlen("addr"))==0) {
			int value;
			char *q;
			uchar str[8]={0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00};
			value = strtol((char *)&buf[strlen("addr")],&q,10);
			set_addr_mobus_crc(str,value,strtol((&q[strlen("data")]),NULL,10));
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "device_id", strlen("device_id"))==0) {
//			int value;
//			char *q;
//			value = strtol((char *)&buf[strlen("device_id")],&q,10);
//			if(value==device.id){
//				device.id=strtol((char *)&q[strlen("=")],NULL,10);
//			}
//			WriteFlashOneWord(FLASH_DEVICE_ID_ADDR,device.id);
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "work_off", strlen("work_off"))==0) {
			treadmill.work_onoff=OFF;
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "work_on", strlen("work_on"))==0) {
			treadmill.work_onoff=ON;
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "start", strlen("start"))==0) {
			debug_out2((char *)servo.enable,8);
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "stop", strlen("stop"))==0) {
			debug_out2((char *)servo.disable,8);
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "pos_set=", strlen("pos_set="))==0) {
			char *q;
			servo.position = strtol((char *)&buf[strlen("pos_set=")],&q,10);
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "pos_speed=", strlen("pos_speed="))==0){
			servo.speed = strtol((char *)&buf[strlen("pos_speed=")],NULL,10);
			
			send_servor_motor_data(servo.PosSpeed,value);
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "pos_adtime=", strlen("pos_adtime="))==0){
			servo.adtime = strtol((char *)&buf[strlen("pos_adtime=")],NULL,10);
			set_mobus_crc(servo.ATime,servo.adtime);
			set_mobus_crc(servo.DTime,servo.adtime);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "setid", strlen("setid"))==0){
			char *q;
			treadmill.SETID[0] = strtol((char *)&buf[strlen("setid")],&q,10);
			value = strtol((char *)&q[strlen("=")],NULL,10);
			set_mobus_crc((uchar *)treadmill.SETID,value);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_speed=", strlen("car_speed="))==0){
			value = strtol((char *)&buf[strlen("car_speed=")],NULL,10);
			treadmill.speed = value;
			id_set_mobus_crc((uchar *)treadmill.SET_SPEED,value,CAR1);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_adtime=", strlen("car_adtime="))==0){
			value = strtol((char *)&buf[strlen("car_adtime=")],NULL,10);
			treadmill.adtime=value;
			id_set_mobus_crc(treadmill.ADTIME_A,value,0x01);
			id_set_mobus_crc(treadmill.ADTIME_D,value,0x01);
			debug_out1("succ\r",5);
		}else if (memcmp(buf, "car_tick=", strlen("car_tick="))==0) {
				int value;
				value = strtol((char *)&buf[strlen("car_tick=")],NULL,10);
				uart2_485_delay=value;
				debug_out1("succ\r",5);
			}else if(memcmp(buf, "car_f", strlen("car_f"))==0){

			treadmill.work_status=FOREWARD;
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_b", strlen("car_b"))==0){

			treadmill.work_status=BACKWARD;
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_s", strlen("car_s"))==0){

			treadmill.work_status=STOP;
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "version", strlen("version"))==0){
			debug_out1(VERSION,strlen(VERSION));
			
		}else if(memcmp(buf, "read_flash", strlen("read_flash"))==0){
			flash_read();

		}else if(memcmp(buf, "save", strlen("save"))==0){
			WriteFlashOneWord(FLASH_CAR_SPEED_DATA_ADDR,treadmill.speed);
			WriteFlashOneWord(FLASH_CAR_ADTIME_DATA_ADDR,treadmill.adtime);
			WriteFlashOneWord(FLASH_CAR_RS485_TICK_ADDR,uart2_485_delay);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_other_speed=", strlen("car_other_speed="))==0){
			value = strtol((char *)&buf[strlen("car_other_speed=")],NULL,10);
			id_set_mobus_crc((uchar *)treadmill.SET_SPEED,value,0x02);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_other_f", strlen("car_other_f"))==0){

			id_run_ctrl(treadmill.FOREWARD,0x02);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_other_b", strlen("car_other_b"))==0){

			id_run_ctrl(treadmill.BACKWARD,0x02);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "car_other_s", strlen("car_other_s"))==0){

			id_run_ctrl(treadmill.STOP,0x02);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "set_id", strlen("set_id"))==0){
			int value;
			char *q;
			value =  strtol((char *)&buf[strlen("set_id")],&q,10);
			treadmill.SETID[0] = value;
			set_mobus_crc((uchar *)treadmill.SETID,strtol((char *)&q[strlen("=")],NULL,10));
			//set_mobus_crc((uchar *)treadmill.SETID,strtol((char *)&q[strlen("=")],NULL,10),value);
		}else if(memcmp(buf, "clean", strlen("clean"))==0){

			debug_out2((char *)servo.clean,8);
			debug_out1("succ\r",5);
		}else if(memcmp(buf, "initServo", strlen("initServo"))==0){
			initServoLiChuang();
		}else if(memcmp(buf, "reset", strlen("reset"))==0){
			reset();
		}else {
			debug_out1("unknow cmd",strlen("unknow cmd"));
		}
		rs232_clear1();
	}
}

