# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2







int ledPin = 13; // select the pin for the LED

float dist_t1;
float dist_t2;
bool objectDetected = false;

static bool state_c = false;
static bool state_v = false;
static bool state_j = false;
static bool state_w = false;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(ledPin, 0x1);
  digitalWrite(ledPin, 0x0);
}

void CarStopAlarm()
{
  static unsigned long timeStart;
  static unsigned long timeStop;

  if ((is_s1_active()) && (is_s2_active()))
  {
    if (!objectDetected)
    {
      objectDetected = true;
      timeStart = millis();
    }
    else
    {
      timeStop = millis() - timeStart;

      if (timeStop >= 1000 * 1 /* Tempo máximo de espera do carro na cancela*/) // Car stoped too long verification
      {
        digitalWrite(ledPin, 0x1);
      }
    }
  }
  else
  {
    digitalWrite(ledPin, 0x0);
    timeStop = 0;
    objectDetected = false;
  }
}

void isProximityState()
{

  static unsigned long start_state_c, start_state_v, start_state_j, start_state_w;
  volatile unsigned long time_state_c, time_state_v, time_state_j, time_state_w;


  if (!state_c && is_state_c())
  {
    state_c = true;
    start_state_c = millis();
  }
  else if (!state_v && is_state_v())
  {
    state_c = false;
    time_state_c = start_state_c - millis();
    if (time_state_c <= 1 * 1000)
    {
      state_v = true;
      start_state_v = millis();
    }
  }
  else if (!state_j && is_state_j())
  {
    state_v = false;
    time_state_v = start_state_v - millis();
    if (time_state_v <= 1 * 1000)
    {
      state_j = true;
      start_state_j = millis();
    }
  }
  else if (!state_w && is_state_w())
  {
    state_j = false;
    time_state_j = start_state_j - millis();
    if (time_state_j <= 1 * 1000)
    {
      state_w = true;
      start_state_w = millis();
    }
  }
  else if (is_idle())
  {
    time_state_w = start_state_w - millis();
    if (time_state_w <= 1 * 1000)
    {
    }
  }
  else
  {
  }
}

void loop()
{
  dist_t1 = get_dist_s1();
  dist_t2 = get_dist_s2();

  CarStopAlarm();
  isProximityState();

  Serial.print("S1: ");
  Serial.println(dist_t1);
  Serial.print("S2: ");
  Serial.println(dist_t2);

  delay(500);
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino" 2

extern float dist_t1, dist_t2;

inline bool is_s1_active(void)
{
    return (dist_t1 < 7 /* Distância levemente abaixo da distância máxima (cm)*/);
}

inline bool is_s2_active(void)
{
    return (dist_t2 < 7 /* Distância levemente abaixo da distância máxima (cm)*/);
}

inline bool is_s1_s2_active(void)
{
    return (is_s1_active() && is_s2_active());
}

inline bool is_state_c(void)
{
    return (is_s1_active() && !is_s2_active());
}

inline bool is_state_v(void)
{
    return (is_s1_s2_active());
}
inline bool is_state_j(void)
{
    return (!is_s1_active() && is_s2_active());
}

inline bool is_state_w(void)
{
    return (is_s1_s2_active());
}

inline bool is_idle(void)
{
    return !(is_s1_s2_active());
}

float get_dist_s1()
{
    float sensity_t1 = analogRead(A0 /* select the input pin for the potentiometer*/);
    return sensity_t1 * 520 /* the max measurement vaule of the module is 520cm(a little bit longer than  effective max range)*/ / 1023.0 /* ADC accuracy of Arduino UNO is 10bit*/;
}

float get_dist_s2()
{
    float sensity_t2 = analogRead(A2 /* select the input pin for the potentiometer*/);
    return sensity_t2 * 520 /* the max measurement vaule of the module is 520cm(a little bit longer than  effective max range)*/ / 1023.0 /* ADC accuracy of Arduino UNO is 10bit*/;
}
