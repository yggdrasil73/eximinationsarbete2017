/*
* adcdac.c
* Denna fil initierar ADC och DAC
*
* Author: Michael Nilsson och Martin Bråhagen
*/
#include <asf.h>
#include "initADC_DAC/adcdac.h"

/************************************************************************/
/* Initierar ADC för pinne  A0-A2 och A8                                */
/************************************************************************/
void adc_setup()
{
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, sysclk_get_main_hz(), 20000000, 0);		// Konfiguerar för maximal frekvens på 84 MHz
	adc_configure_timing(ADC, 0, 0, 0);
	adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);			// Sätter upplösningen på 12 bitar
	adc_enable_channel(ADC, ADC_CHANNEL_10);				// Analog pin 8
	adc_enable_channel(ADC, ADC_CHANNEL_7);					// Analog pin 0
	adc_enable_channel(ADC, ADC_CHANNEL_6);					// Analog pin 1
	adc_enable_channel(ADC, ADC_CHANNEL_5);					// Analog pin 2
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);				
}



/************************************************************************/
/* Initierar DAC för pin DAC1                                           */
/************************************************************************/
void dac_setup() {
	pmc_enable_periph_clk(ID_DACC);
	dacc_reset(DACC);
	dacc_set_transfer_mode(DACC, 0);
	dacc_set_timing(DACC,1,1,0);			// 1 är den kortade uppdateringsperiod, 1 max speed, 0 start upp tid
	
	dacc_set_channel_selection(DACC,1);		// Channel DAC1
	dacc_enable_channel(DACC, 1);			// Startar DAC1
}

