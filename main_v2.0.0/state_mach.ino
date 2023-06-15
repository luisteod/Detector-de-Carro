#include "state_mach.h"

static bool car1_exiting = false;
static bool car1_car2_swap = false;
static bool car2_entering = false;
static bool car2_pay = false;

extern bool warning_flag;

void state_machine(void)
{

    static unsigned long start_car1_exiting = 0;
    static unsigned long start_car1_car2_swap = 0;
    static unsigned long start_car2_entering = 0;
    static unsigned long start_car2_pay = 0;

    volatile unsigned long time_car1_exiting = 0;
    volatile unsigned long time_car1_car2_swap = 0;
    volatile unsigned long time_car2_entering = 0;
    volatile unsigned long time_car2_pay = 0;

    // Bengin of states chaining

    if (!car1_exiting && is_car1_exiting())
    {
        warning_flag = false; // Reset the alarm if new car enter
        if (car2_pay)
        {
            car2_pay = false;
            time_car2_pay = millis() - start_car2_pay;
            // Time between prev state in seconds
            Serial.println(String((float)time_car2_pay / 1000, 3));
            if (time_car2_pay <= TEMPO_MAX_CAR2_PAY * 1000)
            {
                // Activates the indicator of invasor
                warning_flag = true;
                Serial.println("INVASOR");
            }
        }
        rst_states(); // Reseting all flags
        car1_exiting = true;
        Serial.print("CAR_1_EXIT ");
        start_car1_exiting = millis();
    }
    else if (!car1_car2_swap && car1_exiting && is_car1_car2_swap())
    {
        car1_exiting = false;
        time_car1_exiting = millis() - start_car1_exiting;
        // Time between prev state in seconds
        Serial.println(String((float)time_car1_exiting / 1000, 3));
        if (time_car1_exiting <= TEMPO_MAX_CAR1_EXITING * 1000)
        {
            car1_car2_swap = true;
            Serial.print("SWAP ");
            start_car1_car2_swap = millis();
        }
    }
    else if (!car2_entering && car1_car2_swap && is_car2_entering())
    {
        car1_car2_swap = false;
        time_car1_car2_swap = millis() - start_car1_car2_swap;
        // Time between prev state in second
        Serial.println(String((float)time_car1_car2_swap / 1000, 3));
        if (time_car1_car2_swap <= TEMPO_MAX_CAR1_CAR2_SWAP * 1000)
        {
            car2_entering = true;
            Serial.print("CAR_2_ENTER ");
            start_car2_entering = millis();
        }
    }
    else if (!car2_pay && car2_entering && is_car2_pay())
    {
        car2_entering = false;
        time_car2_entering = millis() - start_car2_entering;
        // Time between prev state in seconds
        Serial.println(String((float)time_car2_entering / 1000, 3));
        if (time_car2_entering <= TEMPO_MAX_CAR2_ENTERING * 1000)
        {
            car2_pay = true;
            Serial.print("CAR_2_PAY ");
            start_car2_pay = millis();
        }
    }
    else if (is_idle())
    {
        rst_states(); // Reset states
    }
    // Catch current state for print
    // catch_current_state();
}

void rst_states(void)
{
    car1_exiting = false;
    car1_car2_swap = false;
    car2_entering = false;
    car2_pay = false;
    // Serial.println("RESET");
}

// void catch_current_state(void)
// {

//     static String current_state;
//     if(car1_exiting){
//         current_state = "CAR 1 EXIT";
//     }
//     else if (car1_car2_swap){
//         current_state = "CAR SWAP";
//     }
//     else if (car2_entering)
//         current_state = "CAR 2 ENTER";
//     else if (car2_pay)
//         current_state = "CAR 2 PAY";

// }
