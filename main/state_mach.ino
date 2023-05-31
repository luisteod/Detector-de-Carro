#include "state_mach.h"

static bool state_c = false;
static bool state_v = false;
static bool state_j = false;
static bool state_w = false;

extern bool warning_flag;

void state_machine(void)
{

    static unsigned long start_state_c = 0;
    static unsigned long start_state_v = 0;
    static unsigned long start_state_j = 0;
    static unsigned long start_state_w = 0;

    volatile unsigned long time_state_c = 0;
    volatile unsigned long time_state_v = 0;
    volatile unsigned time_state_j = 0;
    volatile unsigned time_state_w = 0;

    if (!state_c && is_state_c())
    {
        warning_flag = false; // Reset the alarm if new car enter
        if (state_w)
        {
            state_w = false;
            time_state_w = millis() - start_state_w;
            Serial.println("Tempo estado W : " + String((float)time_state_w / 1000, 3) + " seg");
            if (time_state_w <= TEMPO_MAX_STATE_W * 1000)
            {
                // Activates the indicator of invasor
                warning_flag = true;
                Serial.print("****** INVASOR *********\n");
            }
        }
        rst_states(); // Reseting all flags
        state_c = true;
        Serial.print("=====> C ativo <=========\n");
        start_state_c = millis();
    }
    else if (!state_v && state_c && is_state_v())
    {
        state_c = false;
        time_state_c = millis() - start_state_c;
        Serial.println("Tempo estado C : " + String((float)time_state_c / 1000, 3) + " seg");
        if (time_state_c <= TEMPO_MAX_STATE_C * 1000)
        {
            state_v = true;
            Serial.print("=====> V ativo <========\n");
            start_state_v = millis();
        }
    }
    else if (!state_j && state_v && is_state_j())
    {
        state_v = false;
        time_state_v = millis() - start_state_v;
        Serial.println("Tempo estado V : " + String((float)time_state_v / 1000, 3) + " seg");
        if (time_state_v <= TEMPO_MAX_STATE_V * 1000)
        {
            state_j = true;
            Serial.print("=====> J ativo <=========\n");
            start_state_j = millis();
        }
    }
    else if (!state_w && state_j && is_state_w())
    {
        state_j = false;
        time_state_j = millis() - start_state_j;
        Serial.println("Tempo estado J : " + String((float)time_state_j / 1000, 3) + " seg");
        if (time_state_j <= TEMPO_MAX_STATE_J * 1000)
        {
            state_w = true;
            Serial.print("=====> W ativo <=========\n");
            start_state_w = millis();
        }
    }
    else if(is_idle())
    {
        rst_states();    //Reset states 
    }
}

void rst_states(void)
{
    state_c = false;
    state_v = false;
    state_j = false;
    state_w = false;
    Serial.println("RESET");
}