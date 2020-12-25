#ifndef _UART_H_
#define _UART_H_

extern void uart1_init(unsigned int baudrate);
extern unsigned int get_uart1_length(void);
extern unsigned char * get_uart1_buf(void);
extern void uart1_sendstr(char *str, int len);
extern void uart1_clear(void);

extern void uart2_init(unsigned int baudrate);
extern unsigned int get_uart2_length(void);
extern unsigned char * get_uart2_buf(void);
extern void uart2_sendstr(char *str, int len);
extern void uart2_clear(void);

extern void uart3_init(unsigned int baudrate);
extern unsigned int get_uart3_length(void);
extern unsigned char * get_uart3_buf(void);
extern void uart3_sendstr(char *str, int len);
extern void uart3_clear(void);

extern void uart4_init(unsigned int baudrate);
extern unsigned int get_uart4_length(void);
extern unsigned char * get_uart4_buf(void);
extern void uart4_sendstr(char *str, int len);
extern void uart4_clear(void);

extern void uart5_init(unsigned int baudrate);
extern unsigned int get_uart5_length(void);
extern unsigned char * get_uart5_buf(void);
extern void uart5_sendstr(char *str, int len);
extern void uart5_clear(void);

void my_sprintf_32(char *sbuf,int i);

#endif

