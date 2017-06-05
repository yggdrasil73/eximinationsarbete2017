
/*
* LCDFunctions.c
*
* Author:  Michael Nilsson och Martin Br�hagen
*/

#include <inttypes.h>
#include <asf.h>
#include "lcdFunctions.h"
#include "Delay/DelayFunctions.h"	

/************************************************************************/
/* Initialiserar sk�rmen p� LCD-sk�lden. returnerar 1 om allt �r ok   */
/************************************************************************/
int lcdInit(void)		
{
	int all_ok=1;		/* I b�rjan antar man att inget fungerar */
	
	/* At power on */
	ioport_set_pin_dir(LCD_RS, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_Enable, IOPORT_DIR_OUTPUT);
	ioport_set_port_dir(IOPORT_PIOC, LCD_mask_D4_D7, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LCD_Enable, LOW);
	delayMicroseconds(30000);		/* v�ntar > 15 ms */
	ioport_set_pin_level(LCD_RS, LOW);
	
	/* Funktioner s�tts (interface �r 8 bit l�ng) */
	ioport_set_pin_level(LCD_D4, HIGH);
	ioport_set_pin_level(LCD_D5, HIGH);
	ioport_set_pin_level(LCD_D6, LOW);
	ioport_set_pin_level(LCD_D7, LOW);
	ioport_set_pin_level(LCD_Enable, HIGH);
	delayMicroseconds(1);		/* v�ntar 1 us */
	ioport_set_pin_level(LCD_Enable, LOW);
	delayMicroseconds(4100);	/* v�ntar mer �n 4,1 ms */
	ioport_set_pin_level(LCD_D4, HIGH);
	ioport_set_pin_level(LCD_D5, HIGH);
	ioport_set_pin_level(LCD_D6, LOW);
	ioport_set_pin_level(LCD_D7, LOW);
	ioport_set_pin_level(LCD_Enable, HIGH);
	delayMicroseconds(1);	/* v�ntar 1 us */
	ioport_set_pin_level(LCD_Enable, LOW);
	delayMicroseconds(100);	/* v�ntar 100 us */
	ioport_set_pin_level(LCD_D4, HIGH);
	ioport_set_pin_level(LCD_D5, HIGH);
	ioport_set_pin_level(LCD_D6, LOW);
	ioport_set_pin_level(LCD_D7, LOW);
	ioport_set_pin_level(LCD_Enable, HIGH);
	delayMicroseconds(1);	/* v�ntar 1 us */
	ioport_set_pin_level(LCD_Enable, LOW);		
	delayMicroseconds(100);	/* v�ntar 100 us */
		
	/* S�tter sk�rmen till 8-bit input */
	ioport_set_pin_level(LCD_D4, LOW);
	ioport_set_pin_level(LCD_D5, HIGH);
	ioport_set_pin_level(LCD_D6, LOW);
	ioport_set_pin_level(LCD_D7, LOW);
	ioport_set_pin_level(LCD_Enable, HIGH);
	delayMicroseconds(1);
	ioport_set_pin_level(LCD_Enable, LOW);
	delayMicroseconds(100);
	lcdWrite(0b00101000, LOW);		/* Andra raden, liten font */
	lcdWrite(0b00001000, LOW);		/* Sk�rm av*/
	lcdWrite(0b00000001, LOW);		/* Rensar sk�rm */
	delayMicroseconds(3000);
	lcdWrite(0b00000110, LOW);		/* Entry mode set: Flytar cursorn h�ger, ingen display shift*/
	lcdWrite(0b00001111 ,LOW);		/* Sk�rm p�, cursor p� och blinkningp p�  */

	all_ok = 0;					
	return all_ok;
}


int lcdWrite(uint8_t byte, bool type)		/* writes a byte to the LCD display */
/*
 *	writes the byte (8 bits) to the LCD display as two consecutive 4 bits 
 *	type = 0 controls the display
 *	type = 1 writes the content of the byte (usually interpreted as ASCII-code) to the display
 */
{
	/* In it's infinite wisdom the shield has the data pins in mirrored order compared to the Due board */
	uint8_t mirror_pin[16]={0b0000,0b1000,0b0100,0b1100,0b0010,0b1010,0b0110,0b1110,0b0001,0b1001,0b0101,0b1101,0b0011,0b1011,0b0111,0b1111};
	uint32_t byte32;
	uint8_t byte_orig;
	uint8_t mirrored_output = 0;

	byte_orig = byte;

	/* write the first 4 bits to the shield. */
	byte = byte>>4;
	byte = mirror_pin[byte];
	byte32 = (uint32_t)byte << 23;
	mirrored_output = byte<<4;
	
	ioport_set_pin_level(LCD_RS, type); 
	ioport_set_port_level(IOPORT_PIOC, LCD_mask_D4_D7, byte32);
	ioport_set_pin_level(LCD_Enable, HIGH);
	delayMicroseconds(1);
	ioport_set_pin_level(LCD_Enable, LOW);
	
	delayMicroseconds(100);
	
	/* write the second 4 bits to the shield. */
	byte = byte_orig;
	byte = byte & 0x0f;
	byte = mirror_pin[byte];
	byte32 = (uint32_t)byte<<23;
	mirrored_output = mirrored_output + byte;

	ioport_set_port_level(IOPORT_PIOC, LCD_mask_D4_D7, byte32);
	ioport_set_pin_level(LCD_Enable, HIGH);
	delayMicroseconds(1);
	ioport_set_pin_level(LCD_Enable, LOW);
	delayMicroseconds(100);
	
	return 0;
}

int lcdClearDisplay(void)
{
	lcdWrite(0b00000001, LOW);		/* Display clear */
	delayMicroseconds(3000);		
	return 0;
}
