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

#line 18 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void setup();
#line 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void CarStopAlarm();
#line 65 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void free_verify(void);
#line 78 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void loop();
#line 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s1_active(void);
#line 8 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s2_active(void);
#line 13 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s1_s2_active(void);
#line 18 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car1_exiting(void);
#line 23 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car1_car2_swap(void);
#line 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car2_entering(void);
#line 32 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car2_enter(void);
#line 37 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_idle(void);
#line 42 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
int get_dist_s1(void);
#line 47 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
int get_dist_s2(void);
#line 10 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
void state_machine(void);
#line 87 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
void rst_states(void);
#line 14 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void timer_initialize(void);
#line 41 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void onTimer(void);
#line 71 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void sampling(void);
#line 18 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
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
    /* Serial.print("S1: ");
    Serial.println(val_s1);
    Serial.print("S2: ");
    Serial.println(val_s2); */

    print_time = millis();
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

inline bool is_s1_active(void)
{
    return 0;
}

inline bool is_s2_active(void)
{
    return 0;
}

inline bool is_s1_s2_active(void)
{
    return (is_s1_active() && is_s2_active());
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

inline bool is_car2_enter(void)
{
    return (is_s1_s2_active());
}

inline bool is_idle(void)
{
    return (!is_s1_active() && !is_s2_active());
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
    
    //Bengin of states chaining
    
    if (!car1_exiting && is_car1_exiting())
    {
        warning_flag = false; // Reset the alarm if new car enter
        if (car2_enter)
        {
            car2_enter = false;
            time_car2_enter = millis() - start_car2_enter;
            Serial.println("Tempo estado W : " + String((float)time_car2_enter / 1000, 3) + " seg");
            if (time_car2_enter <= TEMPO_MAX_CAR2_ENTER * 1000)
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
        if (time_car1_exiting <= TEMPO_MAX_CAR1_EXITING * 1000)
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
        if (time_car1_car2_swap <= TEMPO_MAX_CAR1_CAR2_SWAP * 1000)
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
        if (time_car2_entering <= TEMPO_MAX_CAR2_ENTERING * 1000)
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
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
#include "timer.h"
#include "sensors.h"

unsigned long elapsedTime, previousTime;

extern bool warning_flag;
extern int val_s1;
extern int val_s2;
extern int dist_t1_buff[BUFFER_SIZE];
extern int dist_t2_buff[BUFFER_SIZE];

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
    static float val_s1 = 0;
    static float val_s2 = 0;
    static float dist_t1_buff[BUFFER_SIZE] = {0};
    static float dist_t2_buff[BUFFER_SIZE] = {0};
    float sample = 0;

    sample = (float)get_dist_s1() / BUFFER_SIZE;
    Serial.println(sample);
    delay(500);
    val_s1 = val_s1 + sample - dist_t1_buff[counter];
    dist_t1_buff[counter] = sample;
    Serial.println(val_s1);

    delay(500);

    sample = (float)get_dist_s2() / BUFFER_SIZE;
    val_s2 = val_s2 + sample - dist_t2_buff[counter];
    dist_t2_buff[counter] = sample;
    counter++;
    if (counter == BUFFER_SIZE)
        counter = 0;
}
