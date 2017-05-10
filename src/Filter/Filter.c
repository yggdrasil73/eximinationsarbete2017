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
/**
*  Hanterar interruptet som då utför filtret
*/
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;
	uint32_t invalue, outvalue;
	uint32_t out0,out1,out2,out3;
	uint32_t analogpin0,analogpin1,analogpin2,analogpin3;

	
	ul_dummy = tc_get_status(TC0, 0);

	
	UNUSED(ul_dummy);
	
	ioport_set_pin_level(CHECK_PIN,HIGH);		// Sätter pin 22 hög
	adc_start(ADC);								// Startar ADC
	while((adc_get_status(ADC) & 0x1<<24)==0);  // väntar till DRDY blir hög

	invalue=adc_get_channel_value(ADC,ADC_CHANNEL_10);			// Läser av värdet på analogpin8 som är själva ljud signalen
	
	//Filter 1-2500 Hz
	out0 = 0;
	out0 = oneHzToTwoFivekHz(invalue);//FUNGERAR BRA MEN SIGNALEN GÅR UPP LÅNGSAMT
	//analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_7);
	//out0 = modifyOutPut(out0,analogpin0);
	//Filter 2500-5000 Hz
	out1 = 0;
	out1 = twofiveToFivekHz(invalue);//FUNGERAR BRA
	//analogpin1 = adc_get_channel_value(ADC,ADC_CHANNEL_6);
	//out1 = modifyOutPut(out1,analogpin1);
	//Filter 5000-7500 Hz
	out2 = 0;
	out2 = fiveToSevenkHz(invalue);//FUNGERAR BRA
	//analogpin2 = adc_get_channel_value(ADC,ADC_CHANNEL_5);
	//out2 = modifyOutPut(out2,analogpin2);
	//Filter 7500-9999 Hz
	out3 = 0;
	out3 = sevenToNinekHz(invalue);//FUNGERAR BRA
	//analogpin3 = adc_get_channel_value(ADC,ADC_CHANNEL_4);
	//out3 = modifyOutPut(out3,analogpin3);

	//OUTPUT
	outvalue = (out0+out1+out2+out3)+2048;							//lägger till filter på outvalue som skickas till DAC. 2048 lägger till offset!! på ~1.4 V
	
	
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