#include "sensors.h"

extern float val_s1;
extern float val_s2;

inline bool is_s1_active(void)
{
   // Serial.println(val_s1);
    return val_s1 > 0.8;
}

inline bool is_s2_active(void)
{
    return val_s2 > 0.8;
}

inline bool is_s1_s2_active(void)
{
    return (is_s1_active() && is_s2_active());
}

inline int get_dist_s1(void)
{
    return digitalRead(PIN_S1);
}

inline int get_dist_s2(void)
{
    return digitalRead(PIN_S2);
}