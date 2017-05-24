/*
 * lcdApplication.c
 *
 * Created: 2015-09-10 08:44:50
 *  Author: Patricia J�nsson och Martin Br�hagen
 */ 
#include "LCDFunctions.h"
#include "lcdApplication.h"


int lcdWrite4DigitNumber(int number)
/*  
 * To write on the display we need to send numbers between 0-9.
 * Then we add 48 to get the right ASCII-value.
 */
{
	int zero,first,second,third,fourth, temp_1, temp_2,temp_3;

	temp_3 =number % 10000;
	zero = ((number - temp_3) / 10000) + 48;/* To get the 10000th number*/

	temp_1 = number % 1000;						
	first = ((temp_3 - temp_1) / 1000) + 48;	/* To get the 1000th number*/
		
	temp_2 = temp_1 % 100;
	second = ((temp_1 - temp_2) / 100) + 48;	/* To get the 100th number*/
	
	temp_1 = temp_2 % 10;
	third = ((temp_2 - temp_1) / 10) + 48;		/* To get the 10th number*/
	
	fourth = temp_1 + 48;						
	lcdWrite(zero, HIGH);
	lcdWrite(first, HIGH);	
	lcdWrite(second, HIGH);
	lcdWrite(third, HIGH);
	lcdWrite(fourth, HIGH);
		
	return 0;	
}

int lcdWriteAsciiString(const char *string)
{
	while(*string !='\0') {			/* Writes out one char at a time*/
		lcdWrite(*string,HIGH);
		string++;
	}
	return 0;	
}
