#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"

#ifndef _UCHAR_
#define _UCHAR_
#define uchar unsigned char
#endif


#define CAR1 0x01
extern unsigned int g_system_tick;
extern unsigned short uart2_485_delay;

enum{
	ON,
	OFF
};

enum{
	WIN1,
	WIN2,
	FOREWARD,
	BACKWARD,
	STOP,
	WAIT
};


void uart1_process(void);

#define debug_out1				uart1_sendstr
#define rs232_uart_init1	uart1_init
#define rs232_process1		uart1_process
#define get_rs232_len1		get_uart1_length
#define get_rs232_buf1		get_uart1_buf
#define rs232_clear1			uart1_clear

#define debug_out2				uart2_sendstr
#define rs232_uart_init2	uart2_init
#define rs232_process2		uart2_process
#define get_rs232_len2		get_uart2_length
#define get_rs232_buf2		get_uart2_buf
#define rs232_clear2			uart2_clear

#define debug_out4				uart4_sendstr
#define rs232_uart_init4	uart4_init
#define rs232_process4		uart4_process
#define get_rs232_len4		get_uart4_length
#define get_rs232_buf4		get_uart4_buf
#define rs232_clear4			uart4_clear

void reset(void);
unsigned int get_system_tick(void);

void treadmill_init(void);
void servo_motor_init(void);
void init(void);
void delay_ms(int ms);
void goto_win(void);
void send_servor_motor_data(uchar *ctrl_data,const short int data);
void send_servor_motor_pos(uchar *ctrl_data,int data);
void send_xiaoche_data(uchar *ctrl_data,const int data);
void send_xiaoche_id_data(uchar *ctrl_data,const int data,const int xiaoche_id);
void send_xiaoche_id_run_data(uchar *ctrl_data,const int xiaoche_id);
void check_car_and_servo(unsigned int tick_delay);

void servo_motor_rs485_init(void);
void set_adtime(unsigned int adtime);

void uart_485_delay(int delay);

void servo_motor_run_ctrl_func(unsigned int tick_delay);
void car_run_ctrl_func(unsigned int tick_delay);

uint8_t ret_ir(void);
void ir_win(void);

void set_addr_mobus_crc(unsigned char *input_str,unsigned int addr,unsigned int data);
void id_set_mobus_crc(unsigned char *input_str,unsigned int data,unsigned short array_0_data);
void set_mobus_crc(unsigned char *input_str,unsigned int data);
void id_run_ctrl(unsigned char *input_str,unsigned short array_0_data);
unsigned int retTickDiff(unsigned int tick);


#endif
