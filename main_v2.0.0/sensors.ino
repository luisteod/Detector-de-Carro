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