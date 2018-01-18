/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_esp8266.h"
#include "zigbeeSLC_usart.h"
#include "zigbeeGS_usart.h"
#include "bsp_SysTick.h"
#include "common.h"
#include <stdio.h>
#include <string.h> 

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles macESP8266_USARTx Handler.
  * @param  None
  * @retval None
  *          USART1
  */
void ZigBeeSLC_USART_INT_FUN(void)
{
	uint8_t ucCh;
	volatile uint8_t clear = clear;
	if ( USART_GetITStatus ( ZigBeeSLC_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( ZigBeeSLC_USARTx );	
		//ucCh = CharToHex(ucCh);//将HEX转换为ASCII码字符串
		if(strZigBeeSLC_Fram_Record.InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) //预留1个字节写结束符
			strZigBeeSLC_Fram_Record .Data_RX_BUF [ strZigBeeSLC_Fram_Record .InfBit .FramLength ++ ]  = ucCh;
	}
	if ( USART_GetITStatus( ZigBeeSLC_USARTx, USART_IT_IDLE ) == SET ) //数据帧接收完毕
	{
		/*由于单灯控制器发送数据过慢导致空闲中断触发，数据无回车换行
		strZigBeeSLC_Fram_Record.Data_RX_BUF[strZigBeeSLC_Fram_Record .InfBit .FramLength - 1] 等于0x55*/
		if( strZigBeeSLC_Fram_Record .InfBit .FramLength > 5 &&
			((unsigned char)strZigBeeSLC_Fram_Record.Data_RX_BUF[strZigBeeSLC_Fram_Record .InfBit .FramLength - 1] == 0x55 ||
			(unsigned char)strZigBeeSLC_Fram_Record.Data_RX_BUF[strZigBeeSLC_Fram_Record .InfBit .FramLength - 2] == 0x55 ||
			(unsigned char)strZigBeeSLC_Fram_Record.Data_RX_BUF[strZigBeeSLC_Fram_Record .InfBit .FramLength - 3] == 0x55))
			strZigBeeSLC_Fram_Record .InfBit .FramFinishFlag = 1;			
		/*将HEX转换为ASCII码字符串*/
		//ucCh = USART_ReceiveData( ZigBeeSLC_USARTx );     
		//由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
		clear = ZigBeeSLC_USARTx->SR;
		clear = ZigBeeSLC_USARTx->DR;
	}
}

/***********************USART2******************************/
void ZigBeeGS_USART_INT_FUN(void)
{
	uint8_t ucCh;
	volatile uint8_t clear = clear;
	if ( USART_GetITStatus ( ZigBeeGS_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( ZigBeeGS_USARTx );	
		//ucCh = CharToHex(ucCh);//将HEX转换为ASCII码字符串
		if(strZigBeeGS_Fram_Record.InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) //预留1个字节写结束符
			strZigBeeGS_Fram_Record .Data_RX_BUF [ strZigBeeGS_Fram_Record .InfBit .FramLength ++ ]  = ucCh;
	}
	if ( USART_GetITStatus( ZigBeeGS_USARTx, USART_IT_IDLE ) == SET ) //数据帧接收完毕
	{
		/*由于传感器发送数据过慢导致空闲中断触发，数据无回车换行
		strZigBeeGS_Fram_Record.Data_RX_BUF[strZigBeeGS_Fram_Record .InfBit .FramLength - 1] 等于0x55*/
		if( strZigBeeGS_Fram_Record .InfBit .FramLength > 5 &&  (unsigned char)strZigBeeGS_Fram_Record.Data_RX_BUF[strZigBeeGS_Fram_Record .InfBit .FramLength - 1] == 0x55)	
			strZigBeeGS_Fram_Record .InfBit .FramFinishFlag = 1;	
		/*将HEX转换为ASCII码字符串*/   
		//由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
		clear = ZigBeeGS_USARTx->SR;
		clear = ZigBeeGS_USARTx->DR;
	}	
}

/****************USART3******************************/
void macESP8266_USART_INT_FUN ( void )
{	
	uint8_t ucCh;
	volatile uint8_t clear = clear;
	if ( USART_GetITStatus ( macESP8266_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( macESP8266_USARTx );		
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) //预留1个字节写结束符
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;
	}
	if ( USART_GetITStatus( macESP8266_USARTx, USART_IT_IDLE ) == SET ) //数据帧接收完毕
	{
		strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;	
		ucCh = USART_ReceiveData( macESP8266_USARTx );     //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
		clear = macESP8266_USARTx->SR;
		clear = macESP8266_USARTx->DR;
		
		
//		ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
	}	
}



/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
