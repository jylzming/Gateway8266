#ifndef __USART2_H
#define	__USART2_H


#include "stm32f10x.h"
#include <stdio.h>



/**************************USART参数定义********************************/
#define             ZigBeeGS_USART_BAUD_RATE                       115200

#define             ZigBeeGS_USARTx                                USART2
#define             ZigBeeGS_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             ZigBeeGS_USART_CLK                             RCC_APB1Periph_USART2
#define             ZigBeeGS_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             ZigBeeGS_USART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             ZigBeeGS_USART_TX_PORT                         GPIOA   
#define             ZigBeeGS_USART_TX_PIN                          GPIO_Pin_2
#define             ZigBeeGS_USART_RX_PORT                         GPIOA 
#define             ZigBeeGS_USART_RX_PIN                          GPIO_Pin_3
#define             ZigBeeGS_USART_IRQ                             USART2_IRQn
#define             ZigBeeGS_USART_INT_FUN                         USART2_IRQHandler

#define     ZigBeeGS_Usart( fmt, ... )           USART_printf ( ZigBeeGS_USARTx, fmt, ##__VA_ARGS__ ) 

void ZigBeeGS_USART_Config( void );
static void ZigBeeGS_USART_NVIC_Configuration(void);



#endif /* __USART1_H */
