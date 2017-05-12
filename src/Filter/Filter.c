/*
* Filter.c
*
* Denna fil filtrerar ljudet från ADC:n
* Author: Michael Nilsson och Martin Bråhagen
*/
#include <asf.h>
#include "Filter/Filter.h"
#include "Filter/oneHzToTwoFivekHz.h"
#include "Filter/twofiveToFivekHz.h"
#include "Filter/fiveToSevenkHz.h"
#include "Filter/sevenToNinekHz.h"
#include "Filter/movingFilter.h"
uint32_t sampelfrekvens;

/**
*  Hanterar interruptet som då utför filtret
*/
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;
	uint32_t invalue, outvalue;
	uint32_t out0,out1,out2,out3,outmovingfilter;
	uint32_t analogpin0,analogpin1,analogpin2,analogpin3;
	

	
	ul_dummy = tc_get_status(TC0, 0);

	
	UNUSED(ul_dummy);
	
	ioport_set_pin_level(CHECK_PIN,HIGH);		// Sätter pin 22 hög
	adc_start(ADC);								// Startar ADC
	while((adc_get_status(ADC) & 0x1<<24)==0);  // väntar till DRDY blir hög

	invalue=adc_get_channel_value(ADC,ADC_CHANNEL_10);			// Läser av värdet på analogpin8 som är själva ljud signalen

	if(ioport_get_pin_level(CHANGE_MODE)==1){
		analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_7);
		//printf("%i\n",1);
		outmovingfilter = movingFilterkHz(invalue);
		sampelfrekvens = MovingFilterfrekvens(analogpin0);
		tc_write_rc(TC0, 0, sampelfrekvens);
		//OUTPUT
		outvalue = (outmovingfilter)+2048;	//lägger till filter på outvalue som skickas till DAC. 2048 lägger till offset!! på ~1.4 V
	}else if(ioport_get_pin_level(CHANGE_MODE)==0){
		tc_write_rc(TC0, 0, 2100);
		//Filter 1-2750 Hz
		out0 = 0;
		out0 = oneHzToTwoFivekHz(invalue);//FUNGERAR BRA MEN SIGNALEN GÅR UPP LÅNGSAMT
		//analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_7);
		//out0 = modifyOutPut(out0,analogpin0);
		//Filter 2250-5250 Hz
		out1 = 0;
		out1 = twofiveToFivekHz(invalue);//FUNGERAR BRA
		//analogpin1 = adc_get_channel_value(ADC,ADC_CHANNEL_6);
		//out1 = modifyOutPut(out1,analogpin1);
		//Filter 4750-7750 Hz
		out2 = 0;
		out2 = fiveToSevenkHz(invalue);//FUNGERAR BRA
		//analogpin2 = adc_get_channel_value(ADC,ADC_CHANNEL_5);
		//out2 = modifyOutPut(out2,analogpin2);
		//Filter 7250-9999 Hz
		out3 = 0;
		out3 = sevenToNinekHz(invalue);//FUNGERAR BRA
		//analogpin3 = adc_get_channel_value(ADC,ADC_CHANNEL_4);
		//out3 = modifyOutPut(out3,analogpin3);
		
		//OUTPUT
		outvalue = (out0+out1+out2+out3)+2048;	///lägger till filter på outvalue som skickas till DAC. 2048 lägger till offset!! på ~1.4 V
	}

	dacc_write_conversion_data(DACC,outvalue);	// Skickar ut värdet på DAC
	
	ioport_set_pin_level(CHECK_PIN,LOW);		// Sätter pin22 låg
	
}



//Modifierar filtervärdet man har fått ut av filtret baserat på potentiometer
uint32_t modifyOutPut(uint32_t filtervalue,uint32_t analogValue){

	if(analogValue <= 819){
		return 0;
		}else if((analogValue >819)&&(analogValue<=1638)){
		return (filtervalue/4);
		}else if((analogValue>1638)&&(analogValue<=2457)){
		return (filtervalue/2);
		}else if((analogValue>2457)&&(analogValue<3276)){
		return ((filtervalue*3)/4);
		}else if((analogValue>3276)&&(analogValue<4095)){
		return filtervalue;
	}
}

uint32_t MovingFilterfrekvens(uint32_t analogmovingfilter){

	if(analogmovingfilter<=372){
		return sampelfrekvens=14483;//Fs = 2900 Hzbandet: 1131-1421
		}else if(analogmovingfilter>372 && analogmovingfilter<=744){
		return sampelfrekvens=11667;//Fs = 3600 Hzbandet: 1404-1764
		}else if(analogmovingfilter>744 && analogmovingfilter<=1116){
		return sampelfrekvens=9333;//Fs = 4500 Hzbandet: 1755-2205
		}else if(analogmovingfilter>1116 && analogmovingfilter<=1488){
		return sampelfrekvens=7636;//Fs =5500 Hzbandet: 2145-2695
		}else if(analogmovingfilter>1488 && analogmovingfilter<=1860){
		return sampelfrekvens=6461;//Fs = 6500 Hzbandet: 2535-3185
		}else if(analogmovingfilter>1860 && analogmovingfilter<=2232){
		return sampelfrekvens=5250;//Fs = 8000 Hzbandet: 3120-3920
		}else if(analogmovingfilter>2232 && analogmovingfilter<=2604){
		return sampelfrekvens=4200;//Fs = 10000 Hzbandet: 3900-4900
		}else if(analogmovingfilter>2604 && analogmovingfilter<=2976){
		return sampelfrekvens=3818;//Fs = 11000 Hzbandet: 4290-5390
		}else if(analogmovingfilter>2976 && analogmovingfilter<=3348){
		return sampelfrekvens=3231;//Fs = 13000 Hzbandet: 5070-6370
		}else if(analogmovingfilter>3348 && analogmovingfilter<=3720 ){
		return sampelfrekvens=2625;//Fs = 16000 Hzbandet: 6240-7820
		}else if(analogmovingfilter>3720 && analogmovingfilter<= 4096){
		return sampelfrekvens=2100;//Fs = 20000 Hzbandet: 7800-9800
	}
}