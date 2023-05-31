#include <pt-sem.h>
#include <pt.h>
#include <pt-sem.h>
#include <Arduino.h>
#include "sensors.h"
#include "interrupt.h"
#include "timer.h"
#include "state_mach.h"

#define TIME_MAX 0.01 // Tempo máximo de espera do carro na cancela

#define NUM_SAMPLE 30
#define STOP_LED 13
#define FREE_LED 9

float dist_t1 = 0;
float dist_t2 = 0;
bool objectDetected = false;

bool warning_flag = false;

// Thread Global variables
static struct pt pt_av;
static int average_thread_run = 1;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(STOP_LED, OUTPUT);
  pinMode(WARNING_LED, OUTPUT);
  pinMode(FREE_LED, OUTPUT);
  digitalWrite(FREE_LED, LOW);
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

    // Serial.print("average thead perfoming\n");

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

//Verifies if the local for pay is free and do something
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

void loop()
{
  averageThread(&pt_av);

  Serial.print("S1: ");
  Serial.println(dist_t1);
  Serial.print("S2: ");
  Serial.println(dist_t2);

  free_verify();
  CarStopAlarm();
  state_machine();

  //Release thread of average to run
  average_thread_run = 1;
}