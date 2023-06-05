#define WARNING_LED 8
#define BUFFER_SIZE 100

#ifndef __AVR_ATmega328P__
#include <avr/iom328p.h>
#endif


void timer_initialize(void);
void onTimer(void);
void sampling(void);