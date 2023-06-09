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
    dist_t1 = digitalRead(PIN_S1);
}

float get_dist_s2()
{
    dist_t2 = digitalRead(PIN_S2);
}