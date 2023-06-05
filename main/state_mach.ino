#include "state_mach.h"

static bool car1_exiting = false;
static bool car1_car2_swap = false;
static bool car2_entering = false;
static bool car2_enter = false;

extern bool warning_flag;

void state_machine(void)
{

    static unsigned long start_car1_exiting = 0;
    static unsigned long start_car1_car2_swap = 0;
    static unsigned long start_car2_entering = 0;
    static unsigned long start_car2_enter = 0;

    volatile unsigned long time_car1_exiting = 0;
    volatile unsigned long time_car1_car2_swap = 0;
    volatile unsigned long time_car2_entering = 0;
    volatile unsigned long time_car2_enter = 0;

    if (!car1_exiting && is_car1_exiting())
    {
        warning_flag = false; // Reset the alarm if new car enter
        if (car2_enter)
        {
            car2_enter = false;
            time_car2_enter = millis() - start_car2_enter;
            Serial.println("Tempo estado W : " + String((float)time_car2_enter / 1000, 3) + " seg");
            if (time_car2_enter <= TEMPO_MAX_car2_enter * 1000)
            {
                // Activates the indicator of invasor
                warning_flag = true;
                Serial.print("****** INVASOR *********\n");
            }
        }
        rst_states(); // Reseting all flags
        car1_exiting = true;
        Serial.print("=====> C ativo <=========\n");
        start_car1_exiting = millis();
    }
    else if (!car1_car2_swap && car1_exiting && is_car1_car2_swap())
    {
        car1_exiting = false;
        time_car1_exiting = millis() - start_car1_exiting;
        Serial.println("Tempo estado C : " + String((float)time_car1_exiting / 1000, 3) + " seg");
        if (time_car1_exiting <= TEMPO_MAX_car1_exiting * 1000)
        {
            car1_car2_swap = true;
            Serial.print("=====> V ativo <========\n");
            start_car1_car2_swap = millis();
        }
    }
    else if (!car2_entering && car1_car2_swap && is_car2_entering())
    {
        car1_car2_swap = false;
        time_car1_car2_swap = millis() - start_car1_car2_swap;
        Serial.println("Tempo estado V : " + String((float)time_car1_car2_swap / 1000, 3) + " seg");
        if (time_car1_car2_swap <= TEMPO_MAX_car1_car2_swap * 1000)
        {
            car2_entering = true;
            Serial.print("=====> J ativo <=========\n");
            start_car2_entering = millis();
        }
    }
    else if (!car2_enter && car2_entering && is_car2_enter())
    {
        car2_entering = false;
        time_car2_entering = millis() - start_car2_entering;
        Serial.println("Tempo estado J : " + String((float)time_car2_entering / 1000, 3) + " seg");
        if (time_car2_entering <= TEMPO_MAX_car2_entering * 1000)
        {
            car2_enter = true;
            Serial.print("=====> W ativo <=========\n");
            start_car2_enter = millis();
        }
    }
    else if(is_idle())
    {
        rst_states();    //Reset states 
    }
}

void rst_states(void)
{
    car1_exiting = false;
    car1_car2_swap = false;
    car2_entering = false;
    car2_enter = false;
    //Serial.println("RESET");
}