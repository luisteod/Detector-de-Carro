#ifndef _SENSORS_H_
#define sensors.h
#endif

#define DIST_MAX 80         // Distância levemente abaixo da distância máxima (cm)
#define ERROR_MARGIN 20
#define MAX_RANG 520        // the max measurement vaule of the module is 520cm(a little bit longer than  effective max range)
#define ADC_SOLUTION 1023.0  // ADC accuracy of Arduino UNO is 10bit

#define PIN_S1  A0  // select the input pin for the potentiometer
#define PIN_S2 A2 // select the input pin for the potentiometer

inline int get_dist_s1(void);
inline int get_dist_s2(void);

inline bool is_s1_active(void);
inline bool is_s2_active(void);
inline bool is_s1_s2_active(void);

inline bool is_car1_exit(void);
inline bool is_car1_car2_swap(void);
inline bool is_car2_enter(void);
inline bool is_car2_pay(void);
inline bool is_idle(void);