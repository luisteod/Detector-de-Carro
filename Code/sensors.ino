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