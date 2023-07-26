

#include"bit.h"
#include"types.h"
#include <avr/delay.h>
#include "LCD_cfg.h"
#include "DIO_interface.h"
#include "LCD_interface.h"


void LCD_vidInit(void) //Initialization
{
	//delay 30 ms
	_delay_ms(30);
	//function set command
	LCD_vidSendCommand(LCD_FUNCSET_8BIT);
	_delay_ms(1);
	//display on/off
	LCD_vidSendCommand(LCD_DISPON_CURSON);
	_delay_ms(15);
	//return home
	LCD_vidSendCommand(LCD_CLR);
	_delay_ms(15);
	//entry set command
	LCD_vidSendCommand(LCD_ENTRYMODE);
	_delay_ms(2);
}

void LCD_vidSendCommand(u8 u8Cmd)
{
	//Clear RS
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_RS_PIN, DIO_LOW);
	//Clear RW
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_RW_PIN, DIO_LOW);
	//Set E to high
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_E_PIN, DIO_HIGH);
	//put cmd on data port
	DIO_vidSetPortVal(LCD_DATA_PORT, u8Cmd);
	//clear E pin
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_E_PIN, DIO_LOW);
	_delay_ms(5);

}
void LCD_vidSendChar(u8 u8Char)
{
	//Set RS
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_RS_PIN, DIO_HIGH);
	//Clear RW
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_RW_PIN, DIO_LOW);
	//Set E to high
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_E_PIN, DIO_HIGH);
	//put
	DIO_vidSetPortVal(LCD_DATA_PORT, u8Char);
	//clear E pin
	DIO_vidSetPinVal(LCD_CONTROL_PORT,LCD_E_PIN, DIO_LOW);
	_delay_ms(5);

}

void LCD_vidWriteStr(u8* pu8Str)
{
	u8 i=0;
	while(pu8Str[i]!=0)    //any array of character prints \0 in the end   (not char)
	{

		LCD_vidSendChar(pu8Str[i]);
		i++;
	}
}
void LCD_vidGoToXY(u8 x,u8 y)
{

}
void LCD_ClrLine(u8 Line)
{
	switch (Line)
	{
	case 0:
		LCD_vidGoToXY(0,0);
		LCD_vidWriteStr("                ");
		break;
	case 1:
		LCD_vidGoToXY(0,1);
		LCD_vidWriteStr("                ");
	    break;
	}
}

void LCD_Custom_Char (u8 loc, u8 msg[])
{
    u8 i;
    if(loc<8)
    {
     LCD_vidSendCommand (0x40 + (loc*8));  /* Command 0x40 and onwards forces
                                       the device to point CGRAM address */
                                      //CGRAM begins from 0x40
       for(i=0;i<8;i++)  /* Write 8 byte for generation of 1 character */
           LCD_vidSendChar(msg[i]);                  //print char
    }
}
void LCD_PRINT_INT(u16 x)
{
	u8 arr[4]={0};
	u8 i=4;
	while(x!=0)
	{                                                   //123 ---> get 3
		arr[i-1]=(x%10)+'0'; //ascii of 0                      //3 int to 3 char
				x=x/10;                                  // get 12, we got 3
		i--;
	}
	for( ; i<=3;i++)
	{
		LCD_vidSendChar(arr[i]);
	}
}
