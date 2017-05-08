/*
* Filter.c
*
* Denna fil filtrerar ljudet från ADC:n
* Author: Michael Nilsson och Martin Bråhagen
*/
#include <asf.h>
#include "Filter/Filter.h"

#define N 7
#define M 7

/****
* 1-3kHz Butterworth 6th order
*****/
static int32_t xbuff[M+1]={0};
static int32_t b[M+1] ={

180,                 0, -542,                 0,
542,                 0, -180

}; //koefficienterna * 10000
/****
* 1-3kHz Butterworth 6th order
*****/
static int32_t y[N+1]={0};
static int32_t a[N+1]={

-1,   40491,    -73339,    76049,
-47719,   17158,   -2780
	
}; //koefficienterna * 10000
/*-------------------------------------------------*/

/****
* 3-5 kHz Butterworth 8th order
****/
static int32_t x2buff[M+1]={0};
static int32_t b2[M+1] ={

	180,                 0, -542,                 0,
	542,                 0, -180

}; //koefficienterna * 10000
/****
* 3-5 kHz Butterworth 8th order
****/
static int32_t y2[N+1]={0};
static int32_t a2[N+1]={
	-1,   15466,    -25732,   20572,
	-17065,  6553,   -2780
}; //koefficienterna * 10000

/*-------------------------------------------------*/
/****
* 5-7 kHz Butterworth 8th order
****/
static int32_t x3buff[M+1]={0};
static int32_t b3[M+1] ={

	180,                 0, -542,                 0,
	542,                 0, -180

}; //koefficienterna * 10000
/****
* 5-7 kHz Butterworth 8th order
****/
static int32_t y3[N+1]={0};
static int32_t a3[N+1]={
	-1,    -15466,    -25732,    -20572,
	-17065,   -6553,   -2780
}; //koefficienterna * 10000
/*-------------------------------------------------*/


/****
* 7-9 kHz Butterworth 8th order
****/
static int32_t x4buff[M+1]={0}; // insignalen
static int32_t b4[M+1] ={
180,                 0, -542,                 0,
542,                 0, -180

}; //koefficienterna * 10000
/****
* 7-9 kHz Butterworth 8th order
****/
static int32_t y4[N+1]={0}; //fördröjda värdena av utsignalen
static int32_t a4[N+1]={
-1,    -40491,    -73339,     -76049,
-47719,    -17158,   -2780
}; //koefficienterna * 10000

//*******************************************************************************
/*
------------------------------------------
500-1500 Hz
B:
48,                 0, -192,                 0,
289,                 0, -192,                 0,
48
A:
-10000,   54182,    -135293,   203192,
-200711,   133443,     -58321,   15347,
-1873
-------------------------------
1000-1500 Hz
A:
-10000,   51407,    -131255,   209376,
-226982,   170341,    -86866,   27671,
-4382
B:
4,                 0,-16,                 0,
24,                0,-16,                 0,
41
-------------------------------
1500-2000 Hz
B:
4                 ,0,-16,                 0,
24,                 0,-16,                 0,
4
A:
-1,   33005,    -72800,    102057,
-109518,   83065,    -48222,   17766,
-4382
-------------------------------
2000-2500 Hz
B:
4,                 0,-16,                 0,
24,                 0,-16,                 0,
4
A:
-1,   11373,    -36673,   28272,
-46536,   23021,    -24339,  6121,
-4382
--------------------------------
2500-3000 Hz
B:
4,                 0,-16,                 0,
24,                 0,-16,                 0,
4
A:
-1,    -11373,    -36673,    -28272,
-46536,    -23021,    -24339,   -6121,
-4382
-------------------
500-1000 Hz 7 ordningen butterworth
B:
28,                 0,-86,                 0,
86,                 0,-28
A:
-1,   48480,    -102497,   120485,
-83006,   31804,   -5320
*/

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
	
	//Filter 1000-3000 Hz
	out0 = 0;
	out0 = oneToThreekHz(invalue);							// Filter
	//	analogpin0 = adc_get_channel_value(ADC,ADC_CHANNEL_7);	// Läser av analogpin0
	//	out0 = modifyOutPut(out0,analogpin0);					// Gör modifierning av filter värdet

	//Filter 3000-5000 Hz
	out1 = 0;
		out1 = threeToFivekHz(invalue);							// Filter
	//	analogpin1 = adc_get_channel_value(ADC,ADC_CHANNEL_6);	// Läser av analogpin1
	//	out1 = modifyOutPut(out1,analogpin1);					// Gör modifierning av filter värdet

	//5000-7000 Hz
	out2 = 0;
		out2 = fiveToSevenkHz(invalue);							// Filter
	//	analogpin2 = adc_get_channel_value(ADC,ADC_CHANNEL_5);	// Läser av analogpin2
	//	out2 = modifyOutPut(out2,analogpin2);					// Gör modifierning av filter värdet
	out3 = 0;
	//	out3 = fiveToSevenkHz(invalue);							// Filter
	//	analogpin3 = adc_get_channel_value(ADC,ADC_CHANNEL_4);	// Läser av analogpin2
	//	out3 = modifyOutPut(out2,analogpin2);					// Gör modifierning av filter värdet
	//OUTPUT
	outvalue = (out0+out1+out2);							// Lägger till alla filter
	
	
	dacc_write_conversion_data(DACC,outvalue);	// Skickar ut värdet på DAC
	
	ioport_set_pin_level(CHECK_PIN,LOW);		// Sätter pin22 låg
	
}
//1000-3000 Hz
uint32_t oneToThreekHz(uint32_t invalue){
	int32_t sum = 0;
	int32_t sumy = 0;
	float totsum = 0;
	uint32_t outvalue;
	
	int i;
	for(i =M; i>0; i--){
		xbuff[i]=xbuff[i-1];
	}
	xbuff[0]=(int)invalue;
	int j;
	for( j =0; j<=M;j++){
		sum += ((xbuff[j]*b[j]));
	}
	for( j =1; j<=N;j++){
		sumy += ((y[j]*a[j]));
	}
	for(i = N; i>1;i--){
		y[i]=y[i-1];
	}
	totsum = ((sum/10000)+(sumy/10000));
	y[1] = totsum;
	outvalue = (uint32_t)totsum;
	return outvalue;
}

//3000-5000 Hz
uint32_t threeToFivekHz(uint32_t invalue){

	int32_t sum = 0;
	int32_t sumy = 0;
	float totsum = 0;
	uint32_t outvalue;
	
	int i;
	for(i =M; i>0; i--){
		x2buff[i]=x2buff[i-1];
	}
	x2buff[0]=(int)invalue;
	int j;
	for( j =0; j<=M;j++){
		sum += ((x2buff[j]*b2[j]));
	}
	for( j =1; j<=N;j++){
		sumy += ((y2[j]*a2[j]));
	}
	for(i = N; i>1;i--){
		y2[i]=y2[i-1];
	}
	totsum = ((sum/10000)+(sumy/10000));
	y2[1] = totsum;
	outvalue = (uint32_t)totsum;
	return outvalue;
}
//5000-7000 Hz
uint32_t fiveToSevenkHz(uint32_t invalue){
	int32_t sum = 0;
	int32_t sumy = 0;
	float totsum = 0;
	uint32_t outvalue;
	
	int i;
	for(i =M; i>0; i--){
		x3buff[i]=x3buff[i-1];
	}
	x3buff[0]=(int)invalue;
	int j;
	for( j =0; j<=M;j++){
		sum += ((x3buff[j]*b3[j]));
	}
	for( j =1; j<=N;j++){
		sumy += ((y3[j]*a3[j]));
	}
	for(i = N; i>1;i--){
		y3[i]=y3[i-1];
	}
	totsum = ((sum/10000)+(sumy/10000));
	y3[1] = totsum;
	outvalue = (uint32_t)totsum;
	return outvalue;
}

//Modifierar filtervärdet man har fått ut av filtret baserat på potentiometer
uint32_t modifyOutPut(uint32_t filtervalue,uint32_t analogValue){

	if(analogValue <= 819){
		return 0;
		}else if((analogValue >819)&&(analogValue<=1638)){
		return (filtervalue*(0.25));
		}else if((analogValue>1638)&&(analogValue<=2457)){
		return (filtervalue*(0.5));
		}else if((analogValue>2457)&&(analogValue<3276)){
		return (filtervalue*(0.75));
		}else if((analogValue>3276)&&(analogValue<4095)){
		return filtervalue;
	}
}