#ifndef __USART1_H
#define	__USART1_H


#include "stm32f10x.h"
#include <stdio.h>



/**************************USART????????********************************/
#define             ZigBeeSLC_USART_BAUD_RATE                       115200

#define             ZigBeeSLC_USARTx                                USART1
#define             ZigBeeSLC_USART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             ZigBeeSLC_USART_CLK                             RCC_APB2Periph_USART1
#define             ZigBeeSLC_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             ZigBeeSLC_USART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             ZigBeeSLC_USART_TX_PORT                         GPIOA   
#define             ZigBeeSLC_USART_TX_PIN                          GPIO_Pin_9
#define             ZigBeeSLC_USART_RX_PORT                         GPIOA 
#define             ZigBeeSLC_USART_RX_PIN                          GPIO_Pin_10
#define             ZigBeeSLC_USART_IRQ                             USART1_IRQn
#define             ZigBeeSLC_USART_INT_FUN                         USART1_IRQHandler

#define     ZigBeeSLC_Usart( fmt, ... )           USART_printf ( ZigBeeSLC_USARTx, fmt, ##__VA_ARGS__ ) 

void  ZigBeeSLC_USART_Config( void );
static void ZigBeeSLC_USART_NVIC_Configuration(void);


#endif /* __USART1_H */
