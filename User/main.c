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
#define ESP8266_IP				"192.168.2.1"
#define ESP8266_TCP_PORT		"333"
#define ESP8266_TCP_OVERTIME	"1800"

void Delay(void);

int main(void)
{	
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
	ESP8266_Net_Mode_Choose(STA_AP);
	while(! ESP8266_CIPAP(ESP8266_IP));
	while(! ESP8266_BuildAP(ESP8266_AP_SSID, ESP8266_AP_PWD, ESP8266_AP_ECN));
	ESP8266_Enable_MultipleId(ENABLE);
	while(! ESP8266_StartOrShutServer(ENABLE, ESP8266_TCP_PORT, ESP8266_TCP_OVERTIME));
	ESP8266_Inquire_ApIp(cStr, 20);
	ZigBeeSLC_Usart("本模块IP地址为%s", cStr);
	ZigBeeSLC_Usart(" 端口为s%\r\n", ESP8266_TCP_PORT);
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
	strZigBeeSLC_Fram_Record .InfBit .FramLength = 0;
	strZigBeeSLC_Fram_Record .InfBit .FramFinishFlag = 0;	
	
	while( 1 )
	{
		if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)
		{
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';			
			ZigBeeSLC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);//转发到ZigBee串口
			strEsp8266_Fram_Record .InfBit .FramLength = 0;
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;				
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
		}
		if(strZigBeeSLC_Fram_Record.InfBit.FramFinishFlag)
		{
			USART_ITConfig ( ZigBeeSLC_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			//strZigBeeSLC_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
			
			ESP8266_SendString (DISABLE, strZigBeeSLC_Fram_Record .Data_RX_BUF, strlen ( strZigBeeSLC_Fram_Record .Data_RX_BUF ), Multiple_ID_0 );
			//macESP8266_Usart("%s", strZigBeeSLC_Fram_Record.Data_RX_BUF);//通过WIFI转发出去
			strZigBeeSLC_Fram_Record .InfBit .FramLength = 0;
			strZigBeeSLC_Fram_Record .InfBit .FramFinishFlag = 0;				
			USART_ITConfig ( ZigBeeSLC_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断			
		}
	}
}

void Delay(void)
{
	uint16_t i = 0, j = 0;
	for(; i< 1024; i++)
		for(; j< 1024; j++);
}

