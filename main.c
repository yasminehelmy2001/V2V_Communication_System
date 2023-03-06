/*
 * main.c
 *
 *  Created on: Mar 5, 2023
 *      Author: yasmi
 */

/*
 * ATmega16 DC Motor control
 * http://www.electronicwings.com
 */

#include <avr/io.h>			/* Include AVR std. library file */
#include <avr/interrupt.h>
#include <stdio.h>			/* Include std. library file */
#include <util/delay.h>			/* Include Delay header file */
#include "types.h"
#include "bit.h"
#include "DIO_interface.h"
#include <avr/interrupt.h>
#include "LCD_interface.h"


u16 counter=0, ov_count=0, rps=0;         //U16!!!
u8 str[]={"RPS="}, str2[]={"No Wheel!"};

void ADC_Init()				/* ADC Initialization function */
{
	DDRA = 0x00;			/* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, with freq/128 */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)		/* ADC Read function */
{
	ADMUX = 0x40 | (channel & 0x07);/* set input channel to read */
	ADCSRA |= (1<<ADSC);		/* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));	/* Wait until end of conversion */
	ADCSRA |= (1<<ADIF);		/* Clear interrupt flag */
	_delay_ms(1);			/* Wait a little bit */
	return ADCW;			/* Return ADC word */
}


int main(void)
{

	DIO_vidSetPortDir(DIO_PORTC, DIO_PORT_OUTPUT);// LCD Data
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN5, DIO_OUTPUT);  //LCD Control
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN6, DIO_OUTPUT);   //LCD Control
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN4, DIO_OUTPUT); //LCD Control

	DDRB = 0xFF;			/* Make PORTB as output Port */

	LCD_vidInit(); //LCD Initialization after setting ports
	LCD_vidSendCommand(LCD_CLR);

	GICR|=(1<<6);   //enable interrupt zero
	MCUCR|=(1<<1);   //falling edge, counts how many holes in rotary encoder
	sei();   //enable global interrupt

	TCNT0=192;      //load number
	TIMSK|=(1<<0);   //set overflow interrupt enable
	TCCR0|=(1<<1);   // 8 prescaler , start timer

	LCD_vidSendCommand(0x80);


	DDRD |= (1<<PD7);		/* Make OC2 pin as Output */

	ADC_Init();			/* Initialize ADC */
	TCNT2 = 0;			/* Set timer2 count zero */
	TCCR2 = (1<<WGM20)|(1<<WGM21)|(1<<COM21)|(1<<CS20)|(1<<CS21);/* Set Fast PWM with Fosc/64 Timer2 clock */

	while(1)
	{
		PORTB = 1;    /* Rotate DC motor Clockwise */
		OCR2 = (ADC_Read(0)/4);	/* Read ADC and map it into 0-255 to write in OCR0 register */
	}
}

ISR(INT0_vect)
{
	counter++;   //incremenet number of holes
}

ISR(TIMER0_OVF_vect)
{
	ov_count++;


	if(ov_count==3906)   //count one second
	{
		//DIO_vidSetPinVal(DIO_PORTD,DIO_PIN0,DIO_HIGH);
			//_delay_ms(100);
		//	DIO_vidSetPinVal(DIO_PORTD,DIO_PIN0,DIO_LOW);
			//_delay_ms(100);

		TCCR0=0;  //stop counter
		rps= counter/20;   //rps calculation


		if(counter!=0)
		{
		LCD_vidSendCommand(LCD_CLR);
		LCD_vidSendCommand(0x80);
		LCD_vidWriteStr(str);
		LCD_PRINT_INT(counter);
		}
		else
		{
			LCD_vidSendCommand(LCD_CLR);
			LCD_vidSendCommand(0x80);
			LCD_vidWriteStr(str2);
		}

		counter=0;   //zero counter
		TCNT0=192;   //load number
		ov_count=0;  //zero overflow counter
		TCCR0|=(1<<1);   // 8 prescaler , start timer

	}
}
