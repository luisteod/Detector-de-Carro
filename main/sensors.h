#ifndef _SENSORS_H_
#define sensors.h
#endif

#define DIST_MAX 7         // Distância levemente abaixo da distância máxima (cm)
#define ERROR_MARGIN 3
#define MAX_RANG 520        // the max measurement vaule of the module is 520cm(a little bit longer than  effective max range)
#define ADC_SOLUTION 1023.0  // ADC accuracy of Arduino UNO is 10bit

#define PIN_S1  A0  // select the input pin for the potentiometer
#define PIN_S2 A2 // select the input pin for the potentiometer

float get_dist_s1(void);
float get_dist_s2(void);

inline bool is_s1_active(void);
inline bool is_s2_active(void);
inline bool is_s1_s2_active(void);

inline bool is_state_c(void);
inline bool is_state_v(void);
inline bool is_state_j(void);
inline bool is_state_w(void);
inline bool is_idle(void);