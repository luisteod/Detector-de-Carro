# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 4 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 5 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2

# 7 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 8 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 16 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
int ledPin = 13; // select the pin for the LED

float dist_t1;
float dist_t2;
bool objectDetected = false;

static bool state_c = false;
static bool state_v = false;
static bool state_j = false;
static bool state_w = false;

static struct pt pt_av;
static int average_thread_run = 1;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(ledPin, 0x1);
  digitalWrite(ledPin, 0x0);
  (&pt_av)->lc = 
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 3 4
 __null
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
                ;
}

static int averageThread(struct pt *pt)
{
  { char PT_YIELD_FLAG = 1; do { if((pt)->lc != 
# 40 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 3 4
 __null
# 40 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
 ) { goto *(pt)->lc; } } while(0);

  while(1)
  {
    //Waits until some part of program set's threadStop Flag
    do { do { LC_LABEL45: ((pt)->lc) = &&LC_LABEL45; } while(0); if(!(average_thread_run != 0)) { return 0; } } while(0);

    Serial.print("average thead perfoming\n");

    float dist_t1_aux = 0;
    float dist_t2_aux = 0;

    for(int count = 0; count < 30 ; count++)
    {
      dist_t1_aux = dist_t1_aux + get_dist_s1();
      dist_t2_aux = dist_t2_aux + get_dist_s2();
      delay(20); //This delay is extreamly necessary for accurate measure
    }

    dist_t1 = dist_t1_aux/30;
    dist_t2 = dist_t2_aux/30;

    //Reset the flag
    average_thread_run = 0;
  }
  ; PT_YIELD_FLAG = 0; (pt)->lc = 
# 65 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 3 4
 __null
# 65 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
 ; return 3; };
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
  averageThread(&pt_av);


  // dist_t1 = get_dist_s1();
  // dist_t2 = get_dist_s2();  

  CarStopAlarm();
  isProximityState();

  Serial.print("S1: ");
  Serial.println(dist_t1);
  Serial.print("S2: ");
  Serial.println(dist_t2);

  //Release thread of average to run
  average_thread_run = 1;

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
