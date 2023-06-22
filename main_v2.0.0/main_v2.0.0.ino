#include <pt-sem.h>
#include <pt.h>
#include <pt-sem.h>
#include <Arduino.h>
#include "sensors.h"
#include "interrupt.h"
#include "timer.h"
#include "state_machine.h"

#define TIME_MAX 0.01 // Tempo máximo de espera do carro na cancela
#define STOP_LED 13
#define FREE_LED 9

bool objectDetected = false;
bool warning_flag = false;

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
  state_machineine();

}