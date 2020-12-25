#include "config.h"
#include "flash.h"
#include "MotorDriver.h"
#include <time.h>

void flash_servo_motor_position_data_get(void);
void flash_servo_motor_adtime_data_get(void);
void flash_servo_motor_speed_data_get(void);

volatile FLASH_Status FLASHStatus = FLASH_BUSY;
#define STARTADDR 0x0800F000
#define FLASH_BUF_LEN 16
void WriteFlashOneWord(uint32_t WriteAddress, uint32_t WriteData)
{
	uint32_t flash_buf[FLASH_BUF_LEN];
	int i;
	for(i=0;i<FLASH_BUF_LEN;i++){
		flash_buf[i] = *(__IO uint32_t*)(STARTADDR + ((i+1)*0x04));
		if(WriteAddress == ((i+1)*0x04)){
			flash_buf[i] = WriteData;
		}
	}
	
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_BUSY;
	FLASHStatus = FLASH_ErasePage(STARTADDR);
	if(FLASHStatus == FLASH_COMPLETE)
	{
		FLASHStatus = FLASH_BUSY;
		for(i=0;i<FLASH_BUF_LEN;i++)
			FLASHStatus = FLASH_ProgramWord(STARTADDR+((i+1)*0x04), flash_buf[i]);
	}

	FLASHStatus = FLASH_BUSY;
	FLASH_LockBank1();
}

void flash_read(void)
{
	char sbuf[128];
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"car_adtime=%d,car_speed=%d,car_tick=%d,pos_speed=%d,pos_adtime=%d,pos_set=%d,id=%d\r",treadmill.adtime,treadmill.speed,uart2_485_delay\
	,servo.speed,servo.adtime,servo.position,device.id);
	debug_out1(sbuf,strlen(sbuf));
}

void flash_init(void)
{
	flash_adtime_data_get();
	flash_speed_data_get();
	flash_rs485_tick_data_get();
	flash_servo_motor_speed_data_get();
	flash_servo_motor_adtime_data_get();
	flash_servo_motor_position_data_get();
	flash_device_id_data_get();
}

void flash_adtime_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_CAR_ADTIME_DATA_ADDR);
	if(data != 0xffffffff){
		treadmill.adtime = data;
	}else treadmill.adtime = 5;
}

void flash_speed_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_CAR_SPEED_DATA_ADDR);
	if(data != 0xffffffff){
		treadmill.speed = data;
	}else treadmill.speed = 1300;
}

void flash_rs485_tick_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_CAR_RS485_TICK_ADDR);
	if(data != 0xffffffff){
		uart2_485_delay = data;
	}else {
		#ifdef BLDC
			uart2_485_delay = 8000;//BLDC 5 
		#else
			uart2_485_delay = 7;
		#endif
		
	}
}

void flash_servo_motor_speed_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_SERVO_MOTOR_SPEED_ADDR);
	if(data != 0xffffffff){
		servo.speed = data;
	}else servo.speed = 1500;
}

void flash_servo_motor_adtime_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_SERVO_MOTOR_ADTIME_ADDR);
	if(data != 0xffffffff){
		servo.adtime = data;
	}else servo.adtime = 50;
}

void flash_servo_motor_position_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_SERVO_MOTOR_POSITION_ADDR);
	if(data != 0xffffffff){
		servo.position = data;
	}else servo.position = 10000;
}

void flash_device_id_data_get(void)
{
	uint32_t data;
	data = *(__IO uint32_t*)(STARTADDR+FLASH_DEVICE_ID_ADDR);
	if(data != 0xffffffff){
		device.id = data;
	}else device.id=1;
}

