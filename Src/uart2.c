#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "config.h"


#define USART2_REC_LEN 200
unsigned char usart2_rx_buffer[USART2_REC_LEN];
unsigned int usart2_rx_len = 0;
int	flag_usart2_rx = 0;

#if 0
void rs3485_pb15_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_15); //默认485接收模式
}
#else
void rs3485_pa4_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4); //默认485接收模式
}
#endif

unsigned char * get_uart2_buf(void)
{
	return usart2_rx_buffer;
}

unsigned int get_uart2_length(void)
{
		return usart2_rx_len;
}

void uart2_clear(void)
{
	memset((char*)usart2_rx_buffer, 0, USART2_REC_LEN);
	usart2_rx_len = 0;
	flag_usart2_rx = 0;
}

void uart2_init(unsigned int baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

 	USART_DeInit(USART2);
	//USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //USART2_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//USART_InitStructure.USART_WordLength = USART_WordLength_9b;//LICHUANG
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//USART_InitStructure.USART_Parity = USART_Parity_Even;//LICHUANG
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
   
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  USART_Cmd(USART2, ENABLE); 
	//rs3485_pb15_init();
	rs3485_pa4_init();
}

void uart2_sendstr(char *str, int len)
{
	int i = 0;

	if (len <= 0)	return;
	//GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	uart_485_delay(1000);
	for(i=0; i<len; i++) {
		USART_SendData(USART2, (unsigned short)str[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
	uart_485_delay(uart2_485_delay);
	//GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

void USART2_IRQHandler(void) 
{
	unsigned char data;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		
		data = USART_ReceiveData(USART2);
//		USART_SendData(USART1, (unsigned short)data);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		usart2_rx_buffer[usart2_rx_len++] = data;
 		if (usart2_rx_len >= USART2_REC_LEN) { // overflow
 					usart2_rx_len = USART2_REC_LEN - 1;
 		}
	}
}
