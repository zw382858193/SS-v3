#include "flash.h"

#ifndef _UCHAR_
#define _UCHAR_
#define uchar unsigned char
#endif
#define BLDC
#define LICHUANG
#define DEVICESNUM 128
#define MAX_POS_SET 225000
#define MIN_POS_SET -225000

typedef struct {
	uchar SETID[8];
	uchar STOP[8];
	uchar BACKWARD[8];
	uchar FOREWARD[8];
	uchar SET_SPEED[8];
	uchar ADTIME_A[8];
	uchar ADTIME_D[8];
	uchar R_ADTIME_A[8];
	uchar R_ADTIME_D[8];
	uchar CHECK_XIAOCHE_STATE[8];
	uchar ERR_RESET[8];
	uchar R_SET_SPEED[8];
	uchar R_UI[8];
	uchar BAUDRATE[8];
	uchar SET_485[8];
	uchar EN_485[8];
	uchar BLDC_RUN_MODE_CHANNEL[8];
	uchar BLDC_DIANJI_JIDUISHU[8];
	uchar BLDC_RUN_DIR[8];
	uchar FIRST_XIAOCHE_FOREWARD[8];
	uchar FIRST_XIAOCHE_BACKWARD[8];
	int RUN_TICK;//单个设备运行时间
	int first_deltime;//第一个设备运行减少时间
	int adtime;//加减速
	int next_state_flag;//退出当前运行函数或者跳过当前小车
	unsigned short first_xiaoche_dir;//改变第一个小车的运行方向
	int speed;
	unsigned int loop_i;
	unsigned short run_mode;//函数运行模式改变
	int work_onoff;
	int work_status;
	
}treadmill_def;
extern treadmill_def treadmill;

typedef struct {
	uchar PostionSpeedMode[8];
	uchar LitMode[8];
	uchar ATime[8];
	uchar DTime[8];
	uchar CommCtrl[8];
	uchar PosModeSelect[8];
	uchar PosLoadMode[8];
	uchar DI0Config[8];
	uchar DI1Config[8];
	uchar DI4Config[8];
	uchar DI5Config[8];
	uchar DI6Config[8];
	uchar DI7Config[8];
	uchar CommCtrlDI[8];
	uchar ServoEnableSelection[8];
	uchar PosSpeed[8];
	uchar DisplayMotorSpeed[8];
	uchar SetPos[13];
	uchar RetSetPos[8];
	uchar clean[8];
	uchar check[8];
	uchar enable[8];
	uchar disable[8];
	uchar save[8];
	int speed;
	int adtime;
	int position;
	
	
}LiChuang;
extern LiChuang servo;


typedef struct{
	unsigned short Win;
	unsigned short goodslist;
}window;
extern window windows[DEVICESNUM];

typedef struct {
	unsigned short car_error;
	unsigned short servo_error;

}motor_error_code_stu;


typedef struct{
	unsigned short id;
	unsigned short goodsnumber;
	unsigned short sensor_position[DEVICESNUM];
	short heartbeat;
	motor_error_code_stu motor_error_code;
}devices_stu;
extern devices_stu device;

typedef struct{
	motor_error_code_stu motor_error_code;
	unsigned short Recv_CAN_ID;
	unsigned short goodsnumber;
	unsigned short sensor_position[DEVICESNUM];

}Recv_CAN_Data_Stu;
extern Recv_CAN_Data_Stu Recv_CAN_Data;
