#include <protothreads.h>
#include <pt-sem.h>
#include <pt-sleep.h>
#include <pt.h>
#include <Arduino.h>
#include "sensors.h"
#include "interrupt.h"
#include "timer.h"

#define TIME_MAX 1 // Tempo máximo de espera do carro na cancela
#define TEMPO_MAX_STATE_C 1
#define TEMPO_MAX_STATE_V 1
#define TEMPO_MAX_STATE_J 1
#define TEMPO_MAX_STATE_W 1

#define NUM_SAMPLE 30
#define STOP_LED 13

float dist_t1;
float dist_t2;
bool objectDetected = false;

static bool state_c = false;
static bool state_v = false;
static bool state_j = false;
static bool state_w = false;

bool warning_flag = false;

// Thread Global variables
static struct pt pt_av;
static int average_thread_run = 1;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(STOP_LED, OUTPUT);
  pinMode(WARNING_LED, OUTPUT);
  digitalWrite(STOP_LED, LOW);
  digitalWrite(WARNING_LED, LOW);

  // Timer initializes
  timer_initialize();

  // Threads initialize
  PT_INIT(&pt_av);
}

static int averageThread(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1)
  {
    // Waits until some part of program set's threadStop Flag
    PT_WAIT_UNTIL(pt, average_thread_run != 0);

    Serial.print("average thead perfoming\n");

    float dist_t1_aux = 0;
    float dist_t2_aux = 0;

    for (int count = 0; count < NUM_SAMPLE; count++)
    {
      dist_t1_aux = dist_t1_aux + get_dist_s1();
      dist_t2_aux = dist_t2_aux + get_dist_s2();
      delay(10); // This delay is extreamly necessary for accurate measure
    }

    dist_t1 = dist_t1_aux / NUM_SAMPLE;
    dist_t2 = dist_t2_aux / NUM_SAMPLE;

    // Reset the flag
    average_thread_run = 0;
  }
  PT_END(pt);
}

void CarStopAlarm()
{
  static unsigned long timeStart = 0;
  static unsigned long timeStop = 0;

  if ((is_s1_active()) && (is_s2_active()))
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

void state_machine()
{

  static unsigned long start_state_c, start_state_v, start_state_j, start_state_w;
  volatile unsigned long time_state_c, time_state_v, time_state_j, time_state_w;

  if (!state_c && is_state_c())
  {
    if (state_w)
    {
      state_w = false;
      time_state_w = start_state_w - millis();
    }
    if (time_state_w <= TEMPO_MAX_STATE_W * 1000)
    {
      //Activates the indicator of invasor
      warning_flag = true;
    }
    state_c = true;
    start_state_c = millis();
  }
  else if (!state_v && is_state_v())
  {
    state_c = false;
    time_state_c = start_state_c - millis();
    if (time_state_c <= TEMPO_MAX_STATE_C * 1000)
    {
      state_v = true;
      start_state_v = millis();
    }
  }
  else if (!state_j && is_state_j())
  {
    state_v = false;
    time_state_v = start_state_v - millis();
    if (time_state_v <= TEMPO_MAX_STATE_V * 1000)
    {
      state_j = true;
      start_state_j = millis();
    }
  }
  else if (!state_w && is_state_w())
  {
    state_j = false;
    time_state_j = start_state_j - millis();
    if (time_state_j <= TEMPO_MAX_STATE_J * 1000)
    {
      state_w = true;
      start_state_w = millis();
    }
  }
}

void loop()
{
  averageThread(&pt_av);

  CarStopAlarm();
  state_machine();

  Serial.print("S1: ");
  Serial.println(dist_t1);
  Serial.print("S2: ");
  Serial.println(dist_t2);

  // Release thread of average to run
  average_thread_run = 1;
}