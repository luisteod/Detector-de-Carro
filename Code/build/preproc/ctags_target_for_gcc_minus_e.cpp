# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2
# 3 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\Code.ino" 2





int ledPin = 13; // select the pin for the LED

float dist_t1;
float dist_t2;
unsigned long timeStart = 0;
unsigned long timeInitial = 0;
unsigned long timePrevious = 0;
unsigned long timeInterval;
unsigned long timeTicket;
bool objectDetected = false;
bool objectDetectedd = false;
bool TimeShow = false;

void setup()
{
  Serial.begin(115200); // Inicializa a comunicação serial com taxa de 9600 bps
  pinMode(ledPin, 0x1);
  digitalWrite(ledPin, 0x0);
}

void CarStopAlarm()
{
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

      if (timeStop >= 1000 * (5) /* Tempo máximo de espera do carro na cancela*/) //Car stoped too long verification
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
  static unsigned long Dtc, Dtv, Dtj;

}

void loop()
{
  dist_t1 = get_dist_s1();
  dist_t2 = get_dist_s2();

  CarStopAlarm();

  Serial.print("S1: ");
  Serial.println(dist_t1);
  Serial.print("S2: ");
  Serial.println(dist_t2);

  delay(500);
}
# 1 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino"
# 2 "C:\\Users\\Hardware 1\\Desktop\\Detector-de-Carro\\Code\\sensors.ino" 2

extern float dist_t1, dist_t2;

int is_s1_active()
{
    if (dist_t1 < (5) /* Distância levemente abaixo da distância máxima (cm)*/)
        return 1;
    else
        return 0;
}

int is_s2_active()
{
    if (dist_t1 < (5) /* Distância levemente abaixo da distância máxima (cm)*/)
        return 1;
    else
        return 0;
}

float get_dist_s1()
{
    float sensity_t1 = analogRead(A0 /* select the input pin for the potentiometer*/);
    return sensity_t1 * (520) /* the max measurement vaule of the module is 520cm(a little bit longer than  effective max range)*/ / (1023.0) /* ADC accuracy of Arduino UNO is 10bit*/;
}

float get_dist_s2()
{
  float sensity_t2 = analogRead(A2 /* select the input pin for the potentiometer*/);
  return sensity_t2 * (520) /* the max measurement vaule of the module is 520cm(a little bit longer than  effective max range)*/ / (1023.0) /* ADC accuracy of Arduino UNO is 10bit*/;
}
