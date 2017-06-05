/*
*
* main.c
* Denna kod initerar alla funktioner som man behöver.
* Author: Michael Nilsson och Martin Bråhagen
*
*/

#include <asf.h>
#include "InitADC_DAC/adcdac.h"
#include "Initinterrupt/init_tc.h"
#include "Filter/Filter.h"
#include "Delay/DelayFunctions.h"
#include "LCD/LCDFunctions.h"
#include "LCD/lcdApplication.h"
uint32_t sampelfrekvens;
uint8_t numberofband;
void configure_console(void){
	const usart_serial_options_t uart_serial_options ={
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART,&uart_serial_options);
}

/************************************************************************/
/* Initerar de viktiga saker man behöver                                */
/************************************************************************/
int main (void)
{
	uint32_t sampel;
	sysclk_init();
	board_init();
	ioport_init();
	ioport_set_pin_dir(CHECK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(pin20,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(pin20,LOW);
	ioport_set_pin_dir(pin21,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(pin21,LOW);
	configure_console();
	adc_setup();
	dac_setup();
	configure_tc();
	delayInit();
	lcdInit();
	//LCD SETUP
	while(1){
	lcdClearDisplay();
		delayMicroseconds(50000);
		sampel= (42000000/sampelfrekvens);
		lcdWrite4DigitNumber(sampel);
		delayMicroseconds(50000);
		lcdWriteAsciiString("Hz ");
		delayMicroseconds(50000);
		 if((ioport_get_pin_level(pin20)==0) &&  (ioport_get_pin_level(pin21)==0)){
			 lcdWriteAsciiString("Inget");
			 delayMicroseconds(50000);
		 }else if((ioport_get_pin_level(pin20)==1) && (ioport_get_pin_level(pin21)==0)){		
			lcdWriteAsciiString("1Band ");
			delayMicroseconds(50000);
			lcdWrite(numberofband,HIGH);
			delayMicroseconds(50000);
		}else if((ioport_get_pin_level(pin20)==0) && (ioport_get_pin_level(pin21)==1)){
			lcdWriteAsciiString("1Stop ");
			delayMicroseconds(50000);
			lcdWrite(numberofband,HIGH);
			delayMicroseconds(50000);
		}else if((ioport_get_pin_level(pin20)==1) && (ioport_get_pin_level(pin21)==1)){
		lcdWriteAsciiString("4Band");
		delayMicroseconds(50000);
		}
	}
}

