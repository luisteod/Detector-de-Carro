# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 4 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 5 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 6 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 7 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 8 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2
# 9 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino" 2






bool objectDetected = false;
bool warning_flag = false;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 115200 bps

  analogReference(1);
  pinMode(13, 0x1);
  pinMode(8, 0x1);
  pinMode(9, 0x1);
  digitalWrite(9, 0x0);
  digitalWrite(13, 0x0);
  digitalWrite(8, 0x0);

  // Timer initializes
  timer_initialize();
}

void CarStopAlarm()
{
  static unsigned long timeStart = 0;
  static unsigned long timeStop = 0;

  if (is_s1_s2_active())
  {
    if (!objectDetected)
    {
      objectDetected = true;
      timeStart = millis();
    }
    else
    {
      timeStop = millis() - timeStart;

      if (timeStop >= 1000 * 0.01 /* Tempo máximo de espera do carro na cancela*/) // Car stoped too long verification
      {
        digitalWrite(13, 0x1);
      }
    }
  }
  else
  {
    digitalWrite(13, 0x0);
    timeStop = 0;
    objectDetected = false;
  }
}

// Verifies if the local for pay is free and do something
void free_verify(void)
{
  if (is_idle())
  {
    digitalWrite(9, 0x1);
  }
  else
  {
    digitalWrite(9, 0x0);
  }
}
long print_time = millis();

void loop()
{
  if (millis() - print_time > 100)
  {
    /* Serial.print("S1: ");

    Serial.println(val_s1);

    Serial.print("S2: ");

    Serial.println(val_s2); */
# 87 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\main_v2.0.0.ino"
    print_time = millis();
  }

  free_verify();
  CarStopAlarm();
  state_machine();

}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino" 2


# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino" 3
extern "C" void __vector_11 /* Timer/Counter1 Compare Match A */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_11 /* Timer/Counter1 Compare Match A */ (void)

# 4 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino"
{
    onTimer();
}


# 8 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino" 3
extern "C" void __vector_7 /* Timer/Counter2 Compare Match A */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_7 /* Timer/Counter2 Compare Match A */ (void)

# 9 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\interrupt.ino"
{
    sampling();
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino" 2

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
    return digitalRead(A0 /* select the input pin for the potentiometer*/);
}

inline int get_dist_s2(void)
{
    return digitalRead(A2 /* select the input pin for the potentiometer*/);
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_mach.ino" 2

static bool car1_exiting = false;
static bool car1_car2_swap = false;
static bool car2_entering = false;
static bool car2_enter = false;

extern bool warning_flag;

void state_machine(void)
{

    static unsigned long start_car1_exiting = 0;
    static unsigned long start_car1_car2_swap = 0;
    static unsigned long start_car2_entering = 0;
    static unsigned long start_car2_enter = 0;

    volatile unsigned long time_car1_exiting = 0;
    volatile unsigned long time_car1_car2_swap = 0;
    volatile unsigned long time_car2_entering = 0;
    volatile unsigned long time_car2_enter = 0;

    //Bengin of states chaining

    if (!car1_exiting && is_car1_exiting())
    {
        warning_flag = false; // Reset the alarm if new car enter
        if (car2_enter)
        {
            car2_enter = false;
            time_car2_enter = millis() - start_car2_enter;
            Serial.println("Tempo estado W : " + String((float)time_car2_enter / 1000, 3) + " seg");
            if (time_car2_enter <= 5 * 1000)
            {
                // Activates the indicator of invasor
                warning_flag = true;
                Serial.print("****** INVASOR *********\n");
            }
        }
        rst_states(); // Reseting all flags
        car1_exiting = true;
        Serial.print("=====> C ativo <=========\n");
        start_car1_exiting = millis();
    }
    else if (!car1_car2_swap && car1_exiting && is_car1_car2_swap())
    {
        car1_exiting = false;
        time_car1_exiting = millis() - start_car1_exiting;
        Serial.println("Tempo estado C : " + String((float)time_car1_exiting / 1000, 3) + " seg");
        if (time_car1_exiting <= 10 * 1000)
        {
            car1_car2_swap = true;
            Serial.print("=====> V ativo <========\n");
            start_car1_car2_swap = millis();
        }
    }
    else if (!car2_entering && car1_car2_swap && is_car2_entering())
    {
        car1_car2_swap = false;
        time_car1_car2_swap = millis() - start_car1_car2_swap;
        Serial.println("Tempo estado V : " + String((float)time_car1_car2_swap / 1000, 3) + " seg");
        if (time_car1_car2_swap <= 10 * 1000)
        {
            car2_entering = true;
            Serial.print("=====> J ativo <=========\n");
            start_car2_entering = millis();
        }
    }
    else if (!car2_enter && car2_entering && is_car2_enter())
    {
        car2_entering = false;
        time_car2_entering = millis() - start_car2_entering;
        Serial.println("Tempo estado J : " + String((float)time_car2_entering / 1000, 3) + " seg");
        if (time_car2_entering <= 10 * 1000)
        {
            car2_enter = true;
            Serial.print("=====> W ativo <=========\n");
            start_car2_enter = millis();
        }
    }
    else if(is_idle())
    {
        rst_states(); //Reset states 
    }
}

void rst_states(void)
{
    car1_exiting = false;
    car1_car2_swap = false;
    car2_entering = false;
    car2_enter = false;
    //Serial.println("RESET");
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 2
# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 2

unsigned long elapsedTime, previousTime;

extern bool warning_flag;
extern int val_s1;
extern int val_s2;
extern int dist_t1_buff[50];
extern int dist_t2_buff[50];

/*Initializes the timer1 with a prescaler

 resulting an interruption each 1 second*/
# 14 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void timer_initialize(void)
{
    
# 16 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   __asm__ __volatile__ ("cli" ::: "memory")
# 16 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
        ; // disable all interrupts

    
# 18 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB0)) 
# 18 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 19 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB1)) 
# 19 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 20 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB2)) 
# 20 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 0;
    // 90.3179190751445 Hz (16000000/((172+1)*1024))
    
# 22 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB3)) 
# 22 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 172;
    // CTC
    
# 24 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB0)) 
# 24 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 24 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   1
# 24 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                        );
    // Prescaler 1024
    
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB1)) 
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   2
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                       ) | (1 << 
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                                 1
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                                     ) | (1 << 
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                                               0
# 26 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                                                   );
    // Output Compare Match A Interrupt Enable
    
# 28 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x70)) 
# 28 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 28 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   1
# 28 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                         );

    
# 30 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x80)) 
# 30 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 31 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 31 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 32 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint16_t *)(0x84)) 
# 32 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 0;
    
# 33 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint16_t *)(0x88)) 
# 33 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 31250; // compare match register 16MHz/256/2 = 31250
    
# 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = (1 << 
# 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                  3
# 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                       ); // CTC mode
    
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   1
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                       ) | (1 << 
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                                 0
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                                     ); // Frequency 16Mhz/64 = 250 kHz
    
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x6F)) 
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = (1 << 
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                  1
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                        ); // Local interruption OCIE1A

    
# 38 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   __asm__ __volatile__ ("sei" ::: "memory")
# 38 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
        ; // enable all interrupts
}

void onTimer(void)
{
    // warning_flag = true;
    if (warning_flag)
    {
        static boolean state = 0x1;
        elapsedTime = millis() - previousTime;
        // Serial.print(F("Set LED 13 : "));
        if (state)
        {
            // Serial.print(F("ON"));
        }
        else
        {
            // Serial.print(F("OFF"));
        }
        // Serial.print(F(" - "));
        // Serial.print(elapsedTime);
        // Serial.println(F("ms"));
        digitalWrite(8, state);
        state = !state;

        previousTime = millis();
    }
    else
    {
        digitalWrite(8, 0x0);
    }
}

void sampling(void)
{
    static int counter = 0;
    static float val_s1 = 0;
    static float val_s2 = 0;
    static float dist_t1_buff[50] = {0};
    static float dist_t2_buff[50] = {0};
    float sample = 0;

    sample = (float)get_dist_s1() / 50;
    Serial.println(sample);
    delay(500);
    val_s1 = val_s1 + sample - dist_t1_buff[counter];
    dist_t1_buff[counter] = sample;
    Serial.println(val_s1);

    delay(500);

    sample = (float)get_dist_s2() / 50;
    val_s2 = val_s2 + sample - dist_t2_buff[counter];
    dist_t2_buff[counter] = sample;
    counter++;
    if (counter == 50)
        counter = 0;
}
