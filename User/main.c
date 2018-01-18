/**********************************************************************************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
 * 作者    ：牧梦苍穹  
 * 博客    ：http://jylzming.blog.163.com/
**********************************************************************************/

#include "stm32f10x.h"
#include "zigbeeSLC_usart.h"
#include "zigbeeGS_usart.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "common.h"
#include "string.h"

#define ESP8266_AP_SSID			"mingWIFI"	
#define ESP8266_AP_ECN			OPEN
#define ESP8266_AP_PWD			"12345678"
#define ESP8266_IP				"10.10.100.254"
#define ESP8266_TCP_PORT		"8899"
#define ESP8266_TCP_OVERTIME	"1800"

void Delay(void);

int main(void)
{	
	char testStr[12] = {'\0'};
	char *p,*pCh;
	uint8_t i, ucId, length = 0;
	char tmpStr[RX_BUF_MAX_LEN] = {0};
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	//SystemInit();//函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	
	char cStr [ 100 ] = { 0 };
	
	ZigBeeSLC_USART_Config();
	ZigBeeGS_USART_Config();
	SysTick_Init();
	ESP8266_Init();
	
	ESP8266_AT_Test();
	ESP8266_Rst();
	ESP8266_StartOrShutServer(DISABLE, ESP8266_TCP_PORT, ESP8266_TCP_OVERTIME);
	ESP8266_Net_Mode_Choose(STA_AP);
	while(! ESP8266_CIPAP(ESP8266_IP));
	while(! ESP8266_BuildAP(ESP8266_AP_SSID, ESP8266_AP_PWD, ESP8266_AP_ECN));
	ESP8266_Enable_MultipleId(ENABLE);
	while(! ESP8266_StartOrShutServer(ENABLE, ESP8266_TCP_PORT, ESP8266_TCP_OVERTIME));
	ESP8266_Inquire_ApIp(cStr, 20);
	ESP8266_SendString(DISABLE, cStr,  strlen(cStr), Multiple_ID_0);
//	ZigBeeSLC_Usart("本模块IP地址为%s", cStr);
//	ZigBeeSLC_Usart(" 端口为s%\r\n", ESP8266_TCP_PORT);
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
	strZigBeeSLC_Fram_Record .InfBit .FramLength = 0;
	strZigBeeSLC_Fram_Record .InfBit .FramFinishFlag = 0;	
	strZigBeeGS_Fram_Record .InfBit .FramLength = 0;
	strZigBeeGS_Fram_Record .InfBit .FramFinishFlag = 0;
	while( 1 )
	{
		//WIFI 串口ESP8266接收到数据，转发到ZigBee串口
		if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)
		{
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';			
			//
			//ZigBeeSLC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);
			p = strstr(strEsp8266_Fram_Record.Data_RX_BUF, ":");//定位到接收数据实体的前一位（“：”）
			if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) 
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
			}			
			//将接收到的通过ZigBee串口发出，这里不能用SendString，因为0x00会被当成结束符'\0'
			if((unsigned char)p[1] == 0xAA || (unsigned char)p[1] == 0xAB)//p[0]=':',p[1]=0xAA...
			{
				for(i = 1; i<=strEsp8266_Fram_Record.InfBit.FramLength; i++)
				{
					if(p[i] == '\0')//判断是否是0x55 和字符串结束符，若是则发送结束
						if(p[i-1] == 0x55 || (unsigned char)p[i-1] == 0xBA)
							break;
					USART_SendData(ZigBeeSLC_USARTx, p[i]);					
					while(USART_GetFlagStatus(ZigBeeSLC_USARTx, USART_FLAG_TXE) == RESET);
					USART_SendData(ZigBeeGS_USARTx, p[i]);					
					while(USART_GetFlagStatus(ZigBeeGS_USARTx, USART_FLAG_TXE) == RESET);						
				}
			}
			else//如果不是0xAA为开头的数据，直接发送字符串咯
			{
				ZigBeeSLC_Usart("%s", p);
			}
			strEsp8266_Fram_Record .InfBit .FramLength = 0;
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;				
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
		}
		//ZigBee接收到单灯控制器的数据 USART1
		if(strZigBeeSLC_Fram_Record.InfBit.FramFinishFlag)
		{
			USART_ITConfig ( ZigBeeSLC_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			//strZigBeeSLC_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
			
			//转发到ESP8266
			length = strZigBeeSLC_Fram_Record.InfBit.FramLength;//length等于ZigBee串口接收到的数据长度
			memcpy(tmpStr, strZigBeeSLC_Fram_Record.Data_RX_BUF, RX_BUF_MAX_LEN);//把ZigBee串口接收到的数据复制到tmpStr，防止使用中丢失
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, length);
			if(ESP8266_Cmd ( cStr, "> ", 0, 300 ))//等待时间过长会导致后续数据丢失
			{
				for(i=0; i<length; i++)//这里不能用SendString，因为0x00会被当成结束符'\0'
				{
					USART_SendData(macESP8266_USARTx, tmpStr[i]);
					while(USART_GetFlagStatus(macESP8266_USARTx, USART_FLAG_TXE) == RESET);
				} 			
			}
			//清除标志
			memset(tmpStr, '\0', sizeof(tmpStr));//清空字符串
			strZigBeeSLC_Fram_Record .InfBit .FramLength = 0;
			strZigBeeSLC_Fram_Record .InfBit .FramFinishFlag = 0;				
			USART_ITConfig ( ZigBeeSLC_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断				
		}
		
		//ZigBee接收到地磁传感器的数据 USART2
		if(strZigBeeGS_Fram_Record.InfBit.FramFinishFlag)
		{
			USART_ITConfig ( ZigBeeGS_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			//strZigBeeSLC_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
			
			//转发到ESP8266
			length = strZigBeeGS_Fram_Record.InfBit.FramLength;//length等于ZigBee串口接收到的数据长度
			memcpy(tmpStr, strZigBeeGS_Fram_Record.Data_RX_BUF, RX_BUF_MAX_LEN);//把ZigBee串口接收到的数据复制到tmpStr，防止使用中丢失
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, length);
			if(ESP8266_Cmd ( cStr, "> ", 0, 300 ))//等待时间过长会导致后续数据丢失
			{
				for(i=0; i<length; i++)//这里不能用SendString，因为0x00会被当成结束符'\0'
				{
					USART_SendData(macESP8266_USARTx, tmpStr[i]);
					while(USART_GetFlagStatus(macESP8266_USARTx, USART_FLAG_TXE) == RESET);
				} 			
			}
			//清除标志
			memset(tmpStr, '\0', sizeof(tmpStr));//清空字符串
			strZigBeeGS_Fram_Record .InfBit .FramLength = 0;
			strZigBeeGS_Fram_Record .InfBit .FramFinishFlag = 0;				
			USART_ITConfig ( ZigBeeGS_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断				
		}
	}
}

void Delay(void)
{
	uint16_t i = 0, j = 0;
	for(; i< 1024; i++)
		for(; j< 1024; j++);
}

