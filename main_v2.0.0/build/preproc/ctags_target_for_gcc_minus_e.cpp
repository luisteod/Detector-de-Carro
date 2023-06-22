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
    // Serial.print("S1: ");
    // Serial.println(val_s1);
    // Serial.print("S2: ");
    // Serial.println(val_s2);

    // print_time = millis();
  }

  free_verify();
  CarStopAlarm();
  state_machineine();

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

# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\sensors.ino" 2

extern float val_s1;
extern float val_s2;

inline bool is_s1_active(void)
{
   // Serial.println(val_s1);
    return val_s1 > 0.6;
}

inline bool is_s2_active(void)
{
    return val_s2 > 0.6;
}

inline bool is_s1_s2_active(void)
{
    return (is_s1_active() && is_s2_active());
}

inline int get_dist_s1(void)
{
    return digitalRead(A0 /* select the input pin for the potentiometer*/);
}

inline int get_dist_s2(void)
{
    return digitalRead(A2 /* select the input pin for the potentiometer*/);
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_machine.ino"







# 9 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\state_machine.ino" 2

static bool car1_exit = false;
static bool car1_car2_swap = false;
static bool car2_enter = false;
static bool car2_pay = false;
static bool idle = false;

extern bool warning_flag;

void state_machineine(void)
{

    static unsigned long start_car1_exit = 0;
    static unsigned long start_car1_car2_swap = 0;
    static unsigned long start_car2_enter = 0;
    static unsigned long start_car2_pay = 0;
    static unsigned long start_idle = 0;

    volatile unsigned long time_car1_exit = 0;
    volatile unsigned long time_car1_car2_swap = 0;
    volatile unsigned long time_car2_enter = 0;
    volatile unsigned long time_car2_pay = 0;
    volatile unsigned long time_idle = 0;

    if (!car1_exit && is_car1_exit())
    {
        // Reset the alarm if new car enter
        warning_flag = false;
        if (car2_pay)
        {
            car2_pay = false;
            time_car2_pay = (float)(millis() - start_car2_pay) / 1000.000;
            csv_print(String((float)time_car2_pay, 1));
            if (time_car2_pay <= 5)
            {
                // Activates the indicator of invasor
                warning_flag = true;
                Serial.println("INVASOR");
            }
        }
        rst_states();
        car1_exit = true;
        csv_print(String("CAR_1_EXIT "));
        start_car1_exit = millis();
    }
    else if (!car1_car2_swap && car1_exit && is_car1_car2_swap())
    {
        car1_exit = false;
        time_car1_exit = (float)(millis() - start_car1_exit) / 1000.000;
        csv_print(String((float)time_car1_exit, 1));
        if (time_car1_exit <= 3)
        {
            car1_car2_swap = true;
            csv_print(String("SWAP "));
            start_car1_car2_swap = millis();
        }
    }
    else if (!car2_enter && (car1_car2_swap || idle) && is_car2_enter())
    {
        if (car1_car2_swap)
        {
            car1_car2_swap = false;
            time_car1_car2_swap = (float)(millis() - start_car1_car2_swap) / 1000.000;
            csv_print(String((float)time_car1_car2_swap, 1));
        }
        else if (idle)
        {
            idle = false;
            time_idle = (float)(millis() - start_idle) / 1000.000;
            csv_print(String((float)time_idle, 1));
        }

        if (time_car1_car2_swap <= 3 || time_idle <= 2)
        {
            car2_enter = true;
            csv_print(String("CAR_2_ENTER "));
            start_car2_enter = millis();
        }
    }
    else if (!car2_pay && car2_enter && is_car2_pay())
    {
        car2_enter = false;
        time_car2_enter = (float)(millis() - start_car2_enter) / 1000.000;
        csv_print(String((float)time_car2_enter, 1));
        if (time_car2_enter <= 3)
        {
            car2_pay = true;
            csv_print(String("CAR_2_PAY "));
            start_car2_pay = millis();
        }
    }
    else if (!idle && is_idle())
    {
        rst_states();
        idle = true;
        // Restarta a contagem com o tempo de início relativo
        // a última ativação do estado car1_entering.
        if (car1_exit)
        {
            csv_print(String("IDLE "));
            start_idle = millis();
        }
    }
    // Bengin of states chaining
    // Caso esteja vindo de idle, será feita a verificacao do
    // tempo decorrido para avaliar se irá para o prox estado.

    // else if (idle && is_car2_enter())
    // {
    //     idle = false;
    //     time_idle = (float)(millis() - start_idle) / 1000.000;
    //     csv_print(String((float)time_idle, 1));
    //     if (time_idle <= TIME_COND_CAR_SWAP)
    //         goto car2_enter;
    // }
    else
    {
        if (time_car1_car2_swap || time_car1_exit ||
            time_car2_enter || time_car2_pay > 10)
        {
            Serial.println("TIMEOUT");
            rst_states();
        }
    }
}

void rst_states(void)
{
    car1_exit = false;
    car1_car2_swap = false;
    car2_enter = false;
    car2_pay = false;
    idle = false;
}

inline bool is_car1_exit(void)
{
    return (!is_s1_active() && is_s2_active());
}
inline bool is_car1_car2_swap(void)
{
    return (is_s1_s2_active());
}
inline bool is_car2_enter(void)
{
    return (is_s1_active() && !is_s2_active());
}
inline bool is_car2_pay(void)
{
    return (is_s1_s2_active());
}
inline bool is_idle(void)
{
    return (!is_s1_active() && !is_s2_active());
}

inline bool word_verifier(String word)
{
    return word.equals("CAR_1_EXIT ") ||
           word.equals("SWAP ") ||
           word.equals("CAR_2_ENTER ") ||
           word.equals("CAR_2_PAY ") ||
           word.equals("IDLE ");
}

void csv_print(String string)
{
    static String last_string = String();

    if (word_verifier(last_string))
    {
        if (word_verifier(string))
        {
            Serial.print('\n');
            Serial.print(string);
        }
        else
            Serial.println(string);
    }
    else
    {
        Serial.print(string);
    }

    last_string = string;
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"


# 4 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 2
# 5 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 2

unsigned long elapsedTime, previousTime;

extern bool warning_flag;
float val_s1 = 0;
float val_s2 = 0;


/*Initializes the timer1 with a prescaler

 resulting an interruption each 1 second*/
# 15 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
void timer_initialize(void)
{
    
# 17 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   __asm__ __volatile__ ("cli" ::: "memory")
# 17 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
        ; // disable all interrupts

    
# 19 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB0)) 
# 19 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 20 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB1)) 
# 20 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 21 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB2)) 
# 21 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 0;
    // 90.3179190751445 Hz (16000000/((172+1)*1024))
    
# 23 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB3)) 
# 23 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 172;
    // CTC
    
# 25 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB0)) 
# 25 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 25 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   1
# 25 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                        );
    // Prescaler 1024
    
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0xB1)) 
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   2
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                       ) | (1 << 
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                                 1
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                                     ) | (1 << 
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                                               0
# 27 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                                                   );
    // Output Compare Match A Interrupt Enable
    
# 29 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x70)) 
# 29 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 29 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   1
# 29 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                         );

    
# 31 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x80)) 
# 31 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 32 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 32 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = 0;
    
# 33 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint16_t *)(0x84)) 
# 33 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 0;
    
# 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint16_t *)(0x88)) 
# 34 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
         = 31250; // compare match register 16MHz/256/2 = 31250
    
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = (1 << 
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                  3
# 35 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                       ); // CTC mode
    
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          |= (1 << 
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                   1
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                       ) | (1 << 
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                                 0
# 36 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                                     ); // Frequency 16Mhz/64 = 250 kHz
    
# 37 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   (*(volatile uint8_t *)(0x6F)) 
# 37 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
          = (1 << 
# 37 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
                  1
# 37 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
                        ); // Local interruption OCIE1A

    
# 39 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino" 3
   __asm__ __volatile__ ("sei" ::: "memory")
# 39 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\main_v2.0.0\\timer.ino"
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
    static float dist_t1_buff[200] = {0};
    static float dist_t2_buff[200] = {0};
    float sample = 0;

    sample = (float)get_dist_s1() / 200;
    val_s1 = val_s1 + sample - dist_t1_buff[counter];
    // Serial.print(String(val_s1));
    // Serial.print(" ");
    dist_t1_buff[counter] = sample;

    sample = (float)get_dist_s2() / 200;
    val_s2 = val_s2 + sample - dist_t2_buff[counter];
    // Serial.print(String(val_s2));
    // Serial.print("\n");

    dist_t2_buff[counter] = sample;
    counter++;
    if (counter == 200)
        counter = 0;
}
