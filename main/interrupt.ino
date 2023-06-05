#include "interrupt.h"

ISR(TIMER1_COMPA_vect)
{
    onTimer();
}

ISR(TIMER2_COMPA_vect)
{
    sampling();
}