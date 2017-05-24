/*
* Author: Michael Nilsson och Martin Bråhagen
*/
#ifndef SAMPEL_INT_H_
#define SAMPEL_INT_H_

#define CHECK_PIN PIO_PB26_IDX	//Pin22 på Arduino Due
#define pin20 PIO_PB12_IDX //Pin24 på Arduino Due
#define pin21 PIO_PB13_IDX	//Pin26 på arduino due
void TCO_Handler(void);
uint32_t modifyOutPut(uint32_t filtervalue,uint32_t analogValue);
uint32_t MovingFilterfrekvens(uint32_t analogmovingfilter,uint32_t invalue);
uint32_t MovingFilterfrekvensBandStop(uint32_t analogmovingfilter,uint32_t invalue);
#endif /* SAMPEL_INT_H_ */