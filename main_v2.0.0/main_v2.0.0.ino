#include "sensors.h"

float dist_t1;
float dist_t2;

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
