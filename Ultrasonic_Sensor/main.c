#include "types.h"
#include "bit.h"
#include "DIO_interface.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/delay.h>

#include "LCD_cfg.h"
#include "LCD_interface.h"

u16 pulse=0, i=0;

int main (void)
{
	u16 distance;
	u8 arr1[]={"Distance="};
		u8 arr2[]={"cm  "};

	//buzzer
	DIO_vidSetPinDir(DIO_PORTC, DIO_PIN7, DIO_OUTPUT);

	//LCD data pins
	DIO_vidSetPortDir(DIO_PORTB, DIO_PORT_OUTPUT);
	DIO_vidSetPinDir(DIO_PORTC,DIO_PIN0, DIO_OUTPUT);
	DIO_vidSetPinDir(DIO_PORTC,DIO_PIN1, DIO_OUTPUT);
	DIO_vidSetPinDir(DIO_PORTC,DIO_PIN2, DIO_OUTPUT);

	//trigger
	DIO_vidSetPinDir(DIO_PORTD, DIO_PIN0, DIO_OUTPUT);

	//interrupt
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC00);

	sei();// enabling global interrupts
	//LCD Initialization

		LCD_vidInit();


	while(1)

	{
		DIO_vidSetPinVal(DIO_PORTD, DIO_PIN0, DIO_HIGH);
		_delay_us(10);
		DIO_vidSetPinVal(DIO_PORTD, DIO_PIN0, DIO_LOW);
		distance = pulse/58;

		//LCD
		LCD_vidSendCommand(0x80);      //cursor on first place on first line
		LCD_vidWriteStr(arr1);
		LCD_PRINT_INT(distance);
		LCD_vidWriteStr(arr2);

		//buzzer
		if(distance<70)
		{
			DIO_vidSetPinVal(DIO_PORTC, DIO_PIN7, DIO_HIGH);
		}
		else
		{
			DIO_vidSetPinVal(DIO_PORTC, DIO_PIN7, DIO_LOW);
		}
	}
}

ISR(INT0_vect)//interrupt service routine when there is a change in logic level

{

if (i==1)//when logic from HIGH to LOW

{

TCCR1B=0;//disabling counter

pulse=TCNT1;//count memory is updated to integer

TCNT1=0;//resetting the counter memory

i=0;

}

if (i==0)//when logic change from LOW to HIGH

{

TCCR1B|=(1<<CS10);//enabling counter

i=1;

}

}
