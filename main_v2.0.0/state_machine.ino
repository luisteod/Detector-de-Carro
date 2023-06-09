#define TEMPO_MAX_car1_exit 3
#define TEMPO_MAX_CAR1_CAR2_SWAP 3
#define TEMPO_MAX_car2_enter 3
#define TEMPO_MAX_CAR2_PAY 5
#define TIME_COND_CAR_SWAP 2
#define TIMEOUT 6
#include "state_machine.h"

static bool car1_exit = false;
static bool car1_car2_swap = false;
static bool car2_enter = false;
static bool car2_pay = false;
static bool idle = false;

extern bool warning_flag;

static unsigned long start_car1_exit = 0;
static unsigned long start_car1_car2_swap = 0;
static unsigned long start_car2_enter = 0;
static unsigned long start_car2_pay = 0;
static unsigned long start_idle = 0;

static volatile unsigned long time_car1_exit = 0;
static volatile unsigned long time_car1_car2_swap = 0;
static volatile unsigned long time_car2_enter = 0;
static volatile unsigned long time_car2_pay = 0;
static volatile unsigned long time_idle = 0;

char debug_word[20];

void state_machineine(void)
{

    if (!car1_exit && is_car1_exit())
    {
        // Reset the alarm if new car enter
        warning_flag = false;
        if (car2_pay)
        {
            car2_pay = false;
            //Serial.print("time_car2_pay ");
            time_car2_pay = (float)(millis() - start_car2_pay) / 1000.000;
            csv_print(String((float)time_car2_pay, 1));
            if (time_car2_pay <= TEMPO_MAX_CAR2_PAY)
            {
                // Activates the indicator of invasor
                warning_flag = true;
                Serial.println("INVASOR");
            }
        }
        rst_states();
        car1_exit = true;
        csv_print(String("CAR_1_EXIT "));
        start_car1_exit = millis();
    }
    else if ((!car1_car2_swap && car1_exit && is_car1_car2_swap()))
    {
        car1_exit = false;
        //Serial.print("time_car1_exit ");
        time_car1_exit = (float)(millis() - start_car1_exit) / 1000.000;
        csv_print(String((float)time_car1_exit, 1));
        if (time_car1_exit <= TEMPO_MAX_car1_exit)
        {
            car1_car2_swap = true;
            csv_print(String("SWAP "));
            start_car1_car2_swap = millis();
        }
    }
    else if (!idle && car1_exit && is_idle())
    {
        car1_exit = false;
        // Restarta a contagem com o tempo de início relativo
        // a última ativação do estado car1_entering.
        //Serial.print("time_car1_exit ");
        time_car1_exit = (float)(millis() - start_car1_exit) / 1000.000;
        csv_print(String((float)time_car1_exit, 1));

        csv_print(String("IDLE "));
        start_idle = millis();
        idle = true;
    }
    else if (!car2_enter && (car1_car2_swap || idle) && is_car2_enter())
    {
        if (car1_car2_swap)
        {
            car1_car2_swap = false;
            //Serial.print("time_swap ");
            time_car1_car2_swap = (float)(millis() - start_car1_car2_swap) / 1000.000;
            csv_print(String((float)time_car1_car2_swap, 1));
        }
        else if (idle)
        {
            idle = false;
            //Serial.print("time_idle ");
            time_idle = (float)(millis() - start_idle) / 1000.000;
            csv_print(String((float)time_idle, 1));
        }

        if (time_car1_car2_swap <= TEMPO_MAX_CAR1_CAR2_SWAP || time_idle <= TIME_COND_CAR_SWAP)
        {
            car2_enter = true;
            csv_print(String("CAR_2_ENTER "));
            start_car2_enter = millis();
        }
    }
    else if (!car2_pay && car2_enter && is_car2_pay())
    {
        car2_enter = false;
        //Serial.print("time_car2_enter ");
        time_car2_enter = (float)(millis() - start_car2_enter) / 1000.000;
        csv_print(String((float)time_car2_enter, 1));
        if (time_car2_enter <= TEMPO_MAX_car2_enter)
        {
            car2_pay = true;
            csv_print(String("CAR_2_PAY "));
            start_car2_pay = millis();
        }
    }

    // else if (!idle && is_idle())
    // {
    //     idle = true;
    //     // Restarta a contagem com o tempo de início relativo
    //     // a última ativação do estado car1_entering.
    //     if (car1_exit)
    //     {
    //         csv_print(String("IDLE "));
    //         start_idle = millis();
    //     }
    // }
    // Bengin of states chaining
    // Caso esteja vindo de idle, será feita a verificacao do
    // tempo decorrido para avaliar se irá para o prox estado.

    // else if (idle && is_car2_enter())
    // {
    //     idle = false;
    //     time_idle = (float)(millis() - start_idle) / 1000.000;
    //     csv_print(String((float)time_idle, 1));
    //     if (time_idle <= TIME_COND_CAR_SWAP)
    //         goto car2_enter;
    // }

    time_out_verifier();
}

void debug(char *word)
{
    if (car1_exit)
        strcpy(word, "car1_exit");
    if (car1_car2_swap)
        strcpy(word, "car1_car2_swap");
    if (car2_enter)
        strcpy(word, "car2_enter");
    if (car2_pay)
        strcpy(word, "car2_pay");
}

void time_out_verifier(void)
{
    time_car1_exit = (millis() - start_car1_exit) / 1000.00;
    time_idle = (millis() - start_idle) / 1000.00;
    time_car1_car2_swap = (millis() - start_car1_car2_swap) / 1000.00;
    time_car2_enter = (millis() - start_car2_enter) / 1000.00;
    time_car2_pay = (millis() - start_car2_pay) / 1000.00;

    if ((time_car1_exit > TIMEOUT && car1_exit) ||
        (time_car1_car2_swap > TIMEOUT && car1_car2_swap) ||
        (time_car2_enter > TIMEOUT && car2_enter) ||
        (time_car2_pay > TIMEOUT && car2_pay))
    {
        Serial.println("TIMEOUT");
        rst_states();
    }
}

void rst_states(void)
{
    car1_exit = false;
    car1_car2_swap = false;
    car2_enter = false;
    car2_pay = false;
    idle = false;
}

inline bool is_car1_exit(void)
{
    return (!is_s1_active() && is_s2_active());
}
inline bool is_car1_car2_swap(void)
{
    return (is_s1_s2_active());
}
inline bool is_car2_enter(void)
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

inline bool word_verifier(String word)
{
    return word.equals("CAR_1_EXIT ") ||
           word.equals("SWAP ") ||
           word.equals("CAR_2_ENTER ") ||
           word.equals("CAR_2_PAY ") ||
           word.equals("IDLE ");
}

void csv_print(String string)
{
    static String last_string = String();

    if (word_verifier(last_string))
    {
        if (word_verifier(string))
        {
            Serial.print('\n');
            Serial.print(string);
        }
        else
            Serial.println(string);
    }
    else
    {
        Serial.print(string);
    }

    last_string = string;
}