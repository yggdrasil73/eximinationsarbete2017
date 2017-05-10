/*
* Author: Michael Nilsson och Martin Bråhagen
*/
#ifndef SAMPEL_INT_H_
#define SAMPEL_INT_H_

#define CHECK_PIN PIO_PB26_IDX	//Pin22 på Arduino Due

void TCO_Handler(void);
uint32_t modifyOutPut(uint32_t filtervalue,uint32_t analogValue);
#endif /* SAMPEL_INT_H_ */