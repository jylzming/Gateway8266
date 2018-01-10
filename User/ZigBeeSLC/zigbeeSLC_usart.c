#include "zigbeeSLC_usart.h"
#include "common.h"

struct  STRUCT_USARTx_Fram strZigBeeSLC_Fram_Record = { 0 };

 /**
  * @brief  USARTx GPIO ????,?????????ив?115200 8-N-1
  * @param  ??
  * @retval ??
  */
void ZigBeeSLC_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	ZigBeeSLC_USART_APBxClock_FUN(ZigBeeSLC_USART_CLK, ENABLE);
	ZigBeeSLC_USART_GPIO_APBxClock_FUN(ZigBeeSLC_USART_GPIO_CLK, ENABLE);
		
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  ZigBeeSLC_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ZigBeeSLC_USART_TX_PORT, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = ZigBeeSLC_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ZigBeeSLC_USART_RX_PORT, &GPIO_InitStructure);	
			
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = ZigBeeSLC_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ZigBeeSLC_USARTx, &USART_InitStructure);

	USART_ITConfig ( ZigBeeSLC_USARTx, USART_IT_RXNE, ENABLE ); 
	USART_ITConfig ( ZigBeeSLC_USARTx, USART_IT_IDLE, ENABLE ); 
	ZigBeeSLC_USART_NVIC_Configuration();
	USART_Cmd(ZigBeeSLC_USARTx, ENABLE);
}


static void ZigBeeSLC_USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ZigBeeSLC_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/// ?????c????printf??USART1
int fputc(int ch, FILE *f)
{
		/* ???????????????USART1 */
		USART_SendData(ZigBeeSLC_USARTx, (uint8_t) ch);
		
		/* ?????????? */
		while (USART_GetFlagStatus(ZigBeeSLC_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// ?????c????scanf??USART1
int fgetc(FILE *f)
{
		/* ???????1???????? */
		while (USART_GetFlagStatus(ZigBeeSLC_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(ZigBeeSLC_USARTx);
}
/*********************************************END OF FILE**********************/
