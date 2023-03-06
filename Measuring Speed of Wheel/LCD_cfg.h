/*
 * LCD_cfg.h
 *
 *  Created on: Sep 16, 2022
 *      Author: yasmi
 */

#ifndef LCD_LCD_CFG_H_
#define LCD_LCD_CFG_H_

//Description: LCD Data Port config
//range: DIO_u8PORTA ~ DIO_u8PORTD

#define LCD_DATA_PORT    DIO_PORTC

//Description: LCD Control Port config
//Range: DIO_u8PORTA ~ DIO_u8PORTD

#define LCD_CONTROL_PORT      DIO_PORTD

#define LCD_RS_PIN   DIO_PIN5

#define LCD_RW_PIN   DIO_PIN6

#define LCD_E_PIN    DIO_PIN4

#endif /* LCD_LCD_CFG_H_ */
