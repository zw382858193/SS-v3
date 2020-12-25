#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

#define USART1_REC_LEN 200  	  	
unsigned char usart1_rx_buffer[USART1_REC_LEN];    
unsigned int usart1_rx_len = 0;    
unsigned char usart1_at_buffer[2];
int	flag_usart1_rx = 0;

unsigned char * get_uart1_buf(void)
{
	return usart1_rx_buffer;
}

unsigned int get_uart1_length(void)
{
	if(flag_usart1_rx == 1)
		return usart1_rx_len;
	else
		return 0;
}

void uart1_clear(void)
{
	memset((char*)usart1_rx_buffer, 0, USART1_REC_LEN);
	usart1_rx_len = 0;
	flag_usart1_rx = 0;
}

void uart1_init(unsigned int baudrate)
{    
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	USART_DeInit(USART1);
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);


	USART_Cmd(USART1, ENABLE); 

}


void uart1_sendstr(char *str, int len)
{
	int i = 0;
	
	if (len <= 0)	return;
	
	for(i=0; i<len; i++) {
		USART_SendData(USART1, (unsigned short)str[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}


void USART1_IRQHandler(void) 
{
	unsigned char data;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {

 	data = USART_ReceiveData(USART1);	
//  USART_SendData(USART1, (unsigned short)data);
// 	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		if (flag_usart1_rx == 0) {
			
			if (data == '\r' || data == '\n') {
				
				if (usart1_rx_len > 0) {
					flag_usart1_rx = 1;

				}
			} else {
					usart1_rx_buffer[usart1_rx_len++] = data;
					if (usart1_rx_len >= USART1_REC_LEN) { // overflow
					usart1_rx_len = USART1_REC_LEN - 1;
				}
			}
		}
	}
}
