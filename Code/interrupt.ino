#include "interrupt.h"

ISR(TIMER1_COMPA_vect)
{
    onTimer();
}