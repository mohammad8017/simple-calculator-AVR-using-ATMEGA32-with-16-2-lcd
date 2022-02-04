#ifndef F_CPU
#define F_CPU 1600000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

#include "LCD!/lcd.h"


int state = 1;

void adc_init()
{
	ADMUX = (1<<REFS0);

	// ADC Enable and prescaler of 128
	
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;

	ADCSRA |= (1<<ADSC);

	
	while(ADCSRA & (1<<ADSC));

	return (ADC);
}

int main()
{
	DDRB=0xff;
	DDRC = 0xFF;
	uint16_t adc_result0;
	int temp;
	char buffer[10];
	

	// initialize adc and lcd
	adc_init();
	lcd_init(LCD_DISP_ON_CURSOR); //CURSOR

	
	lcd_clrscr();
	lcd_gotoxy(0,0);
	
	

	_delay_ms(50);

	while(1)
	{
		adc_result0 = adc_read(0);      // read adc value at PA0
		temp=adc_result0/2.01;   // finding the temperature
		
		
		
		lcd_gotoxy(0,0);
		itoa(temp,buffer,10);
		lcd_puts("Temp=");   //display temperature
		lcd_puts(buffer);
		lcd_gotoxy(7,0);
		lcd_puts("C");
		
		if(state==1)
		{
			PORTC=(0<<PINC1); //cooler
			PORTB=(0<<PINB1); //heater

			if(temp>35)
			{
				state=2;
				lcd_clrscr();
				lcd_home();
				lcd_gotoxy(0,1);
				lcd_puts("Cooler ON");
				
				PORTC=(1<<PINC1); //cooler
				PORTB=(0<<PINB1); //heater
				
			}
			
			if(temp<15)
			{
				state=3;
				lcd_clrscr();
				lcd_home();
				lcd_gotoxy(0,1);
				lcd_puts("Heater ON");
				
				PORTC=(0<<PINC1); //cooler
				PORTB=(1<<PINB1); //heater
			}
		}
		if(state==2)
		{

			if(temp<25)
			{
				state=1;
				lcd_clrscr();
				lcd_home();
				lcd_gotoxy(0,1);
				lcd_puts("---");
				
				PORTC=(0<<PINC1); //cooler
				PORTB=(0<<PINB1); //heater
			}
			
		}
		if(state==3)
		{
			PORTC=(0<<PINC1); //cooler
			PORTB=(1<<PINB1); //heater

			if(temp>30)
			{
				state=1;
				lcd_clrscr();
				lcd_home();
				lcd_gotoxy(0,1);
				lcd_puts("---");
		
				PORTC=(0<<PINC1); //cooler
				PORTB=(0<<PINB1); //heater
			}
	
		}
	}
}