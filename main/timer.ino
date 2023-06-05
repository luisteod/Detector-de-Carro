#include "timer.h"
#include "sensors.h"

unsigned long elapsedTime, previousTime;

extern bool warning_flag;
extern float dist_t1;
extern float dist_t2;
extern float dist_t1_buff[BUFFER_SIZE];
extern float dist_t2_buff[BUFFER_SIZE];

/*Initializes the timer1 with a prescaler
 resulting an interruption each 1 second*/
void timer_initialize(void)
{
    cli(); // disable all interrupts

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    // 90.3179190751445 Hz (16000000/((172+1)*1024))
    OCR2A = 172;
    // CTC
    TCCR2A |= (1 << WGM21);
    // Prescaler 1024
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    // Output Compare Match A Interrupt Enable
    TIMSK2 |= (1 << OCIE2A);

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 31250;                       // compare match register 16MHz/256/2 = 31250
    TCCR1B = (1 << WGM12);               // CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10); // Frequency 16Mhz/64 = 250 kHz
    TIMSK1 = (1 << OCIE1A);              // Local interruption OCIE1A

    sei(); // enable all interrupts
}

void onTimer(void)
{
    // warning_flag = true;
    if (warning_flag)
    {
        static boolean state = HIGH;
        elapsedTime = millis() - previousTime;
        // Serial.print(F("Set LED 13 : "));
        if (state)
        {
            // Serial.print(F("ON"));
        }
        else
        {
            // Serial.print(F("OFF"));
        }
        // Serial.print(F(" - "));
        // Serial.print(elapsedTime);
        // Serial.println(F("ms"));
        digitalWrite(WARNING_LED, state);
        state = !state;

        previousTime = millis();
    }
    else
    {
        digitalWrite(WARNING_LED, LOW);
    }
}

void sampling(void)
{
    static int counter = 0;
    float sample = 0;

    sample = get_dist_s1() / BUFFER_SIZE;
    dist_t1 = dist_t1 + sample - dist_t1_buff[counter];
    dist_t1_buff[counter] = sample;

    sample = get_dist_s2() / BUFFER_SIZE;
    dist_t2 = dist_t2 + sample - dist_t2_buff[counter];
    dist_t2_buff[counter] = sample;
    counter++;
    if (counter == BUFFER_SIZE)
        counter = 0;
}