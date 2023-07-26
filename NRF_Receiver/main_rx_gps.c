#include "types.h"
#include "bit.h"
#include "DIO_interface.h"
#include "LCD_cfg.h"
#include "LCD_interface.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "nrf24l01.h"


static uint8_t rx_buf[CONFIG_NRF_STATIC_PL_LENGTH];   //16
static uint8_t lati_value[CONFIG_NRF_STATIC_PL_LENGTH]={"None"};   //16
static uint8_t longi_value[CONFIG_NRF_STATIC_PL_LENGTH]={"None"};   //16
static uint8_t lati_dir[CONFIG_NRF_STATIC_PL_LENGTH];   //16
static uint8_t longi_dir[CONFIG_NRF_STATIC_PL_LENGTH];   //16

static uint8_t addr[CONFIG_NRF_ADDR_LEN] = CONFIG_NRF_ADDRESS;
u8 drowsy=0;

u16 pulse=0, i=0;

int main(void)
{
	u16 distance;
		u8 arr1[]={"Distance="};
		u8 arr2[]={"cm  "};

		//buzzer for ultrasonic
		DIO_vidSetPinDir(DIO_PORTD, DIO_PIN7, DIO_OUTPUT);

		//echo
		DIO_vidSetPinDir(DIO_PORTB,DIO_PIN0,DIO_INPUT);
		DIO_vidSetPinVal(DIO_PORTB,DIO_PIN0,DIO_HIGH);

		//trigger
		DIO_vidSetPinDir(DIO_PORTD, DIO_PIN2, DIO_OUTPUT);

	uint8_t length;
	u8 i;
	DIO_vidSetPortDir(DIO_PORTC,DIO_PORT_OUTPUT);    //LCD Data Port
	DIO_vidSetPinDir(DIO_PORTD, DIO_PIN5,DIO_OUTPUT);    //LCD Control Pin
	DIO_vidSetPinDir(DIO_PORTD, DIO_PIN6,DIO_OUTPUT);    // LCD Control Pin
	DIO_vidSetPinDir(DIO_PORTD, DIO_PIN4,DIO_OUTPUT);   //LCD Control Pin

	DIO_vidSetPinDir(DIO_PORTA, DIO_PIN0,DIO_OUTPUT);    //Speed Led 0
		DIO_vidSetPinDir(DIO_PORTA, DIO_PIN1,DIO_OUTPUT); //Speed Led 1
		DIO_vidSetPinDir(DIO_PORTA, DIO_PIN2,DIO_OUTPUT); //Speed Led 2
		DIO_vidSetPinDir(DIO_PORTA, DIO_PIN3,DIO_OUTPUT); //Speed Led 3

	DIO_vidSetPinDir(DIO_PORTB,DIO_PIN3,DIO_OUTPUT); //buzzer for drowsiness

	LCD_vidInit();
	nrf_init(NRF_MODE_PRX, addr);  //Set mode to Receiver
	sei();   //Enable global Interrupt

	_delay_ms(100);
	while(1) {

		DIO_vidSetPinVal(DIO_PORTD, DIO_PIN2, DIO_HIGH);
			_delay_us(10);
			DIO_vidSetPinVal(DIO_PORTD, DIO_PIN2, DIO_LOW);

			while(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN0)==0);
				TCCR1B|=(1<<CS11);//enabling counter, prescaler= 8 to count in 1us ticks

			while(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN0)==1);
				TCCR1B=0;//disabling counter
				pulse=TCNT1;//count memory is updated to integer
				TCNT1=0;//resetting the counter memory

			distance = pulse/29;

			//buzzer
			if(distance<15)
			{
				DIO_vidSetPinVal(DIO_PORTD, DIO_PIN7, DIO_HIGH);
			}
			else
			{
				DIO_vidSetPinVal(DIO_PORTD, DIO_PIN7, DIO_LOW);
			}

		nrf_receive_packet(rx_buf, &length);    //Receive string in rx_buf
		if(rx_buf[0]=='S'&&rx_buf[1]=='t'&&rx_buf[2]==':')
						{
							if(rx_buf[3]=='0')
							{
								DIO_vidSetPinVal(DIO_PORTB,DIO_PIN3,DIO_HIGH);
							}


							else if(rx_buf[3]=='1')
							{
								DIO_vidSetPinVal(DIO_PORTB,DIO_PIN3,DIO_LOW);
							}
						}

		if(rx_buf[0]=='L'&&rx_buf[1]=='A'&&rx_buf[2]=='V')
		{
			for(i=0;i<=11;i++)
			{
				lati_value[i]=rx_buf[i+3];
			}
		}

		if(rx_buf[0]=='L'&&rx_buf[1]=='O'&&rx_buf[2]=='V')
				{
					for(i=0;i<=12;i++)
					{
						longi_value[i]=rx_buf[i+3];
					}
				}

		if(rx_buf[0]=='r'&&rx_buf[1]=='p'&&rx_buf[2]=='s')
		{
			if(rx_buf[3]=='0')
			{
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN0,DIO_HIGH);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN1,DIO_LOW);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN2,DIO_LOW);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN3,DIO_LOW);
			}
			else if(rx_buf[3]=='1')
			{
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN0,DIO_LOW);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN1,DIO_HIGH);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN2,DIO_LOW);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN3,DIO_LOW);
			}
			else if(rx_buf[3]=='2')
			{
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN0,DIO_LOW);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN1,DIO_LOW);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN2,DIO_HIGH);
				DIO_vidSetPinVal(DIO_PORTA, DIO_PIN3,DIO_LOW);
			}
			else if(rx_buf[3]=='3')
					{
						DIO_vidSetPinVal(DIO_PORTA, DIO_PIN0,DIO_LOW);
						DIO_vidSetPinVal(DIO_PORTA, DIO_PIN1,DIO_LOW);
						DIO_vidSetPinVal(DIO_PORTA, DIO_PIN2,DIO_LOW);
						DIO_vidSetPinVal(DIO_PORTA, DIO_PIN3,DIO_HIGH);
					}
		}


		LCD_vidSendCommand(0x80);
				LCD_vidWriteStr(lati_value);
				LCD_vidWriteStr(lati_dir);
		LCD_vidSendCommand(0xC0);
						LCD_vidWriteStr(longi_value);
						LCD_vidWriteStr(longi_dir);

//		LCD_vidSendCommand(0xC0);
//			LCD_vidWriteStr(rx_buf2);


		}
	}


