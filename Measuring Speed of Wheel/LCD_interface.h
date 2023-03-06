/*
 * LCD_interface.h
 *
 *  Created on: Sep 16, 2022
 *      Author: yasmi
 */

#ifndef LCD_LCD_INTERFACE_H_
#define LCD_LCD_INTERFACE_H_

#define LCD_CLR              0x01
#define LCD_HOME             0x02
#define LCD_ENTRYMODE        0x06
#define LCD_DISPOFF          0x08
#define LCD_DISPON_CURSON    0x0E
#define LCD_FUNCRESET        0x30
#define LCD_FUNCSET_8BIT     0x38
#define LCD_SETCURSOR        0x80
#define LCD_DISPON_CURSBLINK 0x0F
#define LCD_DISPON_CURSOFF   0x0C
/***************************Public Functions**********************/

void LCD_vidInit(void);

void LCD_vidSendCommand(u8 u8Cmd);

void LCD_vidSendChar(u8 u8Char);

void LCD_vidWriteStr(u8* pu8Str);

void LCD_vidGoToXY(u8 x, u8 y);

void LCD_Custom_Char (u8 loc, u8 msg[]);

void LCD_PRINT_INT(u16 x);

#endif /* LCD_LCD_INTERFACE_H_ */
