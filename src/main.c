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
	
	sysclk_init();
	board_init();
	ioport_init();
	ioport_set_pin_dir(CHECK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CHANGE_MODE,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CHANGE_MODE,LOW);
	configure_console();
	adc_setup();
	dac_setup();
	configure_tc();
	while(1);			//Gör ingeting i main programmet
}

