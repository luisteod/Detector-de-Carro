#include <Arduino.h>
#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
#include "sensors.h"

float dist_t1;
float dist_t2;

#line 6 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void setup();
#line 14 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void loop();
#line 5 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s1_active(void);
#line 10 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s2_active(void);
#line 15 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_s1_s2_active(void);
#line 20 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car1_exiting(void);
#line 25 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car1_car2_swap(void);
#line 29 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car2_entering(void);
#line 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_car2_enter(void);
#line 39 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
bool is_idle(void);
#line 44 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
float get_dist_s1();
#line 49 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
float get_dist_s2();
#line 6 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
void setup()
{
    Serial.begin(115200);

    pinMode(PIN_S1,INPUT);
    pinMode(PIN_S2,INPUT);
}

void loop()
{
	Serial.println(get_dist_s1());
    delay(500);
}

#line 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
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
