#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
#include <Arduino.h>
#include "sensors.h"


#define Time_Max (5)         // Tempo máximo de espera do carro na cancela
#define Inter_Max (360)       // Tempo mínimo entre carros

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

#line 21 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
void setup();
#line 28 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
void CarStopAlarm();
#line 57 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
void isProximityState();
#line 63 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
void loop();
#line 5 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
int is_s1_active();
#line 13 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
int is_s2_active();
#line 21 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
float get_dist_s1();
#line 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
float get_dist_s2();
#line 21 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
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

      if (timeStop >= 1000 * Time_Max) //Car stoped too long verification
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

void isProximityState()
{
  static unsigned long Dtc, Dtv, Dtj;

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
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
#include "sensors.h"

extern float dist_t1, dist_t2;

int is_s1_active()
{
    if (dist_t1 < Dist_Max)
        return 1;
    else
        return 0;
}

int is_s2_active()
{
    if (dist_t1 < Dist_Max)
        return 1;
    else
        return 0;
}

float get_dist_s1()
{
    float sensity_t1 = analogRead(PIN_S1);
    return sensity_t1 * MAX_RANG / ADC_SOLUTION;
}

float get_dist_s2()
{
  float sensity_t2 = analogRead(PIN_S2);
  return sensity_t2 * MAX_RANG / ADC_SOLUTION;
}
