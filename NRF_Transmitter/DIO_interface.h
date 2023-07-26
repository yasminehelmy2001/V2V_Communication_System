/*
 * DIO_interface.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Rago
 */

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_
/************Public functions***********/

/*Description: this function is to set pin direction
 *INPUT      : u8PortID    , Range: DIO_PORTA~DIO_PORTD
 *           : u8PinId     ,*/
void DIO_vidSetPinDir(u8 u8PortId,u8 u8PinId,u8 u8Dir);
void DIO_vidSetPinVal(u8 u8PortId,u8 u8PinId,u8 u8Val);
void DIO_vidSetPortDir(u8 u8PortId,u8 u8Dir);
void DIO_vidSetPortVal(u8 u8PortId,u8 u8Val);
u8   DIO_u8GetPinVal(u8 u8PortId,u8 u8PinId);

/************Public MACROS***********/
// port MACROS
#define DIO_PORTA  0
#define DIO_PORTB  1
#define DIO_PORTC  2
#define DIO_PORTD  3

// Pin MACROS
#define DIO_PIN0  0
#define DIO_PIN1  1
#define DIO_PIN2  2
#define DIO_PIN3  3
#define DIO_PIN4  4
#define DIO_PIN5  5
#define DIO_PIN6  6
#define DIO_PIN7  7
// DIO status
#define DIO_HIGH  1
#define DIO_LOW   0
// DIO Direction
#define DIO_INPUT    0
#define DIO_OUTPUT   1

#define DIO_PORT_OUTPUT 0xFF
#define DIO_PORT_INPUT   0
#define DIO_PORT_HIGH 0xFF
#define DIO_PORT_LOW   0



#endif /* DIO_INTERFACE_H_ */
