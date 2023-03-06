
#include "LIB/Bit_Math.h"
#include "LIB/Standerd_Types.h"
#include<avr/io.h>
#include<util/delay.h>
#include "DIO_interface.h"
#include "LCD_cfg.h"
#include "LCD_interface.h"


#define USART_BAUDRATE 19200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void usart_init();
unsigned int usart_getch();

u8 lati_value[16]={0}, longi_value[16]={0},lati_dir,longi_dir, value, ch, i ,j;


int main(void)
{
	DIO_vidSetPortDir(DIO_PORTC, DIO_PORT_OUTPUT); //LCD Data Port
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN5, DIO_OUTPUT);  //LCD Control
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN6, DIO_OUTPUT);   //LCD Control
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN4, DIO_OUTPUT); //LCD Control

	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN0, DIO_INPUT);   //D0
	DIO_vidSetPinVal(DIO_PORTD,DIO_PIN0, DIO_HIGH);   //D0

	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN1, DIO_OUTPUT); //D1


LCD_vidInit();
usart_init();
//LCD_vidSendCommand(0x80);

while(1)
{
	//LCD_vidSendCommand(0x80);
	value=usart_getch();
	LCD_vidSendChar(value);
	value=usart_getch();
		LCD_vidSendChar(value);
		value=usart_getch();
			LCD_vidSendChar(value);
			value=usart_getch();
				LCD_vidSendChar(value);
				value=usart_getch();
					LCD_vidSendChar(value);
					value=usart_getch();
						LCD_vidSendChar(value);
						value=usart_getch();
							LCD_vidSendChar(value);
							value=usart_getch();
								LCD_vidSendChar(value);
								value=usart_getch();
									LCD_vidSendChar(value);
									value=usart_getch();
										LCD_vidSendChar(value);
}

}
//while(1)
//{
//  value=usart_getch();
//  if(value=='$')
//  {
//   value=usart_getch();
//   if(value=='G')
//   {
//    value=usart_getch();
//    if(value=='P')
//    {
//     value=usart_getch();
//     if(value=='G')
//     {
//      value=usart_getch();
//      if(value=='G')
//      {
//       value=usart_getch();
//       if(value=='A')
//       {
//        value=usart_getch();
//        if(value==',')
//        {
//         value=usart_getch();
//         while(value!=',')
//         {
//          value=usart_getch();
//         }
//         lati_value[0]=usart_getch();
//         value=lati_value[0];
//         for(i=1;value!=',';i++)
//         {
//          lati_value[i]=usart_getch();
//          value=lati_value[i];
//         }
//         lati_dir=usart_getch();
//         value=usart_getch();
//        /* while(value!=',')
//         {
//          value=usart_getch();
//         }*/
//         longi_value[0]=usart_getch();
//         value=longi_value[0];
//         for(i=1;value!=',';i++)
//         {
//          longi_value[i]=usart_getch();
//          value=longi_value[i];
//         }
//         longi_dir=usart_getch();
//
//         LCD_vidSendCommand(0x80);
//         LCD_vidWriteStr(lati_value);
//         LCD_vidSendChar(lati_dir);
//
//         LCD_vidSendCommand(0xC0);
//         LCD_vidWriteStr(longi_value);
//         LCD_vidSendChar(longi_dir);
//
//        }
//       }
//      }
//     }
//    }
//   }
//  }
//}
//}


void usart_init()
{

UCSRB |= (1<<RXCIE) | (1 << RXEN) | (1 << TXEN);
UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);

UBRRL = BAUD_PRESCALE;
UBRRH = (BAUD_PRESCALE >> 8);
}

unsigned int usart_getch()
{

while ((UCSRA & (1 << RXC)) == 0);
return(UDR);
}
