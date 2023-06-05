#include "sensors.h"

extern float dist_t1, dist_t2;

inline bool is_s1_active(void)
{
    return (dist_t1 < (DIST_MAX + ERROR_MARGIN));
}

inline bool is_s2_active(void)
{
    return (dist_t2 < (DIST_MAX + ERROR_MARGIN));
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