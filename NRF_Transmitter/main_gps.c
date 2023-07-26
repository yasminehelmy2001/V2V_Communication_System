/**
 * @file main.c
 * @brief NRF24L01+ transmit test
 *
 * @author
 * @version
 * @date Jan 8, 2015
 * @copyright
 */
#include "types.h"
#include "bit.h"
#include <stdbool.h>
#include "DIO_interface.h"
#include "LCD_cfg.h"
#include "LCD_interface.h"
#include "stdio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "nrf24l01.h"
#include "string.h"

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define MOSI_BIT	5
#define MISO_BIT	6

#define SCK_BIT		7
#define SS_BIT		4
#define CSN_HIGH()	SS_HIGH()
#define SS_HIGH()		(SPI_PORT |= (1 << SS_BIT))

#define USART_BAUDRATE 9600
//#define USART_BAUDRATE 19200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void usart_init();
unsigned int usart_getch();
void get_latlong(u8 lati_value[],u8*lati_dir,u8 longi_value[], u8*longi_dir);
void SPI_disable();
void usart_disable();

char packet[16] = {"None"},lati_value[16]={"None"}, longi_value[16]={"None"};
u8 lati_dir='K',longi_dir, value, ch, i ,j;
u8 last_saved_lati_value[16];

#define RED_LED					PB2
#define RED_LED_DDR				DDRB
#define RED_LED_PORT			PORTB

#define RED_LED_OUT()			(RED_LED_DDR |= (1 << RED_LED))
#define RED_LED_ON()			(RED_LED_PORT |= (1 << RED_LED))
#define RED_LED_OFF()			(RED_LED_PORT &= ~(1 << RED_LED))
#define RED_LED_TOGGLE()		(RED_LED_PORT ^= (1 << RED_LED))


static uint8_t tx_buf[CONFIG_NRF_STATIC_PL_LENGTH];   //Message to send
static uint8_t addr[CONFIG_NRF_ADDR_LEN] = CONFIG_NRF_ADDRESS;

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
	RED_LED_OUT();
	RED_LED_ON();   //LED on if message received
	//nrf_init(NRF_MODE_PTX, addr);  //Set nRF to transmitter mode

	DIO_vidSetPortDir(DIO_PORTC, DIO_PORT_OUTPUT); //LCD Data Port
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN5, DIO_OUTPUT);  //LCD Control
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN6, DIO_OUTPUT);   //LCD Control
	DIO_vidSetPinDir(DIO_PORTD,DIO_PIN4, DIO_OUTPUT); //LCD Control


	//DIO_vidSetPinDir(DIO_PORTA,DIO_PIN1, DIO_OUTPUT);

	DIO_vidSetPinDir(DIO_PORTB,DIO_PIN3,DIO_OUTPUT);			/* Make PinB3 as output*/

	DIO_vidSetPinDir(DIO_PORTB,DIO_PIN1,DIO_INPUT);
//	DIO_vidSetPinVal(DIO_PORTB,DIO_PIN1,DIO_HIGH);


	LCD_vidInit();
	LCD_vidSendCommand(LCD_CLR);

	GICR|=(1<<6);   //enable interrupt zero
	MCUCR|=(1<<1);   //falling edge, counts how many holes in rotary encoder
		//sei();   //enable global interrupt

		TCNT0=192;      //load number
		TIMSK|=(1<<0);   //set overflow interrupt enable
		TCCR0|=(1<<1);   // 8 prescaler , start timer


		DDRD |= (1<<PD7);		/* Make OC2 pin as Output */

		ADC_Init();			/* Initialize ADC */
		TCNT2 = 0;			/* Set timer2 count zero */
		TCCR2 = (1<<WGM20)|(1<<WGM21)|(1<<COM21)|(1<<CS20)|(1<<CS21);/* Set Fast PWM with Fosc/64 Timer2 clock */


	while(1) {
		/* we separated the uart and spi protocols because they interfere with each other (I think their interrupts interfere with each other) */
		usart_init();
		get_latlong(lati_value,&lati_dir,longi_value,&longi_dir);

		usart_disable();
		_delay_ms(100);

		sei();

		nrf_init(NRF_MODE_PTX, addr);  //Set nRF to transmitter mode
		sprintf(packet,"LAV%s%c",lati_value,lati_dir);

		//_delay_ms(100);

		if(nrf_transmit_packet(packet, sizeof(packet)) != 0) {  //If packet not successfully transmitted
						_delay_ms(50);
						RED_LED_ON();    //Blink LED
						_delay_ms(50);
						RED_LED_OFF();   //Blink LED
					}

		_delay_ms(100);

		sprintf(packet,"LOV%s%c",longi_value,longi_dir);
		if(nrf_transmit_packet(packet, sizeof(packet)) != 0) {  //If packet not successfully transmitted
							_delay_ms(50);
							RED_LED_ON();    //Blink LED
							_delay_ms(50);
							RED_LED_OFF();   //Blink LED
						}

		DIO_vidSetPinVal(DIO_PORTB,DIO_PIN3,DIO_HIGH);  /* Rotate DC motor Clockwise */
		OCR2 = (ADC_Read(0)/4);	/* Read ADC and map it into 0-255 to write in OCR0 register */

		_delay_ms(100);

		sprintf((char *)tx_buf, "rps%u", rps); // format the integer and store in buffer

		if(nrf_transmit_packet(tx_buf, sizeof(tx_buf)) != 0) {  //If packet not successfully transmitted
				_delay_ms(50);
				RED_LED_ON();    //Blink LED
				_delay_ms(50);
				RED_LED_OFF();   //Blink LED
			}

		_delay_ms(100);

		u8 x;
		x=DIO_u8GetPinVal(DIO_PORTB,DIO_PIN1);
//		if(x==0)
//		{
//			LCD_vidSendCommand(0x80);
//			LCD_vidSendChar('g');
//		}
//		else if(x==1)
//		{
//			LCD_vidSendCommand(0x80);
//						LCD_vidSendChar('n');
//		}
		sprintf((char *)tx_buf, "St:%u", x); // format the integer and store in buffer
		if(nrf_transmit_packet(tx_buf, sizeof(tx_buf)) != 0) {  //If packet not successfully transmitted
						_delay_ms(50);
						RED_LED_ON();    //Blink LED
						_delay_ms(50);
						RED_LED_OFF();   //Blink LED
					}

		_delay_ms(100);

		SPI_disable();
		/*clear the global interrupt because it interferes with the uart when integrated with the spi*/
		cli();


}
}


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

//void get_latlong(u8 lati_value[],u8*lati_dir,u8 longi_value[], u8*longi_dir)
//
//{
//
//	last_saved_lati_value[8]=lati_value[8];
//	do
//	{
//	u8 value;
//	  value=usart_getch();
//	  if(value=='$')
//	  {
//
//	   value=usart_getch();
//
//	   if(value=='G')
//	   {
//
//	    value=usart_getch();
//
//	    if(value=='P')
//	    {
//
//	     value=usart_getch();
//
//	     if(value=='G')
//	     {
//
//	      value=usart_getch();
//
//	      if(value=='G')
//	      {
//
//	       value=usart_getch();
//
//	       if(value=='A')
//	       {
//
//	        value=usart_getch();
//	        if(value==',')
//	        {
//	         value=usart_getch();
//	         while(value!=',')
//	         {
//	          value=usart_getch();
//	         }
//	         lati_value[0]=usart_getch();
//	         value=lati_value[0];
//	         for(i=1;value!=',';i++)
//	         {
//	          lati_value[i]=usart_getch();
//	          value=lati_value[i];
//	         }
//	         *lati_dir=usart_getch();
//	         value=usart_getch();
//	        /* while(value!=',')
//	         {
//	          value=usart_getch();
//	         }*/
//	         longi_value[0]=usart_getch();
//	         value=longi_value[0];
//	         for(i=1;value!=',';i++)
//	         {
//	          longi_value[i]=usart_getch();
//	          value=longi_value[i];
//	         }
//	         *longi_dir=usart_getch();
//
//
//             LCD_vidSendCommand(0x80);
//             LCD_vidWriteStr(lati_value);
//             LCD_vidSendChar(*lati_dir);
//
//             LCD_vidSendCommand(0xC0);
//             LCD_vidWriteStr(longi_value);
//             LCD_vidSendChar(*longi_dir);
//
//	        }
//	          }
//	         }
//	        }
//	       }
//	      }
//	  }
//	}while(*lati_dir=='K'||last_saved_lati_value[8]==lati_value[8]);
/*Do while (*lati_dir=='K') is written because when the gps module works on its own, it keeps looping the gps loop until it finds the cooordinates and
	displays it, here we have multiple codes, so the gps code msh byl7a2 ygeeb el coordinates before it moves on to the next codes. it loops the gps code,
	doesnt find the coordinates, then goes to the nrf code and so on. fa bydaya3 data gaya mn el gps w howa bynafez el nrf code, so we added the do while code
	the lati=dir=='K' part is for the first time the code is executed, eno mayetla3sh meno ella law el lati_dir=N aw W (law et3'ayaret). w heya mt3arafa foo2 b K.
	In the code above we saved the last digit of the old latitude value, and we compared it to the new latitude value, this is to always update the
	gps value and to not leave the gps code if the latitude value doesnt change because the last digit is very sensitive and it always changes*/
//}

void get_latlong(u8 lati_value[], u8* lati_dir, u8 longi_value[], u8* longi_dir)
{
    last_saved_lati_value[8] = lati_value[8];

    u8 gpgga_found = 0; // Flag to track if "$GPGGA" is found

    do
    {
        u8 value;
        value = usart_getch();

        if (value == '$')
        {
            value = usart_getch();

            if (value == 'G')
            {
                value = usart_getch();

                if (value == 'P')
                {
                    value = usart_getch();

                    if (value == 'G')
                    {
                        value = usart_getch();

                        if (value == 'G')
                        {
                            value = usart_getch();

                            if (value == 'A')
                            {
                                gpgga_found = 1; // Set the flag when "$GPGGA" is found
                            }
                        }
                    }
                }
            }
        }

        if (gpgga_found)
        {
        	value=usart_getch();
        		        if(value==',')
        		        {
        		         value=usart_getch();
        		         while(value!=',')
        		         {
        		          value=usart_getch();
        		         }
        		         lati_value[0]=usart_getch();
        		         value=lati_value[0];
        		         for(i=1;value!=',';i++)
        		         {
        		          lati_value[i]=usart_getch();
        		          value=lati_value[i];
        		         }
        		         *lati_dir=usart_getch();
        		         value=usart_getch();
        		        /* while(value!=',')
        		         {
        		          value=usart_getch();
        		         }*/
        		         longi_value[0]=usart_getch();
        		         value=longi_value[0];
        		         for(i=1;value!=',';i++)
        		         {
        		          longi_value[i]=usart_getch();
        		          value=longi_value[i];
        		         }
        		         *longi_dir=usart_getch();


        	             LCD_vidSendCommand(0x80);
        	             LCD_vidWriteStr(lati_value);
        	             LCD_vidSendChar(*lati_dir);

        	             LCD_vidSendCommand(0xC0);
        	             LCD_vidWriteStr(longi_value);
        	             LCD_vidSendChar(*longi_dir);
        		        }

        }

    } while (!gpgga_found);
}




void SPI_disable()
{
	/* Set MOSI, SCK, SS as ouput */
	SPI_DDR |= (1 << MOSI_BIT)|(1 << SCK_BIT)|(1 << SS_BIT);

	/* Slave Select pin is initially HIGH */
	SS_HIGH();

	/* Initialize SPI */
	SPCR &= ~(1 << SPE); /* Disable SPI */

}

void usart_disable()
{
	UCSRB &= ~((1<<RXCIE) | (1 << RXEN) | (1 << TXEN));
	UCSRC &= ~(1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

ISR(INT0_vect)
{
	//cli();
	counter++;   //incremenet number of holes
	//sei();
}

ISR(TIMER0_OVF_vect)
{
	//cli();
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
		LCD_PRINT_INT(rps);
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

	//sei();
}

