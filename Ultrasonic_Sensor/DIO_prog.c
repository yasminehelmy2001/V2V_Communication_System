/*
 * DIO_prog.c
 *
 *  Created on: Sep 3, 2022
 *      Author: yasmi
 */

#include "bit.h"
#include "types.h"
#include "DIO_interface.h"
#include "DIO_priv.h"               //not include .c files, linker links them




void DIO_vidSetPinDir(u8 u8PortId, u8 u8PinId, u8 u8Dir)
{
	if((u8PortId <=DIO_PORTD)&& (u8PinId<=DIO_PIN7))
	{
	if(u8Dir == DIO_INPUT)
	{
		switch(u8PortId)

		{
		case DIO_PORTA: CLR_BIT(DDRA_REG, u8PinId );break;
		case DIO_PORTB: CLR_BIT(DDRB_REG, u8PinId );break;
		case DIO_PORTC: CLR_BIT(DDRC_REG, u8PinId );break;
		case DIO_PORTD: CLR_BIT(DDRD_REG, u8PinId );break;
		}
	}
	else if (u8Dir == DIO_OUTPUT)
		{
			switch(u8PortId)

			{
			case DIO_PORTA: SET_BIT(DDRA_REG, u8PinId );break;
			case DIO_PORTB: SET_BIT(DDRB_REG, u8PinId );break;
			case DIO_PORTC: SET_BIT(DDRC_REG, u8PinId );break;
			case DIO_PORTD: SET_BIT(DDRD_REG, u8PinId );break;
			}
		}

	else
	{

	}
	}
	else
	{

	}

}
void DIO_vidSetPinVal(u8 u8PortId, u8 u8PinId, u8 u8Val)
{
	if((u8PortId <=DIO_PORTD)&& (u8PinId<=DIO_PIN7))
		{
		if(u8Val == DIO_LOW)
		{
			switch(u8PortId)

			{
			case DIO_PORTA: CLR_BIT(PORTA_REG, u8PinId );break;
			case DIO_PORTB: CLR_BIT(PORTB_REG, u8PinId );break;
			case DIO_PORTC: CLR_BIT(PORTC_REG, u8PinId );break;
			case DIO_PORTD: CLR_BIT(PORTD_REG, u8PinId );break;
			}
		}
		else if (u8Val == DIO_HIGH)
			{
				switch(u8PortId)

				{
				case DIO_PORTA: SET_BIT(PORTA_REG, u8PinId );break;
				case DIO_PORTB: SET_BIT(PORTB_REG, u8PinId );break;
				case DIO_PORTC: SET_BIT(PORTC_REG, u8PinId );break;
				case DIO_PORTD: SET_BIT(PORTD_REG, u8PinId );break;
				}
			}
		else
		{

		}
		}
		else
		{

		}

}
void DIO_vidSetPortVal(u8 u8PortId, u8 u8Val)
{
	if(u8PortId<=DIO_PORTD)
	{
		switch(u8PortId)

						{
						case DIO_PORTA: PORTA_REG= u8Val;break;
						case DIO_PORTB: PORTB_REG= u8Val;break;
						case DIO_PORTC: PORTC_REG= u8Val;break;
						case DIO_PORTD: PORTD_REG= u8Val;break;
						}
	}
	else
	{

	}
}
void DIO_vidSetPortDir(u8 u8PortId, u8 u8Dir)
{
	if(u8PortId<=DIO_PORTD)
		{
			switch(u8PortId)

							{
							case DIO_PORTA: DDRA_REG= u8Dir;break;
							case DIO_PORTB: DDRB_REG= u8Dir;break;
							case DIO_PORTC: DDRC_REG= u8Dir;break;
							case DIO_PORTD: DDRD_REG= u8Dir;break;
							}
		}

		else
		{

		}
}
u8 DIO_u8GetPinVal(u8 u8PortId, u8 u8PinId)
{
	if((u8PortId<=DIO_PORTD)&&(u8PinId<=DIO_PIN7))
			{
				switch(u8PortId)

								{
								case DIO_PORTA: return GET_BIT(PINA_REG,u8PinId);break;
								case DIO_PORTB: return GET_BIT(PINB_REG,u8PinId);break;
								case DIO_PORTC:return GET_BIT(PINC_REG,u8PinId);break;
								case DIO_PORTD: return GET_BIT(PIND_REG,u8PinId);break;
								}
			}
			else
			{

			}
}
