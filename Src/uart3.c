#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

#define USART_REC_LEN  			200  	  	
unsigned char usart3_rx_buffer[USART_REC_LEN];    
unsigned int usart3_rx_len = 0;      
int	flag_usart3_rx = 0;



 unsigned char * get_uart3_buf(void)
 {
 	return usart3_rx_buffer;
 }

 unsigned int get_uart3_length(void)
 {
 		return usart3_rx_len;
 }

 void uart3_clear(void)
 {
 	memset((char*)usart3_rx_buffer, 0, USART_REC_LEN);
 	usart3_rx_len = 0;
 	flag_usart3_rx = 0;
 }

void uart3_init(unsigned int baudrate)
{    
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);


	USART_DeInit(USART3);
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


	USART_Cmd(USART3, ENABLE); 
}

void delay_485_ms(int ms)
{
	int i,j;
	for(i=0;i<10000;i++)
		for(j=0;j<ms;j++){
		}
}

void uart3_sendstr(char *str, int len)
{
	int i = 0;
	
	if (len <= 0)	return;

	for(i=0; i<len; i++) {
		USART_SendData(USART3, (unsigned short)str[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}

}

 void USART3_IRQHandler(void) 
 {
 	unsigned char data;
 	
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) { 
 		
 		data = USART_ReceiveData(USART3);	
 // 		USART_SendData(USART3, (unsigned short)data);
 // 		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
 		usart3_rx_buffer[usart3_rx_len++] = data;
 		if (usart3_rx_len >= USART_REC_LEN) { // overflow
 					usart3_rx_len = USART_REC_LEN - 1;
 		}		
 	}
}



