/*
 * UART_prog.c
 *
 *  Created on: Oct 28, 2022
 *      Author: Rago
 */
#include"LIB/Bit_Math.h"
#include"LIB/Standerd_Types.h"
#include"UART_config.h"
#include"UART_priv.h"
#include"UART_interface.h"


void UART_vidInit(void)
{

	u8 Local_u8UCSRCValue=0;
	SET_BIT(Local_u8UCSRCValue,UART_UCSRC_UCSZ0);
	SET_BIT(Local_u8UCSRCValue,UART_UCSRC_UCSZ1);
	CLR_BIT(UCSRB_REG, UART_UCSRB_UCSZ2);

	SET_BIT(Local_u8UCSRCValue,UART_UCSRC_URSEL);
	CLR_BIT(Local_u8UCSRCValue,UART_UCSRC_UMSEL);

	CLR_BIT(Local_u8UCSRCValue,UART_UCSRC_UPM0);
	CLR_BIT(Local_u8UCSRCValue,UART_UCSRC_UPM1);

	SET_BIT(Local_u8UCSRCValue,UART_UCSRC_USBS);

	UCSRC_REG= Local_u8UCSRCValue;
//8 bit data and accessing ucsrc
 CLR_BIT(UCSRA_REG,UART_UCSRA_U2X); //synchronous operation
 UBRRL_REG=51;//b7ded beh el baud rate 51-->9600
 SET_BIT(UCSRB_REG,UART_UCSRB_TXEN); // enable transmitter
 SET_BIT(UCSRB_REG,UART_UCSRB_RXEN); // enable Reciver
}

void UART_vidTrans(u8 u8_data)
{
	while(GET_BIT(UCSRA_REG,UART_UCSRA_UDRE)==0);
	UDR_REG=u8_data;
}

u8 UART_u8Recive(void)
{
	u8 data_recived=0;
	while(GET_BIT(UCSRA_REG,UART_UCSRA_RXC)==0);
	data_recived=UDR_REG;
	return data_recived;

}
