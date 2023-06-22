#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.h"
#define WARNING_LED 8

#ifndef __AVR_ATmega328P__
#include <avr/iom328p.h>
#endif


void timer_initialize(void);
void onTimer(void);
void sampling(void);