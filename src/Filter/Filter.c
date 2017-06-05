/*
* Filter.c
*
* Denna fil filtrerar ljudet från ADC:n
* Author: Michael Nilsson och Martin Bråhagen
*/
#include <asf.h>
#include "Filter/Filter.h"
#include "Filter/500To1kHz.h"
#include "Filter/1000To2kHz.h"
#include "Filter/2000To4kHz.h"
#include "Filter/4000To8kHz.h"
#include "Filter/movingFilter/movingFilter1.h"
#include "Filter/movingFilter/movingFilter2.h"
#include "Filter/movingFilter/movingFilter3.h"
#include "Filter/movingFilter/movingFilterbandstop1.h"
#include "Filter/movingFilter/movingFilterbandstop2.h"
#include "Filter/movingFilter/movingFilterbandstop3.h"

extern sampelfrekvens;
extern numberofband;
/**
*  Hanterar interruptet som då utför filtret
*/
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;
	uint32_t invalue, outvalue;
	uint32_t out0,out1,out2,out3,outmovingfilter,outmovingfilterbandstop;
	uint32_t analogpin0,analogpin1,analogpin2,analogpin3;
	uint32_t temp1, temp2;

	
	ul_dummy = tc_get_status(TC0, 0);

	
	UNUSED(ul_dummy);
	
	ioport_set_pin_level(CHECK_PIN,HIGH);		// Sätter pin 22 hög
	adc_start(ADC);								// Startar ADC
	while((adc_get_status(ADC) & 0x1<<24)==0);  // väntar till DRDY blir hög

	invalue=adc_get_channel_value(ADC,ADC_CHANNEL_10);			// Läser av värdet på analogpin8 som är själva ljud signalen

	if((ioport_get_pin_level(pin20)==1) && (ioport_get_pin_level(pin21)==1)){
			sampelfrekvens = 2100;
			tc_write_rc(TC0, 0, sampelfrekvens);
			//Filter 500-1000 Hz
			out0 = 0;
			out0 = fiveHToOneKhz(invalue);
			analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_6);
			out0 = modifyOutPut(out0,analogpin0);
			//Filter 1000-2000 Hz
			out1 = 0;
			out1 = oneKTo2KHz(invalue);
			analogpin1 = adc_get_channel_value(ADC,ADC_CHANNEL_5);
			out1 = modifyOutPut(out1,analogpin1);
			//Filter 2000-4000 Hz
			out2 = 0;
			out2 = twokToFourKHz(invalue);
			analogpin2 = adc_get_channel_value(ADC,ADC_CHANNEL_4);
			out2 = modifyOutPut(out2,analogpin2);
			//Filter 4000-8000 Hz
			out3 = 0;
			out3 = fourKTo8Khz(invalue);
			analogpin3 = adc_get_channel_value(ADC,ADC_CHANNEL_3);
			out3 = modifyOutPut(out3,analogpin3);
			//OUTPUT
			outvalue = (out0+out1+out2+out3)+2048;							// Lägger till filter på outvalue som skickas till DAC. 2048 lägger till offset!! på ~1.4 V
		}else if((ioport_get_pin_level(pin20)== 1 ) && (ioport_get_pin_level(pin21)==0)){ // Moving bandPASSfilter
			analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_6);			// Läser av potentiometer
			outmovingfilter = MovingFilterfrekvens(analogpin0,invalue);		// Utför beräkningarna till filtret. Sätter sampelfrekvens!
			tc_write_rc(TC0, 0, sampelfrekvens);							// Sampelfrekvens sätts			
			outvalue = (outmovingfilter)+2048;								// Sätter utvärdet
		
		}else if((ioport_get_pin_level(pin20)==0) && (ioport_get_pin_level(pin21)==1)){// Moving bandSPÄRRfilter
			analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_6);						// Läser av potentiometer
			outmovingfilterbandstop = MovingFilterfrekvensBandStop(analogpin0,invalue);	// Utför beräkningarna till filtret. Sätter sampelfrekvens!
			tc_write_rc(TC0, 0, sampelfrekvens);										// Sampelfrekvens sätts		
			outvalue = (outmovingfilterbandstop)+2048;									// Sätter utvärdet
		}else if((ioport_get_pin_level(pin20)== 0) && (ioport_get_pin_level(pin21)==0)) {
			outvalue = 0;
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
/************************************************************************/		
/* Metoden är till för moving bandSPÄRRfilter							*/
/* Läser av potentiometern, Därefter sätts sampelfrekvensen och			*/
/*  Ett anrop till filtermetoden görs som då returnerar värdet från		*/
/*  beräkningen från filtret                                            */
/************************************************************************/
uint32_t MovingFilterfrekvens(uint32_t analogmovingfilter,uint32_t invalue){
	if(analogmovingfilter<=683){
		sampelfrekvens = 2100;
		numberofband = 49;
		return movingFilter1(invalue);
	}else if(analogmovingfilter>683 && analogmovingfilter<=1365){
		sampelfrekvens = 1050;
		numberofband = 49;
		return movingFilter1(invalue);
	}else if(analogmovingfilter>1365 && analogmovingfilter <=2048){
		sampelfrekvens = 2100;
		numberofband = 50;
		return movingFilter2(invalue);
	}else if(analogmovingfilter>2048 && analogmovingfilter <= 2731){
		sampelfrekvens = 1050;
		numberofband = 50;
		return movingFilter2(invalue);
	}else if(analogmovingfilter>2731 && analogmovingfilter<=3414){
		 sampelfrekvens = 2100;
		 numberofband = 51;
		return movingFilter3(invalue);
	}else if(analogmovingfilter>3414 && analogmovingfilter<=4095){
		sampelfrekvens = 1050;
		numberofband = 51;
		return movingFilter3(invalue);
	}
}
/************************************************************************/
/* Metoden är till för moving bandSPÄRRfilter							*/
/* Läser av potentiometern, Därefter sätts sampelfrekvensen och			*/
/*  Ett anrop till filtermetoden görs som då returnerar värdet från		*/
/*  beräkningen från filtret                                            */
/************************************************************************/
uint32_t MovingFilterfrekvensBandStop(uint32_t analogmovingfilter,uint32_t invalue){
	if(analogmovingfilter<=683){
		sampelfrekvens = 2100;
		numberofband = 49;
		return movingFilterBandStop1(invalue);
		}else if(analogmovingfilter>683 && analogmovingfilter<=1365){
		sampelfrekvens = 1050;
		numberofband = 49;
		return movingFilterBandStop1(invalue);
		}else if(analogmovingfilter>1365 && analogmovingfilter <=2048){
		sampelfrekvens = 2100;
		numberofband = 50;
		return movingFilterBandStop2(invalue);
		}else if(analogmovingfilter>2048 && analogmovingfilter <= 2731){
		sampelfrekvens = 1050;
		numberofband = 50;
		return movingFilterBandStop2(invalue);
		}else if(analogmovingfilter>2731 && analogmovingfilter<=3414){
		sampelfrekvens = 2100;
		numberofband = 51;
		return movingFilterBandStop3(invalue);
		}else if(analogmovingfilter>3414 && analogmovingfilter<=4095){
		sampelfrekvens = 1050;
		numberofband = 51;
		return movingFilterBandStop3(invalue);
	}
}