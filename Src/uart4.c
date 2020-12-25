#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

#define UART4_REC_LEN  			200  	  	
unsigned char uart4_rx_buffer[UART4_REC_LEN];    
unsigned int uart4_rx_len = 0;      
int	flag_uart4_rx = 0;

void rs3485_pb14_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_14); //默认485接收模式
}

unsigned char * get_uart4_buf(void)
{
	return uart4_rx_buffer;
}

unsigned int get_uart4_length(void)
{
	return uart4_rx_len;
}

void uart4_clear(void)
{
	memset((char*)uart4_rx_buffer, 0, UART4_REC_LEN);
	uart4_rx_len = 0;
	flag_uart4_rx = 0;
}

void uart4_init(unsigned int baudrate)
{    
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

 	USART_DeInit(UART4);
	//USART4_TX   PC10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //USART4_RX	  PC11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baudrate;
	#ifdef LICHUANG
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	#else
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	#endif
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	#ifdef LICHUANG
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	#else
	USART_InitStructure.USART_Parity = USART_Parity_No;
	#endif
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
   
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
  USART_Cmd(UART4, ENABLE);
	rs3485_pb14_init();
}

void uart_485_delay(int delay)
{
	int j;
		for(j=0;j<delay;j++){
		}
}

void uart4_sendstr(char *str, int len)
{
	int i = 0;
	
	if (len <= 0)	return;
	uart_485_delay(100);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	for(i=0; i<len; i++) {
		USART_SendData(UART4, (unsigned short)str[i]);
		while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
	}
	uart_485_delay(2500);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

 void UART4_IRQHandler(void) 
 {
 	unsigned char data;
 	
 	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) { 
 		
		data = USART_ReceiveData(UART4);
// 		USART_SendData(USART1, (unsigned short)data);
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		uart4_rx_buffer[uart4_rx_len++] = data;
		if (uart4_rx_len >= UART4_REC_LEN) { // overflow
 					uart4_rx_len = UART4_REC_LEN - 1;
 		}
 	}
}
