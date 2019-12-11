#ifndef __CH2O_H
#define __CH2O_H
#include "main.h"

uint16_t Ch2oProtocolAnalysis(uint8_t *buff,uint8_t len);

char *F2S(float d,char *str);

uint16_t Ch2oSensor(void);
	
extern uint16_t ch2o;
#endif

