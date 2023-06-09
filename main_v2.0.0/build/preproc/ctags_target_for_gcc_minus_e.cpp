# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2

float dist_t1;
float dist_t2;

void setup()
{
    Serial.begin(115200);

    pinMode((14) /* select the input pin for the potentiometer*/,0x0);
    pinMode((16) /* select the input pin for the potentiometer*/,0x0);
}

void loop()
{
 Serial.println(get_dist_s1());
    delay(500);
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino" 2

extern float dist_t1, dist_t2;

inline bool is_s1_active(void)
{
    return (dist_t1 < (80 /* Distância levemente abaixo da distância máxima (cm)*/ + 20));
}

inline bool is_s2_active(void)
{
    return (dist_t2 < (80 /* Distância levemente abaixo da distância máxima (cm)*/ + 20));
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
    dist_t1 = digitalRead((14) /* select the input pin for the potentiometer*/);
}

float get_dist_s2()
{
    dist_t2 = digitalRead((16) /* select the input pin for the potentiometer*/);
}
