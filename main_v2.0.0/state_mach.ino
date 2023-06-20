#define TEMPO_MAX_CAR1_EXITING 10
#define TEMPO_MAX_CAR1_CAR2_SWAP 10
#define TEMPO_MAX_CAR2_ENTERING 10
#define TEMPO_MAX_CAR2_PAY 5
#define START_IDLE_TIME 10
#define TIME_COND_CAR_SWAP 2
#include "state_mach.h"

static bool car1_exiting = false;
static bool car1_car2_swap = false;
static bool car2_entering = false;
static bool car2_pay = false;
static bool idle = false;

extern bool warning_flag;

void state_machine(void)
{

    static unsigned long start_car1_exiting = 0;
    static unsigned long start_car1_car2_swap = 0;
    static unsigned long start_car2_entering = 0;
    static unsigned long start_car2_pay = 0;
    static unsigned long start_idle = 0;

    volatile unsigned long time_car1_exiting = 0;
    volatile unsigned long time_car1_car2_swap = 0;
    volatile unsigned long time_car2_entering = 0;
    volatile unsigned long time_car2_pay = 0;
    volatile unsigned long time_idle = 0;

    // Bengin of states chaining
    // Caso esteja vindo de idle, será feita a verificacao do
    // tempo decorrido para avaliar se irá para o prox estado.
    if (idle && is_car2_entering())
    {
        idle = false;
        time_idle = (float)(millis() - start_idle) / 1000.000;
        Serial.println(String((float)time_idle, 1));
        if (time_idle <= TIME_COND_CAR_SWAP)
            goto car_swap;
    }
    if (!car1_exiting && is_car1_exiting())
    {
        // Reset the alarm if new car enter
        warning_flag = false;
        if (car2_pay)
        {
            car2_pay = false;
            time_car2_pay = (float)(millis() - start_car2_pay) / 1000.000;
            // Time between prev state in seconds
            Serial.println(String((float)time_car2_pay, 1));
            if (time_car2_pay <= TEMPO_MAX_CAR2_PAY)
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
        time_car1_exiting = (float)(millis() - start_car1_exiting) / 1000.000;
        // Time between prev state in seconds
        Serial.println(String((float)time_car1_exiting, 1));
        if (time_car1_exiting <= TEMPO_MAX_CAR1_EXITING)
        {
            car1_car2_swap = true;
            Serial.print("SWAP ");
            start_car1_car2_swap = millis();
        }
    }
    else if (!car2_entering && car1_car2_swap && is_car2_entering())
    {
    car_swap:
        car1_car2_swap = false;
        time_car1_car2_swap = (float)(millis() - start_car1_car2_swap) / 1000.000;
        // Time between prev state in second
        Serial.println(String((float)time_car1_car2_swap, 1));
        if (time_car1_car2_swap <= TEMPO_MAX_CAR1_CAR2_SWAP)
        {
            car2_entering = true;
            Serial.print("CAR_2_ENTER ");
            start_car2_entering = millis();
        }
    }
    else if (!car2_pay && car2_entering && is_car2_pay())
    {
        car2_entering = false;
        time_car2_entering = (float)(millis() - start_car2_entering) / 1000.000;
        // Time between prev state in seconds
        Serial.println(String((float)time_car2_entering, 1));
        if (time_car2_entering <= TEMPO_MAX_CAR2_ENTERING)
        {
            car2_pay = true;
            Serial.print("CAR_2_PAY ");
            start_car2_pay = millis();
        }
    }
    else if (is_idle())
    {
        idle = true;
        // Restarta a contagem com o tempo de início relativo
        // a última ativação do estado car1_entering.
        if (car1_exiting)
        {
            Serial.print("IDLE");
            start_idle = millis();
        }
        rst_states();
    }
}

void rst_states(void)
{
    car1_exiting = false;
    car1_car2_swap = false;
    car2_entering = false;
    car2_pay = false;
    // Serial.println("RESET");
}

inline bool is_car1_exiting(void)
{
    return (!is_s1_active() && is_s2_active());
}
inline bool is_car1_car2_swap(void)
{
    return (is_s1_s2_active());
}
inline bool is_car2_entering(void)
{
    return (is_s1_active() && !is_s2_active());
}
inline bool is_car2_pay(void)
{
    return (is_s1_s2_active());
}
inline bool is_idle(void)
{
    return (!is_s1_active() && !is_s2_active());
}