/*
 * UART_priv.h
 *
 *  Created on: Oct 28, 2022
 *      Author: Rago
 */
#ifndef UART_PRIV_H_
#define UART_PRIV_H_

#define UDR_REG    *((volatile u8*)0x2C)


#define UCSRA_REG  *((volatile u8*)0x2B)

#define UART_UCSRA_MPCM          0
#define UART_UCSRA_U2X           1
#define UART_UCSRA_PE            2
#define UART_UCSRA_DOR           3
#define UART_UCSRA_FE            4
#define UART_UCSRA_UDRE          5
#define UART_UCSRA_TXC           6
#define UART_UCSRA_RXC           7


#define UCSRB_REG  *((volatile u8*)0x2A)

#define UART_UCSRB_TXB8          0
#define UART_UCSRB_RXB8          1
#define UART_UCSRB_UCSZ2         2
#define UART_UCSRB_TXEN          3
#define UART_UCSRB_RXEN          4
#define UART_UCSRB_UDRIE         5
#define UART_UCSRB_TXCIE         6
#define UART_UCSRB_RXCIE         7


#define UBRRL_REG  *((volatile u8*)0x29)

#define UCSRC_REG  *((volatile u8*)0x40)
#define UART_UCSRC_UCPOL         0
#define UART_UCSRC_UCSZ0         1
#define UART_UCSRC_UCSZ1         2
#define UART_UCSRC_USBS          3
#define UART_UCSRC_UPM0          4
#define UART_UCSRC_UPM1          5
#define UART_UCSRC_UMSEL         6
#define UART_UCSRC_URSEL         7



#endif /* UART_PRIV_H_ */
