#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
#include <pt-sem.h>
#include <pt.h>
#include <pt-sem.h>
#include <Arduino.h>
#include "sensors.h"
#include "interrupt.h"
#include "timer.h"
#include "state_mach.h"

#define TIME_MAX 0.01 // Tempo máximo de espera do carro na cancela
#define STOP_LED 13
#define FREE_LED 9

bool objectDetected = false;
bool warning_flag = false;

#line 17 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void setup();
#line 33 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void CarStopAlarm();
#line 64 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void free_verify(void);
#line 77 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void loop();
#line 6 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s1_active(void);
#line 12 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s2_active(void);
#line 17 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s1_s2_active(void);
#line 22 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
int get_dist_s1(void);
#line 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
int get_dist_s2(void);
#line 17 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
void state_machine(void);
#line 119 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
void rst_states(void);
#line 128 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
bool is_car1_exiting(void);
#line 132 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
bool is_car1_car2_swap(void);
#line 136 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
bool is_car2_entering(void);
#line 140 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
bool is_car2_pay(void);
#line 144 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
bool is_idle(void);
#line 13 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void timer_initialize(void);
#line 40 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void onTimer(void);
#line 70 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void sampling(void);
#line 17 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 115200 bps

  analogReference(DEFAULT);
  pinMode(STOP_LED, OUTPUT);
  pinMode(WARNING_LED, OUTPUT);
  pinMode(FREE_LED, OUTPUT);
  digitalWrite(FREE_LED, LOW);
  digitalWrite(STOP_LED, LOW);
  digitalWrite(WARNING_LED, LOW);

  // Timer initializes
  timer_initialize();
}

void CarStopAlarm()
{
  static unsigned long timeStart = 0;
  static unsigned long timeStop = 0;

  if (is_s1_s2_active())
  {
    if (!objectDetected)
    {
      objectDetected = true;
      timeStart = millis();
    }
    else
    {
      timeStop = millis() - timeStart;

      if (timeStop >= 1000 * TIME_MAX) // Car stoped too long verification
      {
        digitalWrite(STOP_LED, HIGH);
      }
    }
  }
  else
  {
    digitalWrite(STOP_LED, LOW);
    timeStop = 0;
    objectDetected = false;
  }
}

// Verifies if the local for pay is free and do something
void free_verify(void)
{
  if (is_idle())
  {
    digitalWrite(FREE_LED, HIGH);
  }
  else
  {
    digitalWrite(FREE_LED, LOW);
  }
}
long print_time = millis();

void loop()
{
  if (millis() - print_time > 100)
  {
    // Serial.print("S1: ");
    // Serial.println(val_s1);
    // Serial.print("S2: ");
    // Serial.println(val_s2);

    // print_time = millis();
  }

  free_verify();
  CarStopAlarm();
  state_machine();

}
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino"
#include "interrupt.h"

ISR(TIMER1_COMPA_vect)
{
    onTimer();
}

ISR(TIMER2_COMPA_vect)
{
    sampling();
}
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
#include "sensors.h"

extern float val_s1;
extern float val_s2;

inline bool is_s1_active(void)
{
   // Serial.println(val_s1);
    return val_s1 > 0.8;
}

inline bool is_s2_active(void)
{
    return val_s2 > 0.8;
}

inline bool is_s1_s2_active(void)
{
    return (is_s1_active() && is_s2_active());
}

inline int get_dist_s1(void)
{
    return digitalRead(PIN_S1);
}

inline int get_dist_s2(void)
{
    return digitalRead(PIN_S2);
}
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
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
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
#include "timer.h"
#include "sensors.h"

unsigned long elapsedTime, previousTime;

extern bool warning_flag;
float val_s1 = 0;
float val_s2 = 0;


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
    static float dist_t1_buff[BUFFER_SIZE] = {0};
    static float dist_t2_buff[BUFFER_SIZE] = {0};
    float sample = 0;

    sample = (float)get_dist_s1() / BUFFER_SIZE;
    val_s1 = val_s1 + sample - dist_t1_buff[counter];
    // Serial.print(String(val_s1));
    // Serial.print(" ");
    dist_t1_buff[counter] = sample;

    sample = (float)get_dist_s2() / BUFFER_SIZE;
    val_s2 = val_s2 + sample - dist_t2_buff[counter];
    // Serial.print(String(val_s2));
    // Serial.print("\n");

    dist_t2_buff[counter] = sample;
    counter++;
    if (counter == BUFFER_SIZE)
        counter = 0;
}
