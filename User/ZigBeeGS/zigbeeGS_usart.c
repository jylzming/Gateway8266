/**
  ******************************************************************************
  * @file    zigbeeGS_usart.c  //USART1
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "zigbeeGS_usart.h"

 /**
  * @brief  USARTx GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void ZigBeeGS_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config ZigBeeGS_USART clock */
	ZigBeeGS_USART_APBxClock_FUN(ZigBeeGS_USART_CLK, ENABLE);
	ZigBeeGS_USART_GPIO_APBxClock_FUN(ZigBeeGS_USART_GPIO_CLK, ENABLE);
		
	/* USART1 GPIO config */
	/* Configure USART2 Tx (PA.2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  ZigBeeGS_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ZigBeeGS_USART_TX_PORT, &GPIO_InitStructure);    
	/* Configure USART2 Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = ZigBeeGS_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ZigBeeGS_USART_RX_PORT, &GPIO_InitStructure);	
			
	/* ZigBeeGS_USART mode config */
	USART_InitStructure.USART_BaudRate = ZigBeeGS_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ZigBeeGS_USART, &USART_InitStructure);
	ZigBeeGS_USART_NVIC_Configuration();
	USART_Cmd(ZigBeeGS_USART, ENABLE);
	
}

static void ZigBeeGS_USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ZigBeeGS_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

///// �ض���c�⺯��printf��USART1
//int fputc(int ch, FILE *f)
//{
//		/* ����һ���ֽ����ݵ�USART1 */
//		USART_SendData(macUSARTx, (uint8_t) ch);
//		
//		/* �ȴ�������� */
//		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

///// �ض���c�⺯��scanf��USART1
//int fgetc(FILE *f)
//{
//		/* �ȴ�����1�������� */
//		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(macUSARTx);
//}
/*********************************************END OF FILE**********************/
