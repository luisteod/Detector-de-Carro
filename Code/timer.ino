#include "timer.h"

unsigned long elapsedTime, previousTime;

extern bool warning_flag;

/*Initializes the timer1 with a prescaler
 resulting an interruption each 1 second*/
void timer_initialize(void)
{
    cli(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 31250;          // compare match register 16MHz/256/2 = 31250
    TCCR1B = (1 << WGM12);  // CTC mode
    TCCR1B = (1 << CS12);   // Frequency 16Mhz/ 256 = 62500
    TIMSK1 = (1 << OCIE1A); // Local interruption OCIE1A
    sei();                  // enable all interrupts
}

void onTimer(void)
{
    warning_flag = true;
    if (warning_flag)
    {
        static boolean state = HIGH;
        elapsedTime = millis() - previousTime;
        Serial.print(F("Set LED 13 : "));
        if (state)
        {
            Serial.print(F("ON"));
        }
        else
        {
            Serial.print(F("OFF"));
        }
        Serial.print(F(" - "));
        Serial.print(elapsedTime);
        Serial.println(F("ms"));
        digitalWrite(WARNING_LED, state);
        state = !state;

        previousTime = millis();
    }
}