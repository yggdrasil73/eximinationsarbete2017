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


/************************************************************************/
/* Initerar de viktiga saker man behöver                                */
/************************************************************************/
int main (void)
{

	sysclk_init();
	board_init();
	ioport_init();
	ioport_set_pin_dir(CHECK_PIN, IOPORT_DIR_OUTPUT);	
	adc_setup();
	dac_setup();
	configure_tc();
	while(1);			//Gör ingeting i main programmet
}

