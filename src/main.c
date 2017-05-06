/*
*
* main.c
* Denna kod initerar alla funktioner som man beh�ver.
* Author: Michael Nilsson och Martin Br�hagen
*
*/

#include <asf.h>
#include "InitADC_DAC/adcdac.h"
#include "Initinterrupt/init_tc.h" 
#include "Filter/Filter.h"


/************************************************************************/
/* Initerar de viktiga saker man beh�ver                                */
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
	while(1);			//G�r ingeting i main programmet
}

