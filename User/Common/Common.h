#ifndef __COMMON_H
#define __COMMON_H



#include "stm32f10x.h"


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/******************************* 宏定义 ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2


#define RX_BUF_MAX_LEN     1024                                     //?????????


extern struct  STRUCT_USARTx_Fram                                  //???????????
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];

	union {
	__IO u16 InfAll;
	struct {
		  __IO u16 FramLength       :15;                               // 14:0 ??,???? 15bit
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
	}; 	
}strEsp8266_Fram_Record, strZigBeeSLC_Fram_Record;



/********************************** 函数声明 ***************************************/
void  USART_printf( USART_TypeDef * USARTx, char * Data, ... );
void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen );
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);


#endif /* __COMMON_H */

