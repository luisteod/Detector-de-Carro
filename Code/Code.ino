#include <Arduino.h>
#include "sensors.h"

#define Time_Max (5)    // Tempo máximo de espera do carro na cancela
#define Inter_Max (360) // Tempo mínimo entre carros

int ledPin = 13; // select the pin for the LED

float dist_t1;
float dist_t2;
unsigned long timeStart = 0;
unsigned long timeInitial = 0;
unsigned long timePrevious = 0;
unsigned long timeInterval;
unsigned long timeTicket;
bool objectDetected = false;
bool objectDetectedd = false;
bool TimeShow = false;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void CarStopAlarm()
{
  static unsigned long timeStop;

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

      if (timeStop >= 1000 * Time_Max) // Car stoped too long verification
      {
        digitalWrite(ledPin, HIGH);
      }
    }
  }
  else
  {
    digitalWrite(ledPin, LOW);
    timeStop = 0;
    objectDetected = false;
  }
}

int is_state_c()
{
  if (!(is_s1_active() && !is_s2_active()))
  {
    return true;
  }
}

void isProximityState()
{
  static unsigned long start_Dtc;
  static unsigned long Dtc, Dtv, Dtj;

  static bool state_c = is_state_c();

  if (state_c)
  {
    start_Dtc = millis();
  }
}

void loop()
{
  dist_t1 = get_dist_s1();
  dist_t2 = get_dist_s2();

  CarStopAlarm();

  Serial.print("S1: ");
  Serial.println(dist_t1);
  Serial.print("S2: ");
  Serial.println(dist_t2);

  delay(500);
}