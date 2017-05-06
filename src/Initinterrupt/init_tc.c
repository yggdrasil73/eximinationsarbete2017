/*
*
* init_tc.c
* Denna fil initerar interruptet
* Author: Michael Nilsson och Martin Bråhagen
*/
#include <asf.h>
#include "Initinterrupt/init_tc.h"


/************************************************************************/
/* Denna metod initierar interruptet                                    */
/* 42000000 -> 2100 - 20000												*/
/************************************************************************/
void configure_tc(void)
{
	/* Konfiguerar PMC */
	pmc_enable_periph_clk(ID_TC0);

	/** Konfiguerar TC för 20 khz frevkens och triggar på RC jämförelse. */
	tc_init(TC0, 0, 0 | TC_CMR_CPCTRG);			//Timer_clock_1 - MCK/2 - 42 MHz
	tc_write_rc(TC0, 0, 2100);					//2100 -> fs = 20 kHz

	/* Konfiguerar och sätter på interruptet på RC-jämförelsen*/
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);

	tc_start(TC0, 0);
}

